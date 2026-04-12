#pragma once
#include "initializer_list.hpp"
#include "assert.hpp"
namespace bedrock {
    template <typename T, usize N>
    class Array {
        T data[N];
        public:
            Array(initializer_list<T> list) {
                Vassert(len(list) == N, "invalid length");
                usize i = 0;
                for (const auto& elem : list) {
                    data[i] = elem;
                }
            }
            usize size() const noexcept override {
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

            
    };
}