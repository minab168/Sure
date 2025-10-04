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


template<typename T>
concept ToString = requires(T t) {
    { t.to_string() };
};