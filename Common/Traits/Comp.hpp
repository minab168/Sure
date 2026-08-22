#pragma once


#include "Primitives.hpp"


template<typename T>
concept EqComparable = requires(T t, T u) {
    { t == u } -> Boolean;
    { t != u } -> Boolean;
};


template<typename T>
concept Ord = requires(T t, T u) {
    { t < u }  -> Boolean;
    { t <= u } -> Boolean;
    { t > u }  -> Boolean;
    { t >= u } -> Boolean;
};