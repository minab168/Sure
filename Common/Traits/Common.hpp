#pragma once
#include "Primitives.hpp"


template<typename T>
concept Clonable = requires(T t) {
    { t.clone() };
};


template<typename T>
concept Printable = Primitive<T> or requires(T t) {
    { t.print() };
};