#pragma once


#include <algorithm>
#include <array>
#include <concepts>
#include <type_traits>
#include <random>

#include "Concepts.h"

namespace math
{

namespace detail
{
template <ScalarLike T, size_t N>
class VecStorage {
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

    void swap(VecStorage& other) noexcept(std::is_nothrow_swappable_v<T>)
    {
        for (std::size_t i = 0; i < Size; ++i) {
            using std::swap;
            swap((*this)[i], other[i]);
        }
    }

    // just for the loops
    [[nodiscard]] Iterator begin() noexcept { return Iterator(m_Storage); }
    [[nodiscard]] ConstIterator begin() const noexcept { return ConstIterator(m_Storage); }
    [[nodiscard]] Iterator end() noexcept { return Iterator(m_Storage); }
    [[nodiscard]] ConstIterator end() const noexcept { return ConstIterator(m_Storage.begin() + Size); }
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

#define LINEAR_ALGEBRA__DEFINE_VECTOR_ACCESSOR(AccessorName, index) \
constexpr T &AccessorName() { return (*this)[index]; }              \
constexpr const T &AccessorName() const { return (*this)[index]; }  \


template <ScalarLike T, size_t N> class Vec : public detail::VecStorage<T, N> {
public:
    using Base = detail::VecStorage<T, N>;
    using Base::Size;
    using Base::ComponentType;
    using Base::Reference;
};

template <ScalarLike T>
class Vec<T, 2> : public detail::VecStorage<T, 2> {
public:
    using Base = detail::VecStorage<T, 2>;
    using Base::Size;
    using Base::ComponentType;
    using Base::Reference;

    constexpr Vec(T x, T y) : Base::VecStorage{ {x, y} } {}

    constexpr Vec() : Vec{ T{0} } {}
    constexpr explicit Vec(T v) : Vec{ v, v } {}

    template <ScalarLike U, std::enable_if_t<std::is_convertible_v<U, T>, int> = 0>
    constexpr Vec(Vec<U, 2>& other) : Vec(other.x(), other.y()) {}

    template <ScalarLike U, std::enable_if_t<std::is_convertible_v<U, T>, int> = 0>
    constexpr explicit Vec(Vec<U, 3>& other) : Vec{ other.x(), other.y() } {}

    template <ScalarLike U, std::enable_if_t<std::is_convertible_v<U, T>, int> = 0>
    constexpr explicit Vec(Vec<U, 4>& other) : Vec{ other.x(), other.y() } {}

    template <ScalarLike U, std::enable_if_t<std::is_convertible_v<U, T>, int> = 0>
    constexpr explicit Vec(U* ptr) : Vec{ ptr[0], ptr[1] } {}

    LINEAR_ALGEBRA__DEFINE_VECTOR_ACCESSOR(x, 0);
    LINEAR_ALGEBRA__DEFINE_VECTOR_ACCESSOR(s, 0);
    LINEAR_ALGEBRA__DEFINE_VECTOR_ACCESSOR(r, 0);

    LINEAR_ALGEBRA__DEFINE_VECTOR_ACCESSOR(y, 1);
    LINEAR_ALGEBRA__DEFINE_VECTOR_ACCESSOR(t, 1);
    LINEAR_ALGEBRA__DEFINE_VECTOR_ACCESSOR(g, 1);
};


template <ScalarLike T>
class Vec<T, 3> : public detail::VecStorage<T, 3> {
    using Base = detail::VecStorage<T, 3>;
    using Base::Size;
    using Base::ComponentType;
    using Base::Reference;

    constexpr Vec(T x, T y, T z) : Base::VecStorage{ {x, y, z} } {}

    constexpr Vec() : Vec{ T{0} } {}
    constexpr explicit Vec(T v) : Vec{ v, v, v } {}

    constexpr Vec(T x, Vec<T, 2>& yz) : Vec{ x, yz[0], yz[1] } {}
    constexpr Vec(Vec<T, 2>& xy, T z) : Vec{ xy[0], xy[1], z } {}

    template <ScalarLike U, std::enable_if_t<std::is_convertible_v<U, T>, int> = 0>
    constexpr Vec(Vec<U, 3>& other) : Vec{ other.x, other.y, other.z } {}

    template <ScalarLike U, std::enable_if_t<std::is_convertible_v<U, T>, int> = 0>
    constexpr explicit Vec(Vec<U, 4>& other) : Vec{ other.x, other.y, other.z } {}

    template <ScalarLike U, std::enable_if_t<std::is_convertible_v<U, T>, int> = 0>
    constexpr explicit Vec(U* ptr) : Vec{ ptr[0], ptr[1], ptr[2] } {}

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

template <ScalarLike T>
class Vec<T, 4> : public detail::VecStorage<T, 4> {
public:
    using Base = detail::VecStorage<T, 4>;
    using Base::Size;
    using Base::ComponentType;
    using Base::Reference;

    constexpr Vec(T x, T y, T z, T w) : Base::VecStorage{ {x, y, z, w} } {}

    constexpr Vec() : Vec{ T{0} } {}
    constexpr explicit Vec(T v) : Vec{ v, v, v, v } {}

    constexpr Vec(Vec<T, 2>& xy, T z, T w) : Vec{ xy[0], xy[1], z, w } {}
    constexpr Vec(T x, Vec<T, 2>& yz, T w) : Vec{ x, yz[0], yz[1], w } {}
    constexpr Vec(T x, T y, Vec<T, 2>& zw) : Vec{ x, y, zw[0], zw[1] } {}
    constexpr Vec(Vec<T, 2>& xy, Vec<T, 2> zw) : Vec{ xy[0], xy[1], zw[0], zw[1] } {}

    constexpr Vec(Vec<T, 3>& xyz, T w) : Vec{ xyz[0], xyz[1], xyz[2], w } {}
    constexpr Vec(T x, Vec<T, 3>& yzw) : Vec{ x, yzw[0], yzw[1], yzw[2] } {}

    template <ScalarLike U, std::enable_if_t<std::is_convertible_v<U, T>, int> = 0>
    constexpr Vec(Vec<U, 4>& other) : Vec{ other.x, other.y, other.z, other.w } {}

    template <ScalarLike U, std::enable_if_t<std::is_convertible_v<U, T>, int> = 0>
    constexpr explicit Vec(U* ptr) : Vec{ ptr[0], ptr[1], ptr[2], ptr[3] } {}

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

template <ScalarLike T> using Vec2 = Vec<T, 2>;
template <ScalarLike T> using Vec3 = Vec<T, 3>;
template <ScalarLike T> using Vec4 = Vec<T, 4>;

template <ScalarLike T> Vec(T, T) -> Vec<T, 2>;
template <ScalarLike T> Vec(T, T, T) -> Vec<T, 3>;
template <ScalarLike T> Vec(Vec<T, 2>, T) -> Vec<T, 3>;
template <ScalarLike T> Vec(T, Vec<T, 3>) -> Vec<T, 3>;
template <ScalarLike T> Vec(T, T, T, T) -> Vec<T, 4>;
template <ScalarLike T> Vec(T, T, Vec<T, 2>) -> Vec<T, 4>;
template <ScalarLike T> Vec(T, Vec<T, 2>, T) -> Vec<T, 4>;
template <ScalarLike T> Vec(Vec<T, 2>, T, T) -> Vec<T, 4>;
template <ScalarLike T> Vec(Vec<T, 2>, Vec<T, 2>) -> Vec<T, 4>;
template <ScalarLike T> Vec(Vec<T, 3>, T) -> Vec<T, 4>;
template <ScalarLike T> Vec(T, Vec<T, 3>) -> Vec<T, 4>;

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

    constexpr uint32_t ToUint32() const noexcept
    {
        return (static_cast<uint32_t>(r() * 255.0f) << 24)
            | (static_cast<uint32_t>(g() * 255.0f) << 16)
            | (static_cast<uint32_t>(b() * 255.0f) << 8)
            | (static_cast<uint32_t>(a() * 255.0f));
    }

    friend bool operator==(Color a, Color b)
    {
        return a.ToUint32() == b.ToUint32();
    }
};

namespace detail
{
template <typename F, VectorLike VecT, size_t ...Is>
[[nodiscard]] constexpr auto MapImpl(F&& f, const VecT& x, std::index_sequence<Is...>)
{
    return VecT{ f(x[Is])... };
}

template <typename F, VectorLike VecT, VectorLike VecU, size_t ...Is>
[[nodiscard]] constexpr auto MapImpl(F&& f, const VecT& x, const VecU& y, std::index_sequence<Is...>) noexcept requires VectorLikeSameSize<VecT, VecU>
{
    using ReturnType = std::common_type_t<typename VecT::ComponentType, typename VecU::ComponentType>;
    return Vec<ReturnType, VecT::Size>(f(x[Is], y[Is])...);
}

template <typename F, VectorLike VecT>
[[nodiscard]] constexpr auto Map(F&& f, const VecT& x) noexcept
{
    return MapImpl(std::forward<F>(f), x, std::make_index_sequence<VecT::Size>{});
}

template <typename F, VectorLike VecT, VectorLike VecU>
[[nodiscard]] constexpr auto Map(F&& f, const VecT& x, const VecU& y) noexcept requires VectorLikeSameSize<VecT, VecU>
{
    return MapImpl(std::forward<F>(f), x, y, std::make_index_sequence<VecT::Size>{});
}

template <typename F, VectorLike VecT, std::size_t... Is >
[[nodiscard]] constexpr bool AndFoldImpl(F&& f, const VecT& x, std::index_sequence<Is...>)
{
    return (... && f(x[Is]));
}

template <typename F, VectorLike VecT, VectorLike VecU, size_t ...Is>
[[nodiscard]] constexpr bool AndFoldImpl(F&& f, const VecT& x, const VecU& y, std::index_sequence<Is...>) noexcept requires VectorLikeSameSize<VecT, VecU>
{
    using ReturnType = std::common_type_t<typename VecT::ComponentType, typename VecU::ComponentType>;
    return (... && f(x[Is], y[Is]));
}

template <typename F, VectorLike VecT>
[[nodiscard]] constexpr bool AndFold(F&& f, const VecT& x)
{
    return AndFoldImpl(std::forward<F>(f), x, std::make_index_sequence<VecT::Size>{});
}

template <typename F, VectorLike VecT, VectorLike VecU>
[[nodiscard]] constexpr bool AndFold(F&& f, const VecT& x, const VecU& y) noexcept requires VectorLikeSameSize<VecT, VecU>
{
    return AndFoldImpl(std::forward<F>(f), x, y, std::make_index_sequence<VecT::Size>{});
}

template <typename F, VectorLike VecT, std::size_t... Is >
[[nodiscard]] constexpr bool PLusFoldImpl(F&& f, const VecT& x, std::index_sequence<Is...>)
{
    return (... + f(x[Is]));
}

template <typename F, VectorLike VecT, VectorLike VecU, size_t ...Is>
[[nodiscard]] constexpr std::common_type_t<typename VecT::ComponentType, typename VecU::ComponentType>
PlusFoldImpl(F&& f, const VecT& x, const VecU& y, std::index_sequence<Is...>) noexcept requires VectorLikeSameSize<VecT, VecU>
{
    return (... + f(x[Is], y[Is]));
}

template <typename F, VectorLike VecT>
[[nodiscard]] constexpr bool PlusFold(F&& f, const VecT& x)
{
    return PlusFoldImpl(std::forward<F>(f), x, std::make_index_sequence<VecT::Size>{});
}

template <typename F, VectorLike VecT, VectorLike VecU>
[[nodiscard]] constexpr auto PlusFold(F&& f, const VecT& x, const VecU& y) noexcept requires VectorLikeSameSize<VecT, VecU>
{
    return PlusFoldImpl(std::forward<F>(f), x, y, std::make_index_sequence<VecT::Size>{});
}

}

/* Unary Operations **************************************************/

// operator +
template <VectorLike VecT>
[[nodiscard]] constexpr auto operator+(const VecT& x) noexcept
{
    return detail::Map([](VecT::ComponentType x_i) { return +x_i; }, x);
}

// operator -
template <VectorLike VecT>
[[nodiscard]] constexpr auto operator-(const VecT& x) noexcept
{
    return detail::Map([](VecT::ComponentType x_i) { return -x_i; }, x);
}


/* Binary Operations ***********************************************/

// operator +

template <VectorLike VecT>
[[nodiscard]] constexpr auto operator+(const VecT& x, ScalarLike auto y) noexcept
{
    return detail::Map([y](VecT::ComponentType x_i) { return x_i + y; }, x);
}

template <VectorLike VecU>
[[nodiscard]] constexpr auto operator+(ScalarLike auto x, const VecU& y) noexcept
{
    return detail::Map([x](VecU::ComponentType y_i) { return x + y_i; }, y);
}

template <VectorLike VecT, VectorLike VecU>
[[nodiscard]] constexpr auto operator+(const VecT& x, const VecU& y) noexcept requires VectorLikeSameSize<VecT, VecU>
{
    return detail::Map([](VecT::ComponentType x_i, VecU::ComponentType y_i) { return x_i + y_i; }, x, y);
}


// operator -

template <VectorLike VecT>
[[nodiscard]] constexpr auto operator-(const VecT& x, ScalarLike auto y) noexcept
{
    return detail::Map([y](VecT::ComponentType x_i) { return x_i - y; }, x);
}

template <VectorLike VecU>
[[nodiscard]] constexpr auto operator-(ScalarLike auto x, const VecU& y) noexcept
{
    return detail::Map([x](VecU::ComponentType y_i) { return x - y_i; }, y);
}

template <VectorLike VecT, VectorLike VecU>
[[nodiscard]] constexpr auto operator-(const VecT& x, const VecU& y) noexcept requires VectorLikeSameSize<VecT, VecU>
{
    return detail::Map([](VecT::ComponentType x_i, VecU::ComponentType y_i) { return x_i - y_i; }, x, y);
}

// operator *

template <VectorLike VecT>
[[nodiscard]] constexpr auto operator*(const VecT& x, ScalarLike auto y) noexcept
{
    return detail::Map([y](VecT::ComponentType x_i) { return x_i * y; }, x);
}

template <VectorLike VecU>
[[nodiscard]] constexpr auto operator*(ScalarLike auto x, const VecU& y) noexcept
{
    return detail::Map([x](VecU::ComponentType y_i) { return x * y_i; }, y);
}

template <VectorLike VecT, VectorLike VecU>
[[nodiscard]] constexpr auto operator*(const VecT& x, const VecU& y) noexcept requires VectorLikeSameSize<VecT, VecU>
{
    return detail::Map([](VecT::ComponentType x_i, VecU::ComponentType y_i) { return x_i * y_i; }, x, y);
}

// operator /

template <VectorLike VecT>
[[nodiscard]] constexpr auto operator/(const VecT& x, ScalarLike auto y) noexcept
{
    return detail::Map([y](VecT::ComponentType x_i) { return x_i / y; }, x);
}

template <VectorLike VecU>
[[nodiscard]] constexpr auto operator/(ScalarLike auto x, const VecU& y) noexcept
{
    return detail::Map([x](VecU::ComponentType y_i) { return x / y_i; }, y);
}

template <VectorLike VecT, VectorLike VecU>
[[nodiscard]] constexpr auto operator/(const VecT& x, const VecU& y) noexcept requires VectorLikeSameSize<VecT, VecU>
{
    return detail::Map([](VecT::ComponentType x_i, VecU::ComponentType y_i) { return x_i / y_i; }, x, y);
}

// relational 

// operator ==

template <VectorLike VecT, VectorLike VecU>
[[nodiscard]] constexpr bool operator==(const VecT& x, const VecU& y) noexcept requires VectorLikeSameSize<VecT, VecU>
{
    return detail::AndFold([](VecT::ComponentType x_i, VecU::ComponentType y_i) { return x_i == y_i; }, x, y);
}


template <VectorLike VecT, VectorLike VecU>
[[nodiscard]] constexpr bool operator!=(const VecT& x, const VecU& y) noexcept requires VectorLikeSameSize<VecT, VecU>
{
    return detail::AndFold([](VecT::ComponentType x_i, VecU::ComponentType y_i) { return x_i != y_i; }, x, y);
}


// geometric operations

template <VectorLike VecT, VectorLike VecU>
constexpr auto Dot(const VecT& x, const VecU& y) noexcept requires VectorLikeSameSize<VecT, VecU>
{
    return detail::PlusFold([](VecT::ComponentType x_i, VecU::ComponentType y_i) { return x_i * y_i; }, x, y);
}


template <VectorLike VecT>
constexpr auto Length(const VecT& v) noexcept
{
    return sqrt(Dot(v, v));
}

template<typename T, size_t N>
constexpr auto RandomUnitVector() noexcept
{
    static std::random_device rd{};
    static std::mt19937 gen{ rd() };
    static std::normal_distribution d{ 0.0f, 1.0f };

    Vec<T, N> v{ 0.0f };
    v = detail::Map([](auto x) { return d(gen); }, v);
    return  v / Length(v);
}

} // math

