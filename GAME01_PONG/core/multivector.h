#pragma once
#include <algorithm>
#include <iostream>
#include <type_traits>
#include <numeric>
#include <utility>
#include <vector>

namespace core
{

template <typename ...Types>
class multivector {
public:
    using slice = std::tuple<Types...>;
    using slice_ref = std::tuple<Types&...>;
    using const_slice_ref = std::tuple<const Types&...>;

    multivector() = default;
    constexpr multivector(size_t capacity)
    {
        reserve(capacity);
    }

    constexpr void resize(size_t capacity)
    {
        resize_impl(capacity, std::index_sequence_for<Types...>{});
    }

    constexpr void clear() noexcept
    {
        clear_impl(std::index_sequence_for<Types...>{});
    }


    constexpr void reserve(size_t capacity)
    {
        reserve_impl(capacity, std::index_sequence_for<Types...>{});
    }

    template <typename ...Args>
    constexpr void push_back(Args&& ...args)
    {
        static_assert(sizeof ...(Args) == sizeof ...(Types),
            "multi_vector::push_back expects a number of argument equal to the template parameters.");
        push_back_impl(std::forward_as_tuple(std::forward<Args>(args)...), std::index_sequence_for<Types...>{});
    }

    constexpr void push_back(const std::tuple<Types...>& value)
    {
        push_back_impl(value, std::index_sequence_for<Types...>{});
    }

    constexpr void push_back(std::tuple<Types...>&& x)
    {
        push_back_impl(std::move(x), std::index_sequence_for<Types...>{});
    }

    constexpr const_slice_ref operator[](size_t i) const noexcept
    {
        return at_impl(i, std::index_sequence_for<Types...>{});
    }

    constexpr slice_ref operator[](size_t i) noexcept
    {
        return at_impl(i, std::index_sequence_for<Types...>{});
    }

    constexpr const_slice_ref at(size_t i) const
    {
        if (i >= size()) {
            throw std::out_of_range("multi_vector::at");
        }

        return at_impl(i, std::index_sequence_for<Types...>{});
    }

    constexpr slice_ref at(size_t i)
    {
        if (i >= size()) {
            throw std::out_of_range("multi_vector::at");
        }

        return at_impl(i, std::index_sequence_for<Types...>{});
    }

    template <size_t Index>
    constexpr auto& get() noexcept
    {
        return std::get<Index>(m_storage);
    }

    template <size_t Index>
    constexpr const auto& get() const noexcept
    {
        return std::get<Index>(m_storage);
    }

    constexpr size_t size() const noexcept
    {
        if constexpr (sizeof...(Types) > 0) {
            return std::get<0>(m_storage).size();
        }
        return 0;
    }

    constexpr bool empty() const noexcept
    {
        return size() == 0;
    }

    constexpr void swap_elements(size_t i, size_t j)
    {
        swap_elements_impl(i, j, std::index_sequence_for<Types...>{});
    }

    template <typename Compare = std::less<>>
    void sort(Compare comp = std::less{})
    {
        if (size() < 2) return;

        std::vector<size_t> indices(size());
        std::iota(indices.begin(), indices.end(), 0);

        std::sort(indices.begin(), indices.end(), [this, &comp](size_t i, size_t j) {
            return comp((*this)[i], (*this)[j]);
        });

        apply_permutation(indices);
    }

private:

    void apply_permutation(const std::vector<size_t>& permutation)
    {
        std::vector<bool> visited(size(), false);

        for (size_t i = 0; i < size(); ++i) {
            if (!visited[i]) {
                size_t current = i;
                size_t next = permutation[i];

                while (next != i) {
                    swap_elements(current, next);
                    visited[current] = true;
                    current = next;
                    next = permutation[current];
                }
                visited[current] = true;
            }
        }
    }

    template <size_t ...Is, typename Tuple>
    constexpr void push_back_impl(Tuple&& x, std::index_sequence<Is...>)
    {
        (std::get<Is>(m_storage).push_back(std::get<Is>(std::forward<Tuple>(x))), ...);
    }

    template <size_t ...Is>
    constexpr auto at_impl(size_t i, std::index_sequence<Is...>)
    {
        return std::tie(std::get<Is>(m_storage)[i]...);
    }

    template <size_t ...Is>
    constexpr auto at_impl(size_t i, std::index_sequence<Is...>) const
    {
        return std::make_tuple(std::cref(std::get<Is>(m_storage)[i])...);
    }

    template <size_t ...Is>
    constexpr auto swap_elements_impl(size_t i, size_t j, std::index_sequence<Is...>)
    {
        return (std::swap(std::get<Is>(m_storage)[i], std::get<Is>(m_storage)[j]), ...);
    }

    template <size_t ...Is>
    constexpr auto resize_impl(size_t capacity, std::index_sequence<Is...>)
    {
        (std::get<Is>(m_storage).resize(capacity), ...);
    }

    template <size_t ...Is>
    constexpr auto reserve_impl(size_t capacity, std::index_sequence<Is...>)
    {
        (std::get<Is>(m_storage).reserve(capacity), ...);
    }

    template <size_t ...Is>
    constexpr auto clear_impl(std::index_sequence<Is...>)
    {
        (std::get<Is>(m_storage).clear(), ...);
    }

    std::tuple<std::vector<Types>...> m_storage;
};

}