#pragma once
#include "types.hpp"
#include <concepts>
#include <iterator>
#include <type_traits>

namespace bedrock {
    namespace types {
        template <typename T>
        concept with_size = requires (T const& o) {
            { o.size() } -> std::convertible_to<usize>;
        };

        template <typename T>
        concept with_empty = requires (T const& o) {
            { o.empty() } -> std::convertible_to<bool>;
        };

        template <typename T>
        concept with_capacity = with_size<T> && requires (T const& o) {
            { o.capacity() } -> std::convertible_to<usize>;
        };

        template <typename T>
        concept iterable = requires (T& t) {
            { std::begin(t) } -> std::input_or_output_iterator;
            { std::end(t) }   -> std::sentinel_for<decltype(std::begin(t))>;
        };

        template <typename T>
        concept const_iterable = iterable<T const>;

        template <typename T>
        concept collection = iterable<T> && with_size<T>;

        constexpr usize len(with_size auto const& o) noexcept {
            return static_cast<usize>(o.size());
        }

        template<typename T, usize N>
        constexpr usize len(T (&)[N]) noexcept {
            return N;
        }
        constexpr bool is_empty(auto const& o) noexcept {
            if constexpr (with_empty<decltype(o)>) {
                return o.empty();
            } else if constexpr (with_size<decltype(o)>) {
                return o.size() == 0;
            } else if constexpr (iterable<decltype(o)>) {
                return std::begin(o) == std::end(o);
            } else {
                
            }
        }

        template <typename T, typename U>
        concept iterable_of = iterable<T> && 
            std::same_as<std::iter_value_t<decltype(std::begin(std::declval<T&>()))>, U>;
    }
}
