#pragma once
#include "types.hpp"
#include "varray.hpp"
#include "assert.hpp"
#include <initializer_list>
#include <new>      
#include <utility> 
namespace bedrock {

    enum class ArrayError {
        None,
        OutOfMemory,
        OutOfBounds,
        LengthError
    };

   template <typename T, usize N>
    struct Array {
        static_assert(N != 0, "no se permite array de 0 elems");
        mutable T data[N];
        public:
            Array() : data({}) {}
            Array(T& val) : data({val}) {}
            Array(std::initializer_list<T> list) {
                Vassert(list.size() == N, "lista debe tener el tamaño del arr");
                usize i = 0;
                for (const T& elem : list) {
                    data[i++] = elem;
                }
            }
            Array(const Array& other) {
                std::copy(std::begin(other.data), std::end(other.data), std::begin(data));
            }

            Array(Array&& other) {
                std::copy(std::begin(other.data), std::end(other.data), std::begin(data));
                std::fill(std::begin(other.data), std::end(other.data), T{});
            }


            void init() {
                for(T& elem : data) {
                    elem = T{};
                }
            }

            void init(T& val) {
                for(T& elem : data) {
                    elem = val;
                }
            }

            void init(const std::initializer_list<T>& list) {
                Vassert(list.size() == N, "lista debe tener el tamaño del arr");
                usize i = 0;
                for (const T& elem : list) {
                    data[i++] = elem;
                }
            }

            Array& operator=(const std::initializer_list<T>& list) {
                init(list);
                return *this;
            }

            Array& operator=(Array&& other) {
                std::copy(std::begin(other.data), std::end(other.data), std::begin(data));
                std::fill(std::begin(other.data), std::end(other.data), T{});
                return *this;
            }


            usize size() const {
                return N;
            }

            T& operator[](usize i) { Vassert(i < N, "index error"); return data[i]; }
            const T& operator[](usize i) const { Vassert(i < N, "index error"); return data[i]; }

    }; 

    template <typename T, usize N>
    constexpr usize len(const Array<T, N>& arr) noexcept {
        return arr.size();
    }
    


}