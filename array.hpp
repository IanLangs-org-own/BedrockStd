#pragma once
#include "initializer_list.hpp"
#include "assert.hpp"
#include "concepts.hpp"
namespace bedrock {
    namespace collections {
        template <typename T, ::bedrock::types::usize N>
        class Array {
            T data[N];
            public:
                Array() {
                    for (::bedrock::types::usize i = 0; i < N; i++)
                        data[i] = T{};
                }

                Array(::bedrock::args::initializer_list<T> list) {
                    Vassert(::bedrock::types::len(list) == N, "invalid length");
                    ::bedrock::types::usize i = 0;
                    for (const auto& elem : list) {
                        data[i++] = elem;
                    }
                }
                ::bedrock::types::usize size() const noexcept {
                    return N;
                }

                const T* begin() const noexcept {
                    return data;
                }

                const T* end() const noexcept {
                    return data+N;
                }

                T* begin() noexcept {
                    return data;
                }

                T* end() noexcept {
                    return data+N;
                }

                T& operator[](::bedrock::types::usize i) {
                    Vassert(i < N, "invalid index");
                    return data[i];
                }

                const T& operator[](::bedrock::types::usize i) const {
                    Vassert(i < N, "invalid index");
                    return data[i];
                }

                constexpr bool operator==(const Array& other) const noexcept {
                    for (::bedrock::types::usize i = 0; i < N; i++)
                        if ((*this)[i] != other[i])
                            return false;
                    return true;

                }
                
        };
    }
}