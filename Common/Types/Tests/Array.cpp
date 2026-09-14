/*
 * ──────────────────────────────────────────────
 * Project   : Sure
 * File      : Array
 * Author    : Emad Redwan
 * Created   : 9/13/26
 * License   : MIT
 * ──────────────────────────────────────────────
 */


#include <gtest/gtest.h>
#include "../Array.hpp"
#include "../../Ownership/Move.hpp"


using namespace sure;


// ------------------------- Helper types -------------------------


// Plain trivially-copyable type (simulates an MMIO/register-like struct):
// no clone(), just ordinary copy/move — exercises the `Copyable` fallback path.
struct PlainPoint {
    I32 x = 0;
    I32 y = 0;

    constexpr PlainPoint() noexcept = default;
    constexpr explicit PlainPoint(I32 vx, I32 vy = 0) noexcept : x(vx), y(vy) {}

    bool operator==(const PlainPoint& o) const noexcept { return x == o.x && y == o.y; }
};


// Move-only, NOT copyable, NOT clonable. Verifies Array can still hold and
// move such a type without ever needing clone()/at_clone()/get_clone().
struct MoveOnlyStrict {
    I32 v;

    explicit MoveOnlyStrict(I32 x = 0) noexcept : v(x) {}
    MoveOnlyStrict(const MoveOnlyStrict&) = delete;
    MoveOnlyStrict& operator=(const MoveOnlyStrict&) = delete;

    MoveOnlyStrict(MoveOnlyStrict&& o) noexcept : v(o.v) { o.v = -1; }
    MoveOnlyStrict& operator=(MoveOnlyStrict&& o) noexcept {
        if (this != &o) { v = o.v; o.v = -1; }
        return *this;
    }

    bool operator==(const MoveOnlyStrict& o) const noexcept { return v == o.v; }
};


// Explicitly Clonable (not Copyable — copy ctor/assign deleted), move-only.
// Verifies the `Clonable` path is preferred over `Copyable` when both could apply,
// and that clone() is actually invoked (not silently skipped).
struct CloneOnly {
    I32 v;
    static inline I32 clone_calls = 0;

    explicit CloneOnly(I32 x = 0) noexcept : v(x) {}
    CloneOnly(const CloneOnly&) = delete;
    CloneOnly& operator=(const CloneOnly&) = delete;

    CloneOnly(CloneOnly&& o) noexcept : v(o.v) { o.v = -1; }
    CloneOnly& operator=(CloneOnly&& o) noexcept {
        if (this != &o) { v = o.v; o.v = -1; }
        return *this;
    }

    NODISCARD_
    CloneOnly clone() const noexcept {
        ++clone_calls;
        return CloneOnly(v);
    }

    bool operator==(const CloneOnly& o) const noexcept { return v == o.v; }
};


// CopyConstructible but NOT CopyAssignable (const member blocks assignment).
// Verifies Array::clone() (needs only CopyConstructible) still works for a type
// that at_clone()/get_clone() (need full Copyable) would refuse to compile for.
struct CopyCtorOnly {
    const I32 v;

    explicit CopyCtorOnly(I32 x = 0) noexcept : v(x) {}
    CopyCtorOnly(const CopyCtorOnly&) noexcept = default;
    CopyCtorOnly(CopyCtorOnly&&) noexcept = default;

    bool operator==(const CopyCtorOnly& o) const noexcept { return v == o.v; }
};


// Tracks constructions/destructions to verify no leaks/double-destroys.
struct LifetimeTracker {
    static inline I32 constructions = 0;
    static inline I32 destructions  = 0;

    I32 payload;

    explicit LifetimeTracker(I32 p = 0) noexcept : payload(p) { ++constructions; }
    LifetimeTracker(const LifetimeTracker& o) noexcept : payload(o.payload) { ++constructions; }
    LifetimeTracker(LifetimeTracker&& o) noexcept : payload(o.payload) { ++constructions; o.payload = -1; }
    LifetimeTracker& operator=(LifetimeTracker&& o) noexcept { payload = o.payload; o.payload = -1; return *this; }
    ~LifetimeTracker() noexcept { ++destructions; }

    bool operator==(const LifetimeTracker& o) const noexcept { return payload == o.payload; }
};


// ------------------------- Construction -------------------------


TEST(ArrayConstruction, DefaultConstructsWithDefaultConstructibleT) {
    let a = Array<I32, 3>{};

    EXPECT_EQ(a.len(), 3);
    EXPECT_EQ(a[0], 0);
    EXPECT_EQ(a[1], 0);
    EXPECT_EQ(a[2], 0);
}


TEST(ArrayConstruction, VariadicConstructorSetsAllElementsInOrder) {
    let a = Array<I32, 4>(10, 20, 30, 40);

    EXPECT_EQ(a[0], 10);
    EXPECT_EQ(a[1], 20);
    EXPECT_EQ(a[2], 30);
    EXPECT_EQ(a[3], 40);
}


TEST(ArrayConstruction, VariadicConstructorWorksWithMoveOnlyType) {
    mut a = Array<MoveOnlyStrict, 2>(MoveOnlyStrict(1), MoveOnlyStrict(2));

    EXPECT_EQ(a[0].v, 1);
    EXPECT_EQ(a[1].v, 2);
}


TEST(ArrayConstruction, SingleElementArrayWorks) {
    let a = Array<I32, 1>(42);

    EXPECT_EQ(a.len(), 1);
    EXPECT_EQ(a[0], 42);
}


// ------------------------- operator[] -------------------------


TEST(ArrayIndexOperator, MutableReferenceAllowsWrite) {
    mut a = Array<I32, 3>(1, 2, 3);

    a[1] = 99;

    EXPECT_EQ(a[1], 99);
}


TEST(ArrayIndexOperator, ConstOverloadReturnsReadOnlyReference) {
    let a = Array<I32, 3>(1, 2, 3);

    let& v = a[2];

    EXPECT_EQ(v, 3);
}


TEST(ArrayIndexOperator, LastValidIndexIsAccessible) {
    let a = Array<I32, 5>(0, 1, 2, 3, 4);

    EXPECT_EQ(a[4], 4);
}


TEST(ArrayIndexOperator, OutOfRangeAborts) {
    let a = Array<I32, 3>(1, 2, 3);

    ASSERT_DEATH({ (Void)a[3]; }, "Array length exceeded");
}


TEST(ArrayIndexOperator, MutableOutOfRangeAborts) {
    mut a = Array<I32, 3>(1, 2, 3);

    ASSERT_DEATH({ a[100] = 5; }, "Array length exceeded");
}


// ------------------------- at_clone -------------------------


TEST(ArrayAtClone, CopyableTypeReturnsIndependentCopy) {
    let a = Array<PlainPoint, 2>(PlainPoint(1, 2), PlainPoint(3, 4));

    let p = a.at_clone(1);

    EXPECT_EQ(p, PlainPoint(3, 4));
}


TEST(ArrayAtClone, ClonableTypeInvokesClone) {
    CloneOnly::clone_calls = 0;
    let a = Array<CloneOnly, 2>(CloneOnly(7), CloneOnly(8));

    let c = a.at_clone(0);

    EXPECT_EQ(CloneOnly::clone_calls, 1);
    EXPECT_EQ(c.v, 7);
}


TEST(ArrayAtClone, OutOfRangeAborts) {
    let a = Array<PlainPoint, 2>(PlainPoint(1, 1), PlainPoint(2, 2));

    ASSERT_DEATH({ (Void)a.at_clone(2); }, "Array length exceeded");
}


// ------------------------- get_ref -------------------------


TEST(ArrayGetRef, MutableInRangeReturnsSomeAndAllowsMutation) {
    mut a = Array<I32, 3>(1, 2, 3);

    mut ref = a.get_ref(1);
    ASSERT_TRUE(ref.is_some());

    ref.value_or_abort() = 42;

    EXPECT_EQ(a[1], 42);
}


TEST(ArrayGetRef, MutableAtBoundaryReturnsNone) {
    mut a = Array<I32, 3>(1, 2, 3);

    let ref = a.get_ref(3);

    EXPECT_TRUE(ref.is_none());
}


TEST(ArrayGetRef, ConstInRangeReturnsSome) {
    let a = Array<I32, 3>(1, 2, 3);

    let ref = a.get_ref(0);

    ASSERT_TRUE(ref.is_some());
    EXPECT_EQ(do_move(ref).value_or_abort(), 1);
}


TEST(ArrayGetRef, ConstAtBoundaryReturnsNone) {
    let a = Array<I32, 3>(1, 2, 3);

    let ref = a.get_ref(3);

    EXPECT_TRUE(ref.is_none());
}


TEST(ArrayGetRef, FarOutOfRangeReturnsNoneNotAbort) {
    let a = Array<I32, 3>(1, 2, 3);

    let ref = a.get_ref(1000);

    EXPECT_TRUE(ref.is_none());
}


// ------------------------- get_clone -------------------------


TEST(ArrayGetClone, CopyableInRangeReturnsSomeWithCopy) {
    let a = Array<PlainPoint, 2>(PlainPoint(5, 6), PlainPoint(7, 8));

    let opt = a.get_clone(0);

    ASSERT_TRUE(opt.is_some());
    EXPECT_EQ(do_move(opt).value_or_abort(), PlainPoint(5, 6));
}


TEST(ArrayGetClone, CopyableAtBoundaryReturnsNone) {
    let a = Array<PlainPoint, 2>(PlainPoint(1, 1), PlainPoint(2, 2));

    let opt = a.get_clone(2);

    EXPECT_TRUE(opt.is_none());
}


TEST(ArrayGetClone, ClonableTypeInvokesClone) {
    CloneOnly::clone_calls = 0;
    let a = Array<CloneOnly, 2>(CloneOnly(3), CloneOnly(4));

    let opt = a.get_clone(1);

    ASSERT_TRUE(opt.is_some());
    EXPECT_EQ(CloneOnly::clone_calls, 1);
    EXPECT_EQ(do_move(opt).value_or_abort().v, 4);
}


// ------------------------- clone() -------------------------


TEST(ArrayClone, ProducesElementwiseIndependentCopyForCopyable) {
    let a = Array<PlainPoint, 3>(PlainPoint(1, 1), PlainPoint(2, 2), PlainPoint(3, 3));

    let b = a.clone();

    EXPECT_EQ(b[0], PlainPoint(1, 1));
    EXPECT_EQ(b[1], PlainPoint(2, 2));
    EXPECT_EQ(b[2], PlainPoint(3, 3));
}


TEST(ArrayClone, CallsCloneForEveryElementWhenClonable) {
    CloneOnly::clone_calls = 0;
    let a = Array<CloneOnly, 4>(CloneOnly(1), CloneOnly(2), CloneOnly(3), CloneOnly(4));

    let b = a.clone();

    EXPECT_EQ(CloneOnly::clone_calls, 4);
    EXPECT_EQ(b[0].v, 1);
    EXPECT_EQ(b[3].v, 4);
}


TEST(ArrayClone, WorksForCopyConstructibleButNotAssignableType) {
    // CopyCtorOnly is CopyConstructible but not Copyable (no operator=).
    // clone() only requires CopyConstructible, so this must still compile & work,
    // even though at_clone()/get_clone() (which require Copyable) would not.
    let a = Array<CopyCtorOnly, 2>(CopyCtorOnly(9), CopyCtorOnly(10));

    let b = a.clone();

    EXPECT_EQ(b[0], CopyCtorOnly(9));
    EXPECT_EQ(b[1], CopyCtorOnly(10));
}


TEST(ArrayClone, DoesNotLeakOrDoubleDestroyElements) {
    LifetimeTracker::constructions = 0;
    LifetimeTracker::destructions  = 0;

    {
        let a = Array<LifetimeTracker, 3>(LifetimeTracker(1), LifetimeTracker(2), LifetimeTracker(3));
        {
            let b = a.clone();
            EXPECT_EQ(b[0].payload, 1);
        }
    }

    EXPECT_EQ(LifetimeTracker::constructions, LifetimeTracker::destructions);
}


// ------------------------- Move -------------------------


TEST(ArrayMove, MoveConstructorTransfersValues) {
    mut a = Array<MoveOnlyStrict, 3>(MoveOnlyStrict(1), MoveOnlyStrict(2), MoveOnlyStrict(3));

    let b = do_move(a);

    EXPECT_EQ(b[0].v, 1);
    EXPECT_EQ(b[1].v, 2);
    EXPECT_EQ(b[2].v, 3);

    // Moved-from elements are left in their moved-from sentinel state.
    EXPECT_EQ(a[0].v, -1);
    EXPECT_EQ(a[1].v, -1);
    EXPECT_EQ(a[2].v, -1);
}


TEST(ArrayMove, MoveAssignmentTransfersValues) {
    mut a = Array<MoveOnlyStrict, 2>(MoveOnlyStrict(5), MoveOnlyStrict(6));
    mut b = Array<MoveOnlyStrict, 2>(MoveOnlyStrict(0), MoveOnlyStrict(0));

    b = do_move(a);

    EXPECT_EQ(b[0].v, 5);
    EXPECT_EQ(b[1].v, 6);
    EXPECT_EQ(a[0].v, -1);
    EXPECT_EQ(a[1].v, -1);
}


TEST(ArrayMove, SelfMoveAssignmentIsNoOp) {
    mut a = Array<MoveOnlyStrict, 2>(MoveOnlyStrict(1), MoveOnlyStrict(2));

    a = do_move(a);

    EXPECT_EQ(a[0].v, 1);
    EXPECT_EQ(a[1].v, 2);
}


TEST(ArrayMove, MoveDoesNotLeakOrDoubleDestroy) {
    LifetimeTracker::constructions = 0;
    LifetimeTracker::destructions  = 0;

    {
        mut a = Array<LifetimeTracker, 2>(LifetimeTracker(1), LifetimeTracker(2));
        let b = do_move(a);
        (Void)b;
    }

    EXPECT_EQ(LifetimeTracker::constructions, LifetimeTracker::destructions);
}


// ------------------------- Iterators -------------------------


TEST(ArrayIterator, RangeBasedForVisitsAllElementsInOrder) {
    let a = Array<I32, 4>(10, 20, 30, 40);

    mut sum = 0;
    mut count = 0;
    for (let& v : a) {
        sum += v;
        ++count;
    }

    EXPECT_EQ(count, 4);
    EXPECT_EQ(sum, 100);
}


TEST(ArrayIterator, RangeBasedForAllowsMutationOnNonConstArray) {
    mut a = Array<I32, 3>(1, 2, 3);

    for (mut& v : a) {
        v *= 10;
    }

    EXPECT_EQ(a[0], 10);
    EXPECT_EQ(a[1], 20);
    EXPECT_EQ(a[2], 30);
}


TEST(ArrayIterator, ConstArrayIterationGivesReadOnlyAccess) {
    let a = Array<I32, 3>(7, 8, 9);

    mut sum = 0;
    for (let& v : a) {
        sum += v;
    }

    EXPECT_EQ(sum, 24);
}


TEST(ArrayIterator, BeginNotEqualEndForNonEmptyArray) {
    let a = Array<I32, 2>(1, 2);

    EXPECT_TRUE(a.begin() != a.end());
}


TEST(ArrayIterator, AdvancingBeginByLenReachesEnd) {
    let a = Array<I32, 3>(1, 2, 3);

    EXPECT_TRUE(a.begin() + 3 == a.end());
}


TEST(ArrayIterator, PlusOffsetPointsToCorrectElement) {
    let a = Array<I32, 4>(10, 20, 30, 40);

    let it = a.begin() + 2;

    EXPECT_EQ(*it, 30);
}


TEST(ArrayIterator, PlusOffsetDoesNotMutateOriginalIterator) {
    let a = Array<I32, 3>(1, 2, 3);
    let original = a.begin();

    let advanced = original + 2;

    EXPECT_EQ(*original, 1);
    EXPECT_EQ(*advanced, 3);
}


TEST(ArrayIterator, PreIncrementAdvancesAndReturnsSelf) {
    let a = Array<I32, 3>(1, 2, 3);
    mut it = a.begin();

    let& same = ++it;

    EXPECT_EQ(*it, 2);
    EXPECT_EQ(&same, &it);
}


TEST(ArrayIterator, DefaultConstructedIteratorDereferenceAborts) {
    Iterator<I32> it;

    ASSERT_DEATH({ (Void)*it; }, "Null pointer");
}


// ------------------------- Misc: data() / len() -------------------------


TEST(ArrayMisc, LenReturnsCompileTimeCapacity) {
    let a = Array<I32, 7>{};

    EXPECT_EQ(a.len(), 7);
}


TEST(ArrayMisc, DataPointerMatchesFirstElementAddress) {
    mut a = Array<I32, 3>(1, 2, 3);

    EXPECT_EQ(a.data(), &a[0]);
    EXPECT_EQ(*a.data(), 1);
}


TEST(ArrayMisc, ConstDataPointerAccessibleOnConstArray) {
    let a = Array<I32, 3>(1, 2, 3);

    const I32* p = a.data();

    EXPECT_EQ(*p, 1);
}


// ------------------------- Copy policy -------------------------


TEST(ArrayCopyPolicy, ArrayIsNotImplicitlyCopyable) {
    static_assert(!CopyConstructible<Array<I32, 3>>,
        "Array must not be implicitly copy-constructible (NoDefaultCopy).");
    static_assert(!CopyAssignable<Array<I32, 3>>,
        "Array must not be implicitly copy-assignable (NoDefaultCopy).");

    SUCCEED();
}