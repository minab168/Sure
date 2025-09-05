#pragma once


#include <type_traits> // TODO remove dependency of std


template<typename T>
concept Integral = std::is_integral_v<T>;


template<typename T>
concept Floating = std::is_floating_point_v<T>;


template<typename T>
concept Boolean = __is_same(T, bool);


template<typename T>
concept Pointer = std::is_pointer_v<T>;


template<typename T>
concept Reference = std::is_reference_v<T>;


template<typename T>
concept Enum = __is_enum(T);


template<typename T>
concept TriviallyCopyable = __is_trivially_copyable(T);


template<typename T>
concept StandardLayout = __is_standard_layout(T);


template<typename T>
concept Primitive = Integral<T> or Floating<T> or Pointer<T> or Reference<T> or Enum<T>;