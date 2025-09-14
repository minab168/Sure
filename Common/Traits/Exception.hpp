#pragma once


#include <type_traits>


template<typename T>
concept NoThrowDefaultConstructible = std::is_nothrow_default_constructible_v<T>;


template<typename T>
concept NoThrowCopyConstructible = std::is_nothrow_copy_constructible_v<T>;


template<typename T>
concept NoThrowMoveConstructible = std::is_nothrow_move_constructible_v<T>;


template<typename T>
concept NoThrowCopyAssignable = std::is_nothrow_copy_assignable_v<T>;


template<typename T>
concept NoThrowMoveAssignable = std::is_nothrow_move_assignable_v<T>;


template<typename T>
concept NoThrowDestructible = std::is_nothrow_destructible_v<T>;


template<typename T>
concept NoThrowCopyable = NoThrowCopyConstructible<T> and NoThrowCopyAssignable<T>;


template<typename T>
concept NoThrowMovable = NoThrowMoveConstructible<T> and NoThrowMoveAssignable<T>;


template<typename T>
concept NoThrowSemiregular = NoThrowDefaultConstructible<T> and NoThrowCopyable<T> and NoThrowDestructible<T>;


template<typename T>
concept NoThrowRegular = NoThrowSemiregular<T> and NoThrowMovable<T>;


template<typename T>
concept NoThrow = NoThrowDefaultConstructible<T> and
    NoThrowCopyConstructible<T> and
    NoThrowMoveConstructible<T> and
    NoThrowCopyAssignable<T> and
    NoThrowMoveAssignable<T> and
    NoThrowDestructible<T>;