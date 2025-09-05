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

    let nested_some = Option<Option<I32>>::some(do_move(prim_none));
    ASSERT_TRUE(nested_some.is_some());

    mut prim_some = Option<I32>::some(10);
    ASSERT_TRUE(prim_some.is_some());

    let nested_none = Option<Option<I32>>::none();
    ASSERT_TRUE(nested_none.is_none());
}


TEST(types_option, unwrap_or_default) {
    mut prim = Option<I32>::some(19);
    mut nested = Option<Option<I32>>::some(do_move(prim));
    ASSERT_TRUE(nested.is_some());

    mut nested_val = do_move(nested).value_or_default();
    ASSERT_TRUE(nested_val.is_some());

    let prim_val = do_move(nested_val).value_or_default();
    ASSERT_EQ(prim_val, 19);
}


TEST(types_option, take) {
    mut prim = Option<I32>::some(19);
    mut nested = Option<Option<I32>>::some(do_move(prim));
    ASSERT_TRUE(nested.is_some());

    mut nested_val = nested.take();
    ASSERT_TRUE(nested.is_none());

    let prim_val = nested_val.take();
    ASSERT_EQ(prim_val, 19);
    ASSERT_TRUE(nested_val.is_none());
}


TEST(types_option, as_ref) {
    mut prim = Option<I32>::some(19);
    let nested = Option<Option<I32>>::some(do_move(prim));
    ASSERT_TRUE(nested.is_some());

    let& nested_ref = nested.as_ref();
    ASSERT_TRUE(nested_ref.is_some());

    let& prim_ref = nested_ref.as_ref();
    ASSERT_EQ(prim_ref, 19);
}


TEST(types_option, as_mut) {
    mut prim = Option<I32>::some(19);
    mut nested = Option<Option<I32>>::some(do_move(prim));
    ASSERT_TRUE(nested.is_some());

    mut& nested_ref = nested.as_mut();
    ASSERT_TRUE(nested_ref.is_some());

    mut& prim_ref = nested_ref.as_mut();
    ASSERT_EQ(prim_ref, 19);

    prim_ref = 15;

    ASSERT_EQ(nested_ref.as_ref(), 15);
}


TEST(types_option, clone) {
    let prim = Option<I32>::some(19);
    let nested = Option<Option<I32>>::some(do_move(prim.clone()));
    ASSERT_TRUE(nested.is_some());

    let nested_cloned = nested.clone();
    ASSERT_TRUE(nested_cloned.is_some());

    let prim_cloned = nested_cloned.as_ref().clone();
    ASSERT_TRUE(prim_cloned.is_some());
    ASSERT_EQ(prim_cloned.as_ref(), 19);
    ASSERT_EQ(prim.as_ref(), 19);
}


TEST(types_option, macro) {
    mut prim_none = none_(I32);
    ASSERT_TRUE(prim_none.is_none());

    let nested_some = some_(do_move(prim_none), Option<I32>);
    ASSERT_TRUE(nested_some.is_some());

    let prim_some = Option<I32>::some(10);
    ASSERT_TRUE(prim_some.is_some());

    let nested_none = Option<Option<I32>>::none();
    ASSERT_TRUE(nested_none.is_none());
}


TEST(types_optionref, create) {
    let prim_const = Option<I32>::some(123);
    mut nested_mut = Option<Option<I32>>::none();

    let prim_const_ref = OptionRef<const I32>::some(prim_const.as_ref());
    ASSERT_TRUE(prim_const_ref.is_some());

    let nested_mut_ref = OptionRef<Option<I32>>::some(nested_mut.as_mut());
    ASSERT_TRUE(nested_mut_ref.is_some());

    let option_ref_none = OptionRef<I32>::none();
    ASSERT_TRUE(option_ref_none.is_none());
}


TEST(types_optionref, value_or) {
    let prim_const = Option<I32>::some(123);
    mut prime_mut = Option<U64>::some(4568731290);

    let prim_const_ref = OptionRef<const I32>::some(prim_const.as_ref());
    ASSERT_TRUE(prim_const_ref.is_some());
    ASSERT_EQ(prim_const_ref.value_or(0), 123);

    // Invalid case:
    // mut& prime_const_ref_val = prim_const_ref.value_or(0);
    // prime_const_ref_val = 5; // ERROR: assigning to the const reference

    let prim_mut_ref = OptionRef<U64>::some(prime_mut.as_mut());
    ASSERT_TRUE(prim_mut_ref.is_some());

    mut& prim_mut_ref_val = prim_mut_ref.value_or(0);
    prim_mut_ref_val = 123456789;

    let prim_mut_val = prime_mut.as_ref();
    ASSERT_EQ(prim_mut_val, 123456789);
}