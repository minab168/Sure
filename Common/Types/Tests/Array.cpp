/*
 * ──────────────────────────────────────────────
 * Project   : Sure
 * File      : Option
 * Author    : Emad Redwan
 * Created   : 9/4/25
 * License   : MIT
 * ──────────────────────────────────────────────
 */


#include <cstdio>

#include "../Array.hpp"

#include "../Inc.hpp"
#include "../../Ownership/Move.hpp"
// #include "../../../Contribs/googletest/googletest/include/gtest/gtest.h"

// #include <new>

int main() {
    // let a = sure::Array<Option<I32>, 5>{};
    let a = sure::Array<I32, 5>{1, 2, 3, 4, 5};
    let b = a.clone();
    printf("HELLO!\n");
}
