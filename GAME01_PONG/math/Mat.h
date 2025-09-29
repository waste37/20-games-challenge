#pragma once

#include <array>
#include <span>

#include "Concepts.h"

namespace math
{


namespace detail
{
template <ScalarLike T, size_t R, size_t C>
class MatStorage {
public:
    using ComponentType = T;
    using Pointer = ComponentType*;
    using ConstPointer = const ComponentType*;
    using Reference = ComponentType&;
    using ConstReference = const ComponentType&;

    using Iterator = Pointer;
    using ConstIterator = ConstPointer;
    using ReverseIterator = std::reverse_iterator<Pointer>;
    using ConstReverseIterator = std::reverse_iterator<ConstPointer>;

    static constexpr size_t Rows = R;
    static constexpr size_t Cols = C;

    [[nodiscard]] constexpr std::span<T> operator[](size_t col)
    {
        return std::span{ m_Storage.begin() + Rows * col, Rows };
    }

    [[nodiscard]] constexpr std::span<T> operator[](size_t col) const
    {
        return std::span{ m_Storage.begin() + Rows * col, Rows };
    }

    [[nodiscard]] constexpr Reference At(size_t row, size_t col)
    {
        if (row >= Rows || col >= Cols) throw std::out_of_range("Mat::At");
        return m_Storage[row + Rows * col];
    }

    [[nodiscard]] constexpr ConstReference At(size_t row, size_t col) const
    {
        if (row >= Rows || col >= Cols) throw std::out_of_range("Mat::At");
        return m_Storage[row + Rows * col];
    }

    [[nodiscard]] Pointer Ptr() noexcept
    {
        return m_Storage.data();
    }

    [[nodiscard]] ConstPointer Ptr() const noexcept
    {
        return m_Storage.data();
    }

    std::array<T, R* C> m_Storage;
};

} // detail

template <typename T, size_t R, size_t C>
class Mat : public detail::MatStorage<T, R, C> {
public:
    using Base = detail::MatStorage<T, R, C>;
    using Base::Rows;
    using Base::Cols;
    using Base::ComponentType;
    using Base::Reference;
};

template <ScalarLike T>
class Mat<T, 4, 4> : public detail::MatStorage<T, 4, 4> {
public:
    using Base = detail::MatStorage<T, 4, 4>;
    using Base::Rows;
    using Base::Cols;
    using Base::ComponentType;
    using Base::Reference;

    Mat(T a00, T a01, T a02, T a03,
        T a10, T a11, T a12, T a13,
        T a20, T a21, T a22, T a23,
        T a30, T a31, T a32, T a33)
        : Base::MatStorage{ {
            a00, a10, a20, a30,
            a01, a11, a21, a31,
            a02, a12, a22, a32,
            a03, a13, a23, a33
        } }
    {
    }

    Mat() : Mat{
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1,
    }
    {
    }
};


/* Default Constructors *******************/

template <ScalarLike T>
constexpr auto OrthographicProjection(T left, T right, T top, T bottom, T near, T far) noexcept
{
    return Mat<T, 4, 4>{
        T{ 2 } / (right - left), T{ 0 }, T{ 0 }, -(right + left) / (right - left),
            T{ 0 }, T{ 2 } / (top - bottom), T{ 0 }, -(top + bottom) / (top - bottom),
            T{ 0 }, T{ 0 }, -T{ 2 } / (far - near), -(far + near) / (far - near),
            T{ 0 }, T{ 0 }, T{ 0 }, T{ 1 }
    };
}

} // math