#pragma once

#include <type_traits>

template <typename T>
concept Arithmetic = std::is_arithmetic_v<T>;
template <typename T>
concept AllowsSubscripting = requires(T x, std::size_t i) { x[i]; };

template <typename T>
concept ScalarLike = !AllowsSubscripting<T> && Arithmetic<T>;

template <typename V>
concept VectorLike = requires(V v, std::size_t i)
{
    typename V::ComponentType;
    { V::Size } -> std::convertible_to<std::size_t>;
    { v[i] } -> std::convertible_to<typename V::ComponentType>;
};

template <typename V1, typename V2>
concept VectorLikeSameSize = VectorLike<V1> && VectorLike<V2> && (V1::Size == V2::Size);