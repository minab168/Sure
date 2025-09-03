#pragma once



template<typename T>
concept DefaultConstructible = requires { T(); T{}; };



template<typename T>
concept Destructible = requires(T t) { { t.~T() } noexcept; };



template<typename T>
concept CopyConstructible = requires(T t) { T(t); T{t}; };



template<typename T>
concept MoveConstructible = requires(T t) { T((T&&)t); T{(T&&)t}; };



template<typename T>
concept CopyAssignable = requires(T t, T u) { { t = u } -> Destructible; };



template<typename T>
concept MoveAssignable = requires(T t, T u) { { t = (T&&)(u) } -> Destructible; };



template<typename T>
concept Copyable = CopyConstructible<T> and CopyAssignable<T> and Destructible<T>;



template<typename T>
concept Movable = MoveConstructible<T> and MoveAssignable<T> and Destructible<T>;