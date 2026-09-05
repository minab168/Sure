/*
 * ──────────────────────────────────────────────
 * Project   : Sure
 * File      : Option
 * Author    : Emad Redwan
 * Created   : 9/4/25
 * License   : MIT
 * ──────────────────────────────────────────────
 */


#include <gtest/gtest.h>
#include "../Inc.hpp"
// #include "../../Ownership/Move.hpp"
//
//
// // ------------------------- Helper types -------------------------
//
//
// // Move-only, move-assignable, noexcept move ops
// struct MoveOnlyNoThrow {
//     I32 v;
//
//     explicit MoveOnlyNoThrow(I32 x = 0) noexcept : v(x) {}
//     MoveOnlyNoThrow(const MoveOnlyNoThrow&) = delete;
//     MoveOnlyNoThrow& operator=(const MoveOnlyNoThrow&) = delete;
//
//     MoveOnlyNoThrow(MoveOnlyNoThrow&& o) noexcept : v(o.v) { o.v = -1; }
//     MoveOnlyNoThrow& operator=(MoveOnlyNoThrow&& o) noexcept {
//         if (this != &o) { v = o.v; o.v = -1; }
//         return *this;
//     }
//
//     bool operator==(const MoveOnlyNoThrow& o) const noexcept { return v == o.v; }
//
//     ~MoveOnlyNoThrow() noexcept = default;
// };
//
//
// // Move-only, NOT move-assignable (to exercise operator= fallback)
// struct MoveOnlyNoAssign {
//     I32 v;
//
//     explicit MoveOnlyNoAssign(I32 x = 0) noexcept : v(x) {}
//     MoveOnlyNoAssign(const MoveOnlyNoAssign&) = delete;
//     MoveOnlyNoAssign& operator=(const MoveOnlyNoAssign&) = delete;
//
//     MoveOnlyNoAssign(MoveOnlyNoAssign&& o) noexcept : v(o.v) { o.v = -1; }
//     MoveOnlyNoAssign& operator=(MoveOnlyNoAssign&&) = delete; // no move-assign
//     bool operator==(const MoveOnlyNoAssign& o) const noexcept { return v == o.v; }
// };
//
//
// // Lifetime tracker to confirm destructor calls
// struct LifetimeTracker {
//     static inline I32 constructions = 0;
//     static inline I32 destructions = 0;
//
//     I32 payload;
//     LifetimeTracker() noexcept : payload(0) { ++constructions; }
//     explicit LifetimeTracker(I32 p) noexcept : payload(p) { ++constructions; }
//     LifetimeTracker(const LifetimeTracker&) noexcept : payload(0) { ++constructions; }
//     LifetimeTracker(LifetimeTracker&& o) noexcept : payload(o.payload) { ++constructions; o.payload = -1; }
//     LifetimeTracker& operator=(LifetimeTracker&& o) noexcept { payload = o.payload; o.payload = -1; return *this; }
//     ~LifetimeTracker() noexcept { ++destructions; }
//     bool operator==(const LifetimeTracker& o) const noexcept { return payload == o.payload; }
// };
//
//
// // A type that is move-assignable but its move-assign may throw (we will NOT instantiate Option with it;
// // only used to reason about constraints if desired)
// struct ThrowingMoveAssign {
//     I32 v;
//     explicit ThrowingMoveAssign(I32 x=0):v(x){}
//     ThrowingMoveAssign(ThrowingMoveAssign&& o) noexcept : v(o.v) { o.v = -1; }
//     ThrowingMoveAssign& operator=(ThrowingMoveAssign&& o) { // potentially throwing
//         if (this != &o) {
//             // simulate possible throw: but we won't actually throw in tests
//             v = o.v;
//             o.v = -1;
//         }
//         return *this;
//     }
// };
//
//
// // ------------------------- Tests: Option basic behaviors -------------------------
//
//
// TEST(OptionBasic, SomeNoneQueries) {
//     let s = Option<I32>::some(42);
//     let n = Option<I32>::none();
//
//     EXPECT_TRUE(s.is_some());
//     EXPECT_FALSE(s.is_none());
//     EXPECT_TRUE(n.is_none());
//     EXPECT_FALSE(n.is_some());
// }
//
//
// TEST(OptionBasic, ValueOrAbortOnSome) {
//     let s = Option<I32>::some(42);
//
//     let val = do_move(s).value_or_abort();
//     EXPECT_EQ(val, 42);
// }
//
//
// TEST(OptionBasic, ValueOrAbortOnNoneDies) {
//     let n = Option<I32>::none();
//
//     ASSERT_DEATH({ do_move(n).value_or_abort(); }, "No value is provided");
// }
//
//
// TEST(OptionBasic, TakeOnSomeReturnsOptionAndLeaveSourceNone) {
//     mut s = Option<MoveOnlyNoThrow>::some(MoveOnlyNoThrow(99));
//
//     let out = s.take();
//     EXPECT_TRUE(out.is_some());
//     EXPECT_TRUE(s.is_none());
//
//     let val = do_move(out).value_or_abort();
//     EXPECT_EQ(val.v, 99);
// }
//
//
// TEST(OptionBasic, TakeOnNoneReturesNone) {
//     mut n = Option<MoveOnlyNoThrow>::none();
//
//     let out = n.take();
//     EXPECT_TRUE(out.is_none());
//     EXPECT_TRUE(n.is_none());
// }
//
//
// TEST(OptionMove, MoveCtorLeavesSourceNone) {
//     let a = Option<MoveOnlyNoThrow>::some(MoveOnlyNoThrow(7));
//     let b = do_move(a);
//
//     EXPECT_TRUE(a.is_none());
//     EXPECT_TRUE(b.is_some());
//
//     let v = do_move(b).value_or_abort();
//     EXPECT_EQ(v.v, 7);
// }
//
//
// TEST(OptionMove, MoveAssignLeavesSourceNone) {
//     let a = Option<MoveOnlyNoThrow>::some(MoveOnlyNoThrow(9));
//     mut b = Option<MoveOnlyNoThrow>::none();
//
//     b = do_move(a);
//
//     EXPECT_TRUE(a.is_none());
//     EXPECT_TRUE(b.is_some());
//
//     let v = do_move(b).value_or_abort();
//     EXPECT_EQ(v.v, 9);
// }
//
//
// TEST(OptionMove, MoveAssignBothHaveValue) {
//     let a = Option<MoveOnlyNoThrow>::some(MoveOnlyNoThrow(11));
//     mut b = Option<MoveOnlyNoThrow>::some(MoveOnlyNoThrow(13));
//
//     b = do_move(a);
//
//     EXPECT_TRUE(a.is_none());
//     EXPECT_TRUE(b.is_some());
//
//     let v = do_move(b).value_or_abort();
//     EXPECT_EQ(v.v, 11);
// }
//
//
// TEST(OptionMove, MoveAssignReconstructWhenNotMoveAssignable) {
//     let a = Option<MoveOnlyNoAssign>::some(MoveOnlyNoAssign(15));
//     mut b = Option<MoveOnlyNoAssign>::some(MoveOnlyNoAssign(17));
//
//     b = do_move(a);
//
//     EXPECT_TRUE(a.is_none());
//     EXPECT_TRUE(b.is_some());
//
//     let v = do_move(b).value_or_abort();
//     EXPECT_EQ(v.v, 15);
// }
//
//
// TEST(OptionMove, SelfMoveAssignmentIsNothing) {
//     mut a = Option<MoveOnlyNoThrow>::some(MoveOnlyNoThrow(21));
//
//     a = do_move(a);
//
//     EXPECT_TRUE(a.is_some());
//     let v = do_move(a).value_or_abort();
//     EXPECT_EQ(v.v, 21);
// }
//
//
// TEST(OptionRetRef, AsRefAndAsMut) {
//     let o = Option<MoveOnlyNoThrow>::some(MoveOnlyNoThrow(4));
//
//     let r_const = o.as_ref();
//
//     let& c_ref = do_move(r_const).value_or_abort();
//     EXPECT_EQ(c_ref.v, 4);
//
//     mut o2 = Option<MoveOnlyNoThrow>::some(MoveOnlyNoThrow(6));
//     mut r_mut = o2.as_mut();
//
//     mut& m_ref = do_move(r_mut).value_or_abort();
//     m_ref.v = 1234;
//
//     let v = do_move(o2).value_or_abort();
//     EXPECT_EQ(v.v, 1234);
// }
//
//
// TEST(OptionRetRef, AsRefNoneAndAsMutNone) {
//     mut o = Option<MoveOnlyNoThrow>::none();
//
//     let r_const = o.as_ref();
//     let r_mut = o.as_mut();
//
//     EXPECT_TRUE(r_const.is_none());
//     EXPECT_TRUE(r_mut.is_none());
// }
//
//
// TEST(OptionRefBasic, SomeNoneAndValueAccess) {
//     mut x = 5;
//     let r = OptionRef<I32>::some(x);
//     ASSERT_TRUE(r.is_some());
//
//     mut& ref = do_move(r).value_or_abort();
//     EXPECT_EQ(ref, 5);
//
//     ref = 99;
//     EXPECT_EQ(x, 99);
// }
//
//
// TEST(OptionRefBasic, MoveCtorClearSource) {
//     mut x = 3;
//     let a = OptionRef<I32>::some(x);
//     let b = do_move(a);
//     EXPECT_TRUE(a.is_none());
//     EXPECT_TRUE(b.is_some());
//
//     EXPECT_EQ(do_move(b).value_or_abort(), 3);
// }
//
//
// TEST(OptionRefBasic, MoveAssignClearSource) {
//     mut x = 1;
//     let a = OptionRef<I32>::some(x);
//     mut b = OptionRef<I32>::none();
//
//     b = do_move(a);
//     EXPECT_TRUE(a.is_none());
//     EXPECT_TRUE(b.is_some());
//
//     do_move(b).value_or_abort() = 7;
//     EXPECT_EQ(x, 7);
// }
//
//
// TEST(OptionRefBasic, ValueOrAbortOnNoneDies) {
//     let n = OptionRef<I32>::none();
//     ASSERT_DEATH({ do_move(n).value_or_abort(); }, "No value is provided");
// }
//
//
// TEST(OptionLifetime, ConstructionsDestructionsMatch) {
//     LifetimeTracker::constructions = 0;
//     LifetimeTracker::destructions = 0;
//
//     {
//         let o = Option<LifetimeTracker>::some(LifetimeTracker(42));
//         EXPECT_TRUE(o.is_some());
//     }
//
//     EXPECT_EQ(LifetimeTracker::constructions, LifetimeTracker::destructions);
// }
//
//
// TEST(OptionEdge, ManyMovesAndTakes) {
//     mut a = Option<MoveOnlyNoThrow>::some(MoveOnlyNoThrow(1));
//     for (I32 i = 0; i < 10; ++i) {
//         let b = a.take();
//         EXPECT_TRUE(a.is_none());
//
//         if (b.is_some()) {
//             let v = do_move(b).value_or_abort();
//             (Void)v;
//         }
//
//         a = Option<MoveOnlyNoThrow>::some(MoveOnlyNoThrow(i + 2));
//     }
// }