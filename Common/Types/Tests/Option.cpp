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
#include "../../Ownership/Move.hpp"



struct MovableOnly {
    I32 val;
    MovableOnly() noexcept = default;
    constexpr explicit MovableOnly(I32 v) noexcept: val(v) {}
    MovableOnly(MovableOnly&& obj) noexcept: val(obj.val) { obj.val = 0; }
    MovableOnly& operator=(MovableOnly&& obj) noexcept { val = obj.val; return *this; }
};


struct CopyableOnly {
    I32 val;
    CopyableOnly() noexcept = default;
    constexpr explicit CopyableOnly(I32 v) noexcept: val(v) {}
    CopyableOnly(const CopyableOnly& obj) noexcept = default;
    CopyableOnly& operator=(const CopyableOnly& obj) noexcept = default;
};


struct ClonableOnly {
    I32 val;
    ClonableOnly() noexcept = default;
    constexpr explicit ClonableOnly(I32 v) noexcept: val(v) {}
    NODISCARD_
    ClonableOnly clone() const noexcept { return ClonableOnly(val); }
};


TEST(OptionRefTest, DefaultIsNone) {
    mut x = 5;
    let opt = OptionRef<I32>::none();

    EXPECT_TRUE(opt.is_none());
    EXPECT_FALSE(opt.is_some());
}


TEST(OptionRefTest, DefaultIsSome) {
    mut x = 42;
    let opt = OptionRef<I32>::some(x);

    EXPECT_TRUE(opt.is_some());
    EXPECT_FALSE(opt.is_none());
    EXPECT_EQ(&do_move(opt).value_or_abort(), &x);
}


TEST(OptionRefTest, MoveConstructionClearsSource) {
    I32 x = 7;
    auto opt1 = OptionRef<I32>::some(x);
    auto opt2 = do_move(opt1);
    EXPECT_TRUE(opt2.is_some());
    EXPECT_TRUE(opt1.is_none());
}


TEST(OptionRefTest, MoveAssignment) {
    mut a = 1;
    mut b = 2;

    let opt1 = OptionRef<I32>::some(a);
    mut opt2 = OptionRef<I32>::some(b);

    opt2 = do_move(opt1);
    EXPECT_TRUE(opt2.is_some());
    EXPECT_EQ(&do_move(opt2).value_or_abort(), &a);
    EXPECT_TRUE(opt1.is_none());
}


TEST(OptionTest, DefaultIsNone) {
    Option<I32> opt;
    EXPECT_TRUE(opt.is_none());
    EXPECT_FALSE(opt.is_some());
}


TEST(OptionTest, SomeValue) {
    let opt = Option<I32>::some(99);
    EXPECT_TRUE(opt.is_some());
    EXPECT_FALSE(opt.is_none());
    EXPECT_EQ(do_move(opt).value_or_abort(), 99);
}


TEST(OptionTest, MoveConstruction) {
    Option<MovableOnly> opt1 = Option<MovableOnly>::some(MovableOnly{10});
    Option<MovableOnly> opt2 = do_move(opt1);
    EXPECT_TRUE(opt2.is_some());
    EXPECT_TRUE(opt1.is_none());
    EXPECT_EQ(do_move(opt2).value_or_abort().val, 10);
}


TEST(OptionTest, CloneCopyable) {
    Option<CopyableOnly> opt1 = Option<CopyableOnly>::some(CopyableOnly{5});
    auto opt2 = opt1._clone_impl();
    EXPECT_TRUE(opt1.is_some());
    EXPECT_TRUE(opt2.is_some());
    EXPECT_EQ(do_move(opt2).value_or_abort().val, 5);
}


TEST(OptionTest, CloneClonable) {
    Option<ClonableOnly> opt1 = Option<ClonableOnly>::some(ClonableOnly{20});
    auto opt2 = opt1._clone_impl();
    EXPECT_TRUE(opt1.is_some());
    EXPECT_TRUE(opt2.is_some());
    EXPECT_EQ(do_move(opt2).value_or_abort().val, 20);
}


TEST(OptionTest, MoveAssignmentWithValue) {
    Option<MovableOnly> opt1 = Option<MovableOnly>::some(MovableOnly{1});
    Option<MovableOnly> opt2 = Option<MovableOnly>::some(MovableOnly{2});
    opt2 = do_move(opt1);
    EXPECT_TRUE(opt2.is_some());
    EXPECT_EQ(do_move(opt2).value_or_abort().val, 1);
    EXPECT_TRUE(opt1.is_none());
}


TEST(OptionTest, MoveAssignmentNoneToSome) {
    Option<I32> opt1;
    Option<I32> opt2 = Option<I32>::some(42);
    opt2 = do_move(opt1);
    EXPECT_TRUE(opt2.is_none());
    EXPECT_TRUE(opt1.is_none());
}


TEST(OptionTest, TakeValue) {
    Option<I32> opt = Option<I32>::some(7);
    auto taken = opt.take();
    EXPECT_TRUE(taken.is_some());
    EXPECT_EQ(do_move(taken).value_or_abort(), 7);
    EXPECT_TRUE(opt.is_none());
}


TEST(OptionTest, AsRefAndAsMut) {
    Option<I32> opt = Option<I32>::some(55);
    auto ref = opt.as_ref();
    auto mu = opt.as_mut();
    EXPECT_TRUE(ref.is_some());
    EXPECT_TRUE(mu.is_some());
    EXPECT_EQ(ref.value_or_abort(), 55);
    EXPECT_EQ(mu.value_or_abort(), 55);
    *mu.value_or_abort() = 100;
    EXPECT_EQ(*opt.as_ref().value_or_abort(), 100);
}


TEST(OptionTest, ValueOrAbortNone) {
    Option<I32> opt;
    EXPECT_DEATH({ do_move(opt).value_or_abort(); }, "No value is provided");
}


TEST(OptionTest, MoveEmptyOption) {
    Option<I32> opt1;
    Option<I32> opt2 = do_move(opt1);
    EXPECT_TRUE(opt2.is_none());
    EXPECT_TRUE(opt1.is_none());
}

TEST(OptionTest, SelfMoveAssignment) {
    Option<I32> opt = Option<I32>::some(10);
    opt = do_move(opt);
    EXPECT_TRUE(opt.is_some());
    EXPECT_EQ(do_move(opt).value_or_abort(), 10);
}