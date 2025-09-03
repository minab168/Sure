#pragma once


template<typename T>
concept From = requires(T t) {
    { T::From(t) };
};