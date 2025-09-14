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


TEST(types_option, create) {
    mut prim_none = Option<I32>::none();
    ASSERT_TRUE(prim_none.is_none());

    // let nested_some = Option<Option<I32>>::some(do_move(prim_none));
    // ASSERT_TRUE(nested_some.is_some());
    //
    // mut prim_some = Option<I32>::some(10);
    // ASSERT_TRUE(prim_some.is_some());
    //
    // let nested_none = Option<Option<I32>>::none();
    // ASSERT_TRUE(nested_none.is_none());
}