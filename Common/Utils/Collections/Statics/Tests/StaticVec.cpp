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

#include "../StaticVec.hpp"


using namespace sure::collections;

TEST(StaticVec, General) {

    StaticVec<Option<U32>, 3> vec;

    let opt = some_ty_(5, U32);
    vec.push(do_move(opt));

    let val = vec.pop();
    let d = do_move(val).value_or_abort();
    let pop = vec.try_pop();

    vec.push(opt);
    vec.push(opt);
    vec.push(opt);
    let a = vec.try_push(opt);

    SUCCEED();
}