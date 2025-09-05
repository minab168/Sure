/*
 * ──────────────────────────────────────────────
 * Project   : Sure
 * File      : Variant
 * Author    : Emad Redwan
 * Created   : 9/4/25
 * License   : MIT
 * ──────────────────────────────────────────────
 */


#pragma once


#include <variant>


template<typename ...T>
using Variant = std::variant<T...>;