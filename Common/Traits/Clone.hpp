#pragma once


template<typename T>
concept Clonable = requires(T t) {
    { t.clone() } -> Clonable;
};