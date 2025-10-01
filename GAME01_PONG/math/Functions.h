#pragma once

#include <cmath>
#include <type_traits>

namespace math
{

template <typename T>
[[nodiscard]] std::enable_if_t<std::is_floating_point_v<T>, T>
constexpr sqrt(T x) noexcept
{
    return std::sqrt(x);
}

}
