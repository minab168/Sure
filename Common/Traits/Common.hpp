#pragma once

#include "Primitives.hpp"
#include "../Types/Primitives/Inc.hpp"


template<typename T>
concept Clonable = requires(const T& t) {
    { t.clone() } -> SameAs<T>;
};


template<typename T>
concept Printable = Primitive<T> or requires(T t) {
    { t.print() } -> SameAs<Void>;
};


template<typename T>
concept ToString = requires(T t) {
    { t.to_string() };
};