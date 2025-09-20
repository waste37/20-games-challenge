#pragma once


#include <algorithm>
#include <array>
#include <concepts>
#include <type_traits>

namespace geom
{

namespace detail
{
template <typename T, size_t N>
class VecStorage {
public:
    using ComponentType = T;
    using Pointer = ComponentType *;
    using ConstPointer = const ComponentType *;
    using Reference = ComponentType &;
    using ConstReference = const ComponentType &;

    using Iterator = Pointer;
    using ConstIterator = ConstPointer;
    using ReverseIterator = std::reverse_iterator<Pointer>;
    using ConstReverseIterator = std::reverse_iterator<ConstPointer>;

    static constexpr size_t Size = N;

    [[nodiscard]] constexpr Reference operator[](size_t index) noexcept
    {
        return m_Storage[index];
    }

    [[nodiscard]] constexpr ConstReference operator[](size_t index) const noexcept
    {
        return m_Storage[index];
    }

    [[nodiscard]] constexpr Reference At(size_t index)
    {
        if (index >= Size) throw std::out_of_range("Vec::At");
        return m_Storage[index];
    }

    [[nodiscard]] constexpr ConstReference At(size_t index) const
    {
        if (index >= Size) throw std::out_of_range("Vec::At");
        return m_Storage[index];
    }

    [[nodiscard]] Pointer Ptr() noexcept
    {
        return m_Storage.data();
    }

    [[nodiscard]] ConstPointer Ptr() const noexcept
    {
        return m_Storage.data();
    }

    void swap(VecStorage &other) noexcept(std::is_nothrow_swappable_v<T>)
    {
        for (std::size_t i = 0; i < Size; ++i) {
            using std::swap;
            swap((*this)[i], other[i]);
        }
    }

    // just for the loops
    [[nodiscard]] Iterator begin() noexcept { return Iterator(storage); }
    [[nodiscard]] ConstIterator begin() const noexcept { return ConstIterator(storage); }
    [[nodiscard]] Iterator end() noexcept { return Iterator(storage); }
    [[nodiscard]] ConstIterator end() const noexcept { return ConstIterator(storage + Size); }
    [[nodiscard]] ReverseIterator rbegin() noexcept { return ReverseIterator(end()); }
    [[nodiscard]] ConstReverseIterator rbegin() const noexcept { return ConstReverseIterator(end()); }
    [[nodiscard]] ReverseIterator rend() noexcept { return ReverseIterator(begin()); }
    [[nodiscard]] ConstReverseIterator rend() const noexcept { return ConstReverseIterator(begin()); }
    [[nodiscard]] ConstIterator cbegin() const noexcept { return begin(); }
    [[nodiscard]] ConstIterator cend() const noexcept { return end(); }
    [[nodiscard]] ConstReverseIterator crbegin() const noexcept { return rbegin(); }
    [[nodiscard]] ConstReverseIterator crend() const noexcept { return rend(); }

    std::array<T, N> m_Storage;
};
} // detail

#define LINEAR_ALGEBRA__DEFINE_VECTOR_ACCESSOR(AccessorName, index)         \
constexpr T &AccessorName() { return (*this)[index]; }              \
constexpr const T &AccessorName() const { return (*this)[index]; }  \


template <typename T, size_t N> class Vec : public detail::VecStorage<T, N> {
    using Base = detail::VecStorage<T, N>;
    using Base::Size;
    using Base::ComponentType;
    using Base::Reference;
};

template <typename T>
class Vec<T, 2> : public detail::VecStorage<T, 2> {
public:
    using Base = detail::VecStorage<T, 2>;
    using Base::Size;
    using Base::ComponentType;
    using Base::Reference;

    constexpr Vec(T x, T y) : Base::VecStorage{ {x, y} } {}

    constexpr Vec() : Vec{ T{0} } {}
    constexpr explicit Vec(T v) : Vec{ v, v } {}

    template <typename U, std::enable_if_t<std::is_convertible_v<U, T>, int> = 0>
    constexpr Vec(Vec<U, 2> &other) : Vec(other.x(), other.y()) {}

    template <typename U, std::enable_if_t<std::is_convertible_v<U, T>, int> = 0>
    constexpr explicit Vec(Vec<U, 3> &other) : Vec{ other.x(), other.y() } {}

    template <typename U, std::enable_if_t<std::is_convertible_v<U, T>, int> = 0>
    constexpr explicit Vec(Vec<U, 4> &other) : Vec{ other.x(), other.y() } {}

    template <typename U, std::enable_if_t<std::is_convertible_v<U, T>, int> = 0>
    constexpr explicit Vec(U *ptr) : Vec{ ptr[0], ptr[1] } {}

    LINEAR_ALGEBRA__DEFINE_VECTOR_ACCESSOR(x, 0);
    LINEAR_ALGEBRA__DEFINE_VECTOR_ACCESSOR(s, 0);
    LINEAR_ALGEBRA__DEFINE_VECTOR_ACCESSOR(r, 0);

    LINEAR_ALGEBRA__DEFINE_VECTOR_ACCESSOR(y, 1);
    LINEAR_ALGEBRA__DEFINE_VECTOR_ACCESSOR(t, 1);
    LINEAR_ALGEBRA__DEFINE_VECTOR_ACCESSOR(g, 1);
};


template <typename T>
class Vec<T, 3> : public detail::VecStorage<T, 3> {
    using Base = detail::VecStorage<T, 3>;
    using Base::Size;
    using Base::ComponentType;
    using Base::Reference;

    constexpr Vec(T x, T y, T z) : Base::VecStorage{ {x, y, z} } {}

    constexpr Vec() : Vec{ T{0} } {}
    constexpr explicit Vec(T v) : Vec{ v, v, v } {}

    constexpr Vec(T x, Vec<T, 2> &yz) : Vec{ x, yz[0], yz[1] } {}
    constexpr Vec(Vec<T, 2> &xy, T z) : Vec{ xy[0], xy[1], z } {}

    template <typename U, std::enable_if_t<std::is_convertible_v<U, T>, int> = 0>
    constexpr Vec(Vec<U, 3> &other) : Vec{ other.x, other.y, other.z } {}

    template <typename U, std::enable_if_t<std::is_convertible_v<U, T>, int> = 0>
    constexpr explicit Vec(Vec<U, 4> &other) : Vec{ other.x, other.y, other.z } {}

    template <typename U, std::enable_if_t<std::is_convertible_v<U, T>, int> = 0>
    constexpr explicit Vec(U *ptr) : Vec{ ptr[0], ptr[1], ptr[2] } {}

    LINEAR_ALGEBRA__DEFINE_VECTOR_ACCESSOR(x, 0);
    LINEAR_ALGEBRA__DEFINE_VECTOR_ACCESSOR(s, 0);
    LINEAR_ALGEBRA__DEFINE_VECTOR_ACCESSOR(r, 0);

    LINEAR_ALGEBRA__DEFINE_VECTOR_ACCESSOR(y, 1);
    LINEAR_ALGEBRA__DEFINE_VECTOR_ACCESSOR(t, 1);
    LINEAR_ALGEBRA__DEFINE_VECTOR_ACCESSOR(g, 1);

    LINEAR_ALGEBRA__DEFINE_VECTOR_ACCESSOR(z, 2);
    LINEAR_ALGEBRA__DEFINE_VECTOR_ACCESSOR(p, 2);
    LINEAR_ALGEBRA__DEFINE_VECTOR_ACCESSOR(b, 2);
};

template <typename T>
class Vec<T, 4> : public detail::VecStorage<T, 4> {
public:
    using Base = detail::VecStorage<T, 4>;
    using Base::Size;
    using Base::ComponentType;
    using Base::Reference;

    constexpr Vec(T x, T y, T z, T w) : Base::VecStorage{ {x, y, z, w} } {}

    constexpr Vec() : Vec{ T{0} } {}
    constexpr explicit Vec(T v) : Vec{ v, v, v, v } {}

    constexpr Vec(Vec<T, 2> &xy, T z, T w) : Vec{ xy[0], xy[1], z, w } {}
    constexpr Vec(T x, Vec<T, 2> &yz, T w) : Vec{ x, yz[0], yz[1], w } {}
    constexpr Vec(T x, T y, Vec<T, 2> &zw) : Vec{ x, y, zw[0], zw[1] } {}
    constexpr Vec(Vec<T, 2> &xy, Vec<T, 2> zw) : Vec{ xy[0], xy[1], zw[0], zw[1] } {}

    constexpr Vec(Vec<T, 3> &xyz, T w) : Vec{ xyz[0], xyz[1], xyz[2], w } {}
    constexpr Vec(T x, Vec<T, 3> &yzw) : Vec{ x, yzw[0], yzw[1], yzw[2] } {}

    template <typename U, std::enable_if_t<std::is_convertible_v<U, T>, int> = 0>
    constexpr Vec(Vec<U, 4> &other) : Vec{ other.x, other.y, other.z, other.w } {}

    template <typename U, std::enable_if_t<std::is_convertible_v<U, T>, int> = 0>
    constexpr explicit Vec(U *ptr) : Vec{ ptr[0], ptr[1], ptr[2], ptr[3] } {}

    LINEAR_ALGEBRA__DEFINE_VECTOR_ACCESSOR(x, 0);
    LINEAR_ALGEBRA__DEFINE_VECTOR_ACCESSOR(s, 0);
    LINEAR_ALGEBRA__DEFINE_VECTOR_ACCESSOR(r, 0);

    LINEAR_ALGEBRA__DEFINE_VECTOR_ACCESSOR(y, 1);
    LINEAR_ALGEBRA__DEFINE_VECTOR_ACCESSOR(t, 1);
    LINEAR_ALGEBRA__DEFINE_VECTOR_ACCESSOR(g, 1);

    LINEAR_ALGEBRA__DEFINE_VECTOR_ACCESSOR(z, 2);
    LINEAR_ALGEBRA__DEFINE_VECTOR_ACCESSOR(p, 2);
    LINEAR_ALGEBRA__DEFINE_VECTOR_ACCESSOR(b, 2);

    LINEAR_ALGEBRA__DEFINE_VECTOR_ACCESSOR(w, 3);
    LINEAR_ALGEBRA__DEFINE_VECTOR_ACCESSOR(q, 3);
    LINEAR_ALGEBRA__DEFINE_VECTOR_ACCESSOR(a, 3);
};

#undef LINEAR_ALGEBRA__DEFINE_VECTOR_ACCESSOR

template <typename T> using Vec2 = Vec<T, 2>;
template <typename T> using Vec3 = Vec<T, 2>;
template <typename T> using Vec4 = Vec<T, 2>;

template <typename T> Vec(T, T) -> Vec<T, 2>;
template <typename T> Vec(T, T, T) -> Vec<T, 3>;
template <typename T> Vec(Vec<T, 2>, T) -> Vec<T, 3>;
template <typename T> Vec(T, Vec<T, 3>) -> Vec<T, 3>;
template <typename T> Vec(T, T, T, T) -> Vec<T, 4>;
template <typename T> Vec(T, T, Vec<T, 2>) -> Vec<T, 4>;
template <typename T> Vec(T, Vec<T, 2>, T) -> Vec<T, 4>;
template <typename T> Vec(Vec<T, 2>, T, T) -> Vec<T, 4>;
template <typename T> Vec(Vec<T, 2>, Vec<T, 2>) -> Vec<T, 4>;
template <typename T> Vec(Vec<T, 3>, T) -> Vec<T, 4>;
template <typename T> Vec(T, Vec<T, 3>) -> Vec<T, 4>;

struct Color : public Vec<float, 4> {
    using Vec<float, 4>::Size;
    using Vec<float, 4>::ComponentType;

    constexpr Color(float r, float g, float b, float a = 1.0f) : Vec{ r, g, b, a } {}
    constexpr Color() : Color{ 0.0f, 0.0f, 0.0f, 0.0f } {}

    constexpr Color(uint32_t hex)
        : Color{
            (float)((hex >> 24) & 0xff) / 255.0f,
            (float)((hex >> 16) & 0xff) / 255.0f,
            (float)((hex >> 8) & 0xff) / 255.0f,
            (float)(hex & 0xff) / 255.0f
        }
    {
    }
};

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

namespace detail
{
template <typename F, VectorLike VecT, size_t ...Is>
constexpr auto MapImpl(F &&f, const VecT &x, std::index_sequence<Is...>)
{
    return VecT{ f(x[Is])... };
}

template <typename F, VectorLike VecT, VectorLike VecU, size_t ...Is>
constexpr auto MapImpl(F &&f, const VecT &x, const VecU &y, std::index_sequence<Is...>) noexcept requires VectorLikeSameSize<VecT, VecU>
{
    using ReturnType = std::common_type_t<typename VecT::ComponentType, typename VecU::ComponentType>;
    return Vec<ReturnType, VecT::Size>(f(x[Is], y[Is])...);
}

template <typename F, VectorLike VecT>
auto Map(F &&f, const VecT &x) noexcept
{
    return MapImpl(std::forward<F>(f), x, std::make_index_sequence<VecT::Size>{});
}

template <typename F, VectorLike VecT, VectorLike VecU>
auto Map(F &&f, const VecT &x, const VecU &y) noexcept requires VectorLikeSameSize<VecT, VecU>
{
    return MapImpl(std::forward<F>(f), x, y, std::make_index_sequence<VecT::Size>{});
}
}

/* Unary Operations **************************************************/

// operator +
template <VectorLike VecT>
[[nodiscard]] constexpr auto operator+(const VecT &x) noexcept
{
    return detail::Map([](VecT::ComponentType x_i) { return +x_i; }, x);
}

// operator -
template <VectorLike VecT>
[[nodiscard]] constexpr auto operator-(const VecT &x) noexcept
{
    return detail::Map([](VecT::ComponentType x_i) { return -x_i; }, x);
}


/* Binary Operations ***********************************************/

// operator +

template <VectorLike VecT>
[[nodiscard]] constexpr auto operator+(const VecT &x, ScalarLike auto y) noexcept
{
    return detail::Map([y](VecT::ComponentType x_i) { return x_i + y; }, x);
}

template <VectorLike VecU>
[[nodiscard]] constexpr auto operator+(ScalarLike auto x, const VecU &y) noexcept
{
    return detail::Map([x](VecU::ComponentType y_i) { return x + y_i; }, y);
}

template <VectorLike VecT, VectorLike VecU>
[[nodiscard]] constexpr auto operator+(const VecT &x, const VecU &y) noexcept requires VectorLikeSameSize<VecT, VecU>
{
    return detail::Map([](VecT::ComponentType x_i, VecT::ComponentType y_i) { return x_i + y_i; }, x, y);
}


// operator -

template <VectorLike VecT>
[[nodiscard]] constexpr auto operator-(const VecT &x, ScalarLike auto y) noexcept
{
    return detail::Map([y](VecT::ComponentType x_i) { return x_i - y; }, x);
}

template <VectorLike VecU>
[[nodiscard]] constexpr auto operator-(ScalarLike auto x, const VecU &y) noexcept
{
    return detail::Map([x](VecU::ComponentType y_i) { return x - y_i; }, y);
}

template <VectorLike VecT, VectorLike VecU>
[[nodiscard]] constexpr auto operator-(const VecT &x, const VecU &y) noexcept requires VectorLikeSameSize<VecT, VecU>
{
    return detail::Map([](VecT::ComponentType x_i, VecT::ComponentType y_i) { return x_i - y_i; }, x, y);
}

// operator *

template <VectorLike VecT>
[[nodiscard]] constexpr auto operator*(const VecT &x, ScalarLike auto y) noexcept
{
    return detail::Map([y](VecT::ComponentType x_i) { return x_i * y; }, x);
}

template <VectorLike VecU>
[[nodiscard]] constexpr auto operator*(ScalarLike auto x, const VecU &y) noexcept
{
    return detail::Map([x](VecU::ComponentType y_i) { return x * y_i; }, y);
}

template <VectorLike VecT, VectorLike VecU>
[[nodiscard]] constexpr auto operator*(const VecT &x, const VecU &y) noexcept requires VectorLikeSameSize<VecT, VecU>
{
    return detail::Map([](VecT::ComponentType x_i, VecT::ComponentType y_i) { return x_i * y_i; }, x, y);
}

// operator /

template <VectorLike VecT>
[[nodiscard]] constexpr auto operator/(const VecT &x, ScalarLike auto y) noexcept
{
    return detail::Map([y](VecT::ComponentType x_i) { return x_i / y; }, x);
}

template <VectorLike VecU>
[[nodiscard]] constexpr auto operator/(ScalarLike auto x, const VecU &y) noexcept
{
    return detail::Map([x](VecU::ComponentType y_i) { return x / y_i; }, y);
}

template <VectorLike VecT, VectorLike VecU>
[[nodiscard]] constexpr auto operator/(const VecT &x, const VecU &y) noexcept requires VectorLikeSameSize<VecT, VecU>
{
    return detail::Map([](VecT::ComponentType x_i, VecT::ComponentType y_i) { return x_i / y_i; }, x, y);
}

// geometric operations


} // LinearAlgebra

