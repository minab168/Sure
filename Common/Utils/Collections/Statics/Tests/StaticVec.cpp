/*
 * ──────────────────────────────────────────────
 * Project   : Sure
 * File      : StaticVec
 * Author    : Emad Redwan
 * License   : MIT
 * ──────────────────────────────────────────────
 */


#include <gtest/gtest.h>

#include "../StaticVec.hpp"
#include "../../../../Ownership/Move.hpp"


using namespace sure;
using namespace sure::collections;


// ------------------------- Helper types -------------------------


// Trivially copyable (implicit copy/move), simulates a POD-like element.
struct PlainPoint {
    I32 x = 0;
    I32 y = 0;

    constexpr PlainPoint() noexcept = default;
    constexpr explicit PlainPoint(I32 vx, I32 vy = 0) noexcept : x(vx), y(vy) {}

    bool operator==(const PlainPoint& o) const noexcept { return x == o.x && y == o.y; }
};


// Move-only, NOT copyable, NOT clonable. Exercises the non-trivial move path.
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


// Explicitly Clonable, move-only (no copy). Verifies clone() is actually
// invoked rather than silently falling back to something else.
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

    NODISCARD_ CloneOnly clone() const noexcept { ++clone_calls; return CloneOnly(v); }

    bool operator==(const CloneOnly& o) const noexcept { return v == o.v; }
};


// Tracks constructions/destructions to catch leaks and double-destroys.
struct LifetimeTracker {
    static inline I32 constructions = 0;
    static inline I32 destructions  = 0;

    I32 payload;

    explicit LifetimeTracker(I32 p = 0) noexcept : payload(p) { ++constructions; }
    LifetimeTracker(const LifetimeTracker& o) noexcept : payload(o.payload) { ++constructions; }
    LifetimeTracker(LifetimeTracker&& o) noexcept : payload(o.payload) { ++constructions; o.payload = -1; }
    LifetimeTracker& operator=(LifetimeTracker&& o) noexcept { payload = o.payload; o.payload = -1; return *this; }
    LifetimeTracker& operator=(const LifetimeTracker& o) noexcept = default;
    ~LifetimeTracker() noexcept { ++destructions; }

    bool operator==(const LifetimeTracker& o) const noexcept { return payload == o.payload; }

    static void reset() noexcept { constructions = 0; destructions = 0; }
};


// Move-assignable but NOT move-constructible. Exercises the ABORT fallback
// in operator= when growth would require constructing a brand-new slot.
struct MoveAssignOnly {
    I32 v;

    explicit MoveAssignOnly(I32 x = 0) noexcept : v(x) {}
    MoveAssignOnly(const MoveAssignOnly&) = delete;
    MoveAssignOnly& operator=(const MoveAssignOnly&) = delete;
    MoveAssignOnly(MoveAssignOnly&&) = delete;
    MoveAssignOnly& operator=(MoveAssignOnly&& o) noexcept { v = o.v; o.v = -1; return *this; }
};


// ------------------------- Construction -------------------------


TEST(StaticVecConstruction, DefaultIsEmpty) {
    let v = StaticVec<I32, 4>{};
    EXPECT_EQ(v.length(), 0);
    EXPECT_EQ(v.capacity(), 4);
}


TEST(StaticVecConstruction, VariadicSetsLengthAndElementsInOrder) {
    let v = StaticVec<I32, 4>(10, 20, 30);
    EXPECT_EQ(v.length(), 3);
    EXPECT_EQ(v[0], 10);
    EXPECT_EQ(v[1], 20);
    EXPECT_EQ(v[2], 30);
}


TEST(StaticVecConstruction, VariadicWorksWithMoveOnlyType) {
    mut v = StaticVec<MoveOnlyStrict, 3>(MoveOnlyStrict(1), MoveOnlyStrict(2));
    EXPECT_EQ(v.length(), 2);
    EXPECT_EQ(v[0].v, 1);
    EXPECT_EQ(v[1].v, 2);
}


TEST(StaticVecConstruction, VariadicAllowsFewerArgsThanCapacity) {
    let v = StaticVec<I32, 10>(1, 2);
    EXPECT_EQ(v.length(), 2);
    EXPECT_EQ(v.capacity(), 10);
}


TEST(StaticVecConstruction, VariadicAtExactCapacityWorks) {
    let v = StaticVec<I32, 3>(1, 2, 3);
    EXPECT_EQ(v.length(), 3);
    EXPECT_EQ(v.capacity(), 3);
}


// ------------------------- push / try_push -------------------------


TEST(StaticVecPush, PushIncreasesLengthAndStoresValue) {
    mut v = StaticVec<I32, 4>{};
    v.push(7);
    EXPECT_EQ(v.length(), 1);
    EXPECT_EQ(v[0], 7);
}


TEST(StaticVecPush, PushMovesRvalueForMoveOnlyType) {
    mut v = StaticVec<MoveOnlyStrict, 2>{};
    v.push(MoveOnlyStrict(5));
    EXPECT_EQ(v[0].v, 5);
}


TEST(StaticVecPush, PushClonesForClonableType) {
    CloneOnly::clone_calls = 0;
    mut v = StaticVec<CloneOnly, 2>{};
    let source = CloneOnly(9);

    v.push(source);

    EXPECT_EQ(CloneOnly::clone_calls, 1);
    EXPECT_EQ(v[0].v, 9);
}


TEST(StaticVecPush, PushBeyondCapacityAborts) {
    mut v = StaticVec<I32, 2>(1, 2);
    ASSERT_DEATH({ v.push(3); }, "");
}


TEST(StaticVecTryPush, SucceedsBelowCapacity) {
    mut v = StaticVec<I32, 2>{};
    EXPECT_TRUE(v.try_push(1));
    EXPECT_EQ(v.length(), 1);
}


TEST(StaticVecTryPush, FailsAtCapacityWithoutAborting) {
    mut v = StaticVec<I32, 2>(1, 2);
    EXPECT_FALSE(v.try_push(3));
    EXPECT_EQ(v.length(), 2);
}


TEST(StaticVecTryPush, MoveOnlyRvalueIsActuallyMoved) {
    // Regression test: try_push(T&&) must forward with do_move, not silently
    // fall back to the copy overload.
    mut v = StaticVec<MoveOnlyStrict, 2>{};
    mut src = MoveOnlyStrict(3);

    EXPECT_TRUE(v.try_push(do_move(src)));

    EXPECT_EQ(v[0].v, 3);
    EXPECT_EQ(src.v, -1); // moved-from sentinel; would still be 3 if silently copied
}


// ------------------------- pop / try_pop -------------------------


TEST(StaticVecPop, PopReturnsLastPushedValueLifoOrder) {
    mut v = StaticVec<I32, 4>(1, 2, 3);

    EXPECT_EQ(v.pop(), 3);
    EXPECT_EQ(v.pop(), 2);
    EXPECT_EQ(v.length(), 1);
}


TEST(StaticVecPop, PopOnEmptyAborts) {
    mut v = StaticVec<I32, 2>{};
    ASSERT_DEATH({ (Void)v.pop(); }, "");
}


TEST(StaticVecPop, PopDoesNotLeakOrDoubleDestroy) {
    LifetimeTracker::reset();
    {
        mut v = StaticVec<LifetimeTracker, 3>(LifetimeTracker(1), LifetimeTracker(2));
        {
            let popped = v.pop();
            EXPECT_EQ(popped.payload, 2);
        }
        EXPECT_EQ(v.length(), 1);
    }
    EXPECT_EQ(LifetimeTracker::constructions, LifetimeTracker::destructions);
}


TEST(StaticVecTryPop, ReturnsSomeWhenNotEmpty) {
    mut v = StaticVec<I32, 2>(1, 2);
    let popped = v.try_pop();
    ASSERT_TRUE(popped.is_some());
    EXPECT_EQ(do_move(popped).value_or_abort(), 2);
}


TEST(StaticVecTryPop, ReturnsNoneWhenEmptyWithoutAborting) {
    mut v = StaticVec<I32, 2>{};
    let popped = v.try_pop();
    EXPECT_TRUE(popped.is_none());
}


// ------------------------- operator[] -------------------------


TEST(StaticVecIndexOperator, MutableReferenceAllowsWrite) {
    mut v = StaticVec<I32, 3>(1, 2, 3);
    v[1] = 99;
    EXPECT_EQ(v[1], 99);
}


TEST(StaticVecIndexOperator, ConstOverloadReturnsReadOnlyReference) {
    let v = StaticVec<I32, 3>(1, 2, 3);
    let& r = v[2];
    EXPECT_EQ(r, 3);
}


TEST(StaticVecIndexOperator, IndexAtOrPastLengthAbortsEvenWithinCapacity) {
    // Regression test: bounds must be checked against _length, not CAP.
    // A StaticVec<I32,10> with only 3 elements must abort on index 5,
    // even though 5 < capacity().
    mut v = StaticVec<I32, 10>(1, 2, 3);
    ASSERT_DEATH({ (Void)v[5]; }, "");
}


TEST(StaticVecIndexOperator, IndexAtLengthBoundaryAborts) {
    mut v = StaticVec<I32, 10>(1, 2, 3);
    ASSERT_DEATH({ (Void)v[3]; }, "");
}


// ------------------------- at_clone -------------------------


TEST(StaticVecAtClone, CopyableTypeReturnsIndependentCopy) {
    mut v = StaticVec<PlainPoint, 3>(PlainPoint(1, 2), PlainPoint(3, 4));
    let p = v.at_clone(1);
    EXPECT_EQ(p, PlainPoint(3, 4));
}


TEST(StaticVecAtClone, ClonableTypeInvokesClone) {
    CloneOnly::clone_calls = 0;
    mut v = StaticVec<CloneOnly, 2>(CloneOnly(7));
    let c = v.at_clone(0);
    EXPECT_EQ(CloneOnly::clone_calls, 1);
    EXPECT_EQ(c.v, 7);
}


TEST(StaticVecAtClone, AtOrPastLengthAborts) {
    mut v = StaticVec<PlainPoint, 5>(PlainPoint(1, 1));
    ASSERT_DEATH({ (Void)v.at_clone(1); }, "");
}


// ------------------------- get_ref -------------------------


TEST(StaticVecGetRef, InRangeReturnsSomeAndAllowsMutation) {
    mut v = StaticVec<I32, 3>(1, 2, 3);
    mut ref = v.get_ref(1);
    ASSERT_TRUE(ref.is_some());
    ref.value_or_abort() = 42;
    EXPECT_EQ(v[1], 42);
}


TEST(StaticVecGetRef, AtOrPastLengthReturnsNoneEvenWithinCapacity) {
    mut v = StaticVec<I32, 10>(1, 2, 3);
    let ref = v.get_ref(3);
    EXPECT_TRUE(ref.is_none());
}


TEST(StaticVecGetRef, ConstInRangeReturnsSome) {
    let v = StaticVec<I32, 3>(1, 2, 3);
    let ref = v.get_ref(0);
    ASSERT_TRUE(ref.is_some());
    EXPECT_EQ(do_move(ref).value_or_abort(), 1);
}


// ------------------------- get_clone -------------------------


TEST(StaticVecGetClone, CopyableInRangeReturnsSomeWithCopy) {
    let v = StaticVec<PlainPoint, 2>(PlainPoint(5, 6));
    let opt = v.get_clone(0);
    ASSERT_TRUE(opt.is_some());
    EXPECT_EQ(do_move(opt).value_or_abort(), PlainPoint(5, 6));
}


TEST(StaticVecGetClone, AtOrPastLengthReturnsNone) {
    let v = StaticVec<PlainPoint, 5>(PlainPoint(1, 1));
    let opt = v.get_clone(1);
    EXPECT_TRUE(opt.is_none());
}


// ------------------------- clone() -------------------------


TEST(StaticVecClone, ProducesElementwiseIndependentCopyForCopyable) {
    let v = StaticVec<PlainPoint, 4>(PlainPoint(1, 1), PlainPoint(2, 2));

    let c = v.clone();
    EXPECT_EQ(c.length(), 2);
    EXPECT_EQ(c[0], PlainPoint(1, 1));
    EXPECT_EQ(c[1], PlainPoint(2, 2));
}


TEST(StaticVecClone, CallsCloneForEveryLiveElementOnly) {
    CloneOnly::clone_calls = 0;
    mut v = StaticVec<CloneOnly, 5>(CloneOnly(1), CloneOnly(2), CloneOnly(3));

    let c = v.clone();

    EXPECT_EQ(CloneOnly::clone_calls, 3); // not capacity() = 5
    EXPECT_EQ(c.length(), 3);
}


TEST(StaticVecClone, DoesNotLeakOrDoubleDestroy) {
    LifetimeTracker::reset();
    {
        mut v = StaticVec<LifetimeTracker, 4>(LifetimeTracker(1), LifetimeTracker(2));
        {
            let c = v.clone();
            EXPECT_EQ(c[0].payload, 1);
        }
    }
    EXPECT_EQ(LifetimeTracker::constructions, LifetimeTracker::destructions);
}


// ------------------------- Move construction -------------------------


TEST(StaticVecMoveConstruct, TrivialPathCopiesValuesAndLength) {
    mut a = StaticVec<PlainPoint, 4>(PlainPoint(1, 1), PlainPoint(2, 2));
    let b = do_move(a);

    EXPECT_EQ(b.length(), 2);
    EXPECT_EQ(b[0], PlainPoint(1, 1));
    EXPECT_EQ(b[1], PlainPoint(2, 2));
}


TEST(StaticVecMoveConstruct, NonTrivialPathMovesAndLeavesSourceEmpty) {
    mut a = StaticVec<MoveOnlyStrict, 3>(MoveOnlyStrict(1), MoveOnlyStrict(2));
    let b = do_move(a);

    EXPECT_EQ(b.length(), 2);
    EXPECT_EQ(b[0].v, 1);
    EXPECT_EQ(b[1].v, 2);
    EXPECT_EQ(a.length(), 0);
}


TEST(StaticVecMoveConstruct, FromEmptySourceProducesEmptyResult) {
    mut a = StaticVec<I32, 3>{};
    let b = do_move(a);
    EXPECT_EQ(b.length(), 0);
}


TEST(StaticVecMoveConstruct, DoesNotLeakOrDoubleDestroy) {
    LifetimeTracker::reset();
    {
        mut a = StaticVec<LifetimeTracker, 3>(LifetimeTracker(1), LifetimeTracker(2), LifetimeTracker(3));
        let b = do_move(a);
        EXPECT_EQ(b.length(), 3);
    }
    EXPECT_EQ(LifetimeTracker::constructions, LifetimeTracker::destructions);
}


// ------------------------- Move assignment -------------------------


TEST(StaticVecMoveAssign, SelfAssignmentIsNoOp) {
    mut a = StaticVec<I32, 3>(1, 2);
    a = do_move(a);
    EXPECT_EQ(a.length(), 2);
    EXPECT_EQ(a[0], 1);
}


TEST(StaticVecMoveAssign, AssigningEmptyClearsTarget) {
    mut a = StaticVec<I32, 3>(1, 2, 3);
    mut b = StaticVec<I32, 3>{};
    a = do_move(b);
    EXPECT_EQ(a.length(), 0);
}


TEST(StaticVecMoveAssign, EqualLengthOverwritesElementwise) {
    mut a = StaticVec<I32, 4>(1, 2);
    mut b = StaticVec<I32, 4>(10, 20);
    a = do_move(b);
    EXPECT_EQ(a.length(), 2);
    EXPECT_EQ(a[0], 10);
    EXPECT_EQ(a[1], 20);
}


TEST(StaticVecMoveAssign, GrowingCopiesAssignedPrefixAndConstructsTail) {
    mut a = StaticVec<I32, 5>(1, 2);
    mut b = StaticVec<I32, 5>(10, 20, 30, 40);
    a = do_move(b);
    EXPECT_EQ(a.length(), 4);
    EXPECT_EQ(a[0], 10);
    EXPECT_EQ(a[1], 20);
    EXPECT_EQ(a[2], 30);
    EXPECT_EQ(a[3], 40);
}


TEST(StaticVecMoveAssign, ShrinkingOverwritesPrefixAndDestroysTail) {
    mut a = StaticVec<I32, 5>(1, 2, 3, 4);
    mut b = StaticVec<I32, 5>(10, 20);
    a = do_move(b);
    EXPECT_EQ(a.length(), 2);
    EXPECT_EQ(a[0], 10);
    EXPECT_EQ(a[1], 20);
}


TEST(StaticVecMoveAssign, TrivialPathWorksForPlainType) {
    mut a = StaticVec<PlainPoint, 4>(PlainPoint(0, 0));
    mut b = StaticVec<PlainPoint, 4>(PlainPoint(1, 1), PlainPoint(2, 2));
    a = do_move(b);
    EXPECT_EQ(a.length(), 2);
    EXPECT_EQ(a[0], PlainPoint(1, 1));
}


TEST(StaticVecMoveAssign, GrowingWithoutMoveConstructibleTypeAborts) {
    // T is move-assignable but NOT move-constructible: growing needs to
    // construct brand-new slots, which is impossible, so this must abort
    // rather than silently doing nothing or corrupting state.
    mut small = StaticVec<MoveAssignOnly, 5>(1, 2);
    mut large = StaticVec<MoveAssignOnly, 5>(10, 20, 30);

    ASSERT_DEATH({ small = do_move(large); }, "");
}


TEST(StaticVecMoveAssign, ShrinkingWithoutMoveConstructibleTypeSucceeds) {
    // Shrinking never needs to construct a new slot, so it must work fine
    // even when T isn't move-constructible.
    mut large = StaticVec<MoveAssignOnly, 5>(1, 2, 3);
    mut small = StaticVec<MoveAssignOnly, 5>(10, 20);

    large = do_move(small);

    EXPECT_EQ(large.length(), 2);
    EXPECT_EQ(large[0].v, 10);
    EXPECT_EQ(large[1].v, 20);
}


TEST(StaticVecMoveAssign, DoesNotLeakSourceOnEqualLength) {
    LifetimeTracker::reset();
    {
        mut a = StaticVec<LifetimeTracker, 3>(LifetimeTracker(1), LifetimeTracker(2));
        {
            mut b = StaticVec<LifetimeTracker, 3>(LifetimeTracker(10), LifetimeTracker(20));
            a = do_move(b);
        }
    }
    EXPECT_EQ(LifetimeTracker::constructions, LifetimeTracker::destructions);
}


TEST(StaticVecMoveAssign, DoesNotLeakSourceWhenGrowing) {
    LifetimeTracker::reset();
    {
        mut a = StaticVec<LifetimeTracker, 5>(LifetimeTracker(1));
        {
            mut b = StaticVec<LifetimeTracker, 5>(LifetimeTracker(10), LifetimeTracker(20), LifetimeTracker(30));
            a = do_move(b);
        }
    }
    EXPECT_EQ(LifetimeTracker::constructions, LifetimeTracker::destructions);
}


TEST(StaticVecMoveAssign, DoesNotLeakSourceWhenShrinking) {
    LifetimeTracker::reset();
    {
        mut a = StaticVec<LifetimeTracker, 5>(LifetimeTracker(1), LifetimeTracker(2), LifetimeTracker(3));
        {
            mut b = StaticVec<LifetimeTracker, 5>(LifetimeTracker(10));
            a = do_move(b);
        }
    }
    EXPECT_EQ(LifetimeTracker::constructions, LifetimeTracker::destructions);
}


// ------------------------- Destructor -------------------------


TEST(StaticVecDestructor, DestroysExactlyLiveElementsNotCapacity) {
    LifetimeTracker::reset();
    {
        mut v = StaticVec<LifetimeTracker, 10>(LifetimeTracker(1), LifetimeTracker(2));
        EXPECT_EQ(v.length(), 2);
    }
    EXPECT_EQ(LifetimeTracker::constructions, 2);
    EXPECT_EQ(LifetimeTracker::destructions, 2);
}


TEST(StaticVecDestructor, EmptyVecDestroysNothing) {
    LifetimeTracker::reset();
    {
        let v = StaticVec<LifetimeTracker, 4>{};
        (Void)v;
    }
    EXPECT_EQ(LifetimeTracker::destructions, 0);
}


// ------------------------- Iterators -------------------------


TEST(StaticVecIterator, RangeBasedForVisitsOnlyLiveElements) {
    let v = StaticVec<I32, 10>(1, 2, 3);

    mut sum = 0;
    mut count = 0;
    for (let& x : v) {
        sum += x;
        ++count;
    }

    EXPECT_EQ(count, 3);   // not capacity() = 10
    EXPECT_EQ(sum, 6);
}


TEST(StaticVecIterator, MutableIterationAllowsWrite) {
    mut v = StaticVec<I32, 4>(1, 2, 3);
    for (mut& x : v) x *= 10;
    EXPECT_EQ(v[0], 10);
    EXPECT_EQ(v[2], 30);
}


TEST(StaticVecIterator, BeginEqualsEndOnEmptyVec) {
    let v = StaticVec<I32, 4>{};
    EXPECT_TRUE(v.begin() == v.end());
}


TEST(StaticVecIterator, BeginNotEqualEndWhenNonEmpty) {
    let v = StaticVec<I32, 4>(1);
    EXPECT_TRUE(v.begin() != v.end());
}


// ------------------------- data() -------------------------


TEST(StaticVecMisc, DataPointerMatchesFirstElementAddress) {
    mut v = StaticVec<I32, 3>(1, 2, 3);
    EXPECT_EQ(v.data(), &v[0]);
    EXPECT_EQ(*v.data(), 1);
}


TEST(StaticVecMisc, ConstDataAccessibleOnConstVec) {
    let v = StaticVec<I32, 3>(1, 2, 3);
    const I32* p = v.data();
    EXPECT_EQ(*p, 1);
}


TEST(StaticVecMisc, CapacityIsFixedAndIndependentOfLength) {
    mut v = StaticVec<I32, 7>{};
    EXPECT_EQ(v.capacity(), 7);
    v.push(1);
    EXPECT_EQ(v.capacity(), 7);
}


// ------------------------- Copy policy -------------------------


TEST(StaticVecCopyPolicy, IsNotImplicitlyCopyable) {
    // static_assert(!CopyConstructible<StaticVec<I32, 3>>,
    //     "StaticVec must not be implicitly copy-constructible (NoDefaultCopy).");

    static_assert(!CopyAssignable<StaticVec<I32, 3>>,
        "StaticVec must not be implicitly copy-assignable (NoDefaultCopy).");
    SUCCEED();
}