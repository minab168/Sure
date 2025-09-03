#pragma once




template<typename T>
concept Integral = __is_integral(T);


template<typename T>
concept Floating = __is_floating_point(T);


template<typename T>
concept Boolean = __is_same(T, bool);


template<typename T>
concept Pointer = __is_pointer(T);


template<typename T>
concept Reference = __is_lvalue_reference(T) or __is_rvalue_reference(T);


template<typename T>
concept Enum = __is_enum(T);


template<typename T>
concept TriviallyCopyable = __is_trivially_copyable(T);


template<typename T>
concept StandardLayout = __is_standard_layout(T);


template<typename T>
concept Primitive = Integral<T> or Floating<T> or Pointer<T> or Reference<T> or Enum<T>;