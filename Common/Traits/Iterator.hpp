#pragma once


#include "Primitives.hpp"


template<typename I>
concept Iterable = requires(I i) {
    { i.begin() };
    { i.end() };
};



template<typename C>
concept Container = Iterable<C> and requires(C c) {
    { c.len() } -> Integral;
    { c.is_empty() } -> Boolean;
};



template<typename T>
concept Resizable = requires(T t) {
    t.resize(0);
};



template<typename I>
concept ForwardIterable = requires(I i) {
    { ++i };
};



template<typename I>
concept BidirectionalIterable = ForwardIterable<I> and requires(I i) {
    { --i };
};



template<typename I>
concept RandomAccessIterable = BidirectionalIterable<I> and requires(I i, int n) {
    { i + n };
};