#pragma once


#include "Base.hpp"


template<typename T>
concept Addable = requires(T t, T u) {
    { t + u } -> SameAs<T>;
};


template<typename T>
concept Subtractable = requires(T t, T u) {
    { t - u } -> SameAs<T>;
};


template<typename T>
concept Multiplicate = requires(T t, T u) {
    { t * u } -> SameAs<T>;
};


template<typename T>
concept Divisible = requires(T t, T u) {
    { t / u } -> SameAs<T>;
};


template<typename T>
concept Modable = requires(T t, T u) {
    { t % u } -> SameAs<T>;
};


template<typename T>
concept Neqatable = requires(T t) {
    { -t } -> SameAs<T>;
};


template<typename T>
concept Dereferenceable = requires(T t) {
    { *t };
};


template<typename T>
concept Arithmetic = Addable<T> and Subtractable<T> and Multiplicate<T> and Divisible<T>;