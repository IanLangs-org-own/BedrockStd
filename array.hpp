#pragma once
#include "initializer_list.hpp"


#include "assert.hpp"
namespace bedrock {
    template <typename T, usize N>
    class Array {
        T data[N];
        public:
            Array() {
                for (usize i = 0; i < N; i++)
                    data[i] = T{};
            }

            Array(initializer_list<T> list) {
                Vassert(len(list) == N, "invalid length");
                usize i = 0;
                for (const auto& elem : list) {
                    data[i++] = elem;
                }
            }
            usize size() const noexcept {
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

            T& operator[](usize i) {
                Vassert(i < N, "invalid index");
                return data[i];
            }

            const T& operator[](usize i) const {
                Vassert(i < N, "invalid index");
                return data[i];
            }

            constexpr bool operator==(const Array& other) const noexcept {
                for (usize i = 0; i < N; i++)
                    if ((*this)[i] != other[i])
                        return false;
                return true;

            }
            
    };
}