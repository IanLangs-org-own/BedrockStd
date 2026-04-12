#pragma once
#include "types.hpp"
#include <initializer_list>
#include <new>      
#include <utility> 
namespace bedrock {

    enum class VArrayError {
        None,
        OutOfMemory,
        OutOfBounds
    };

    template <typename T>
    struct VArray {
        public:
            VArray() = default;

            explicit VArray(usize N) {
                init(N);
            }

            VArray(std::initializer_list<T> list) {
                init(list);
            }

            VArray(const VArray& other) {
                copy_from(other);
            }

            VArray(VArray&& other) noexcept {
                move_from(std::move(other));
            }

            ~VArray() {
                free();
            }

            bool init(usize N) {
                free();

                if (N == 0) {
                    err = VArrayError::None;
                    return false;
                }

                data = new (std::nothrow) T[N]{};
                if (!data) {
                    err = VArrayError::OutOfMemory;
                    return false;
                }

                length = N;
                err = VArrayError::None;
                return true;
            }

            bool init(std::initializer_list<T> list) {
                free();

                usize N = list.size();
                if (N == 0) {
                    err = VArrayError::None;
                    return true;
                }

                data = new (std::nothrow) T[N];
                if (!data) {
                    err = VArrayError::OutOfMemory;
                    return false;
                }

                usize i = 0;
                for (const T& e : list)
                    data[i++] = e;

                length = N;
                err = VArrayError::None;
                return true;
            }

            VArray& operator=(const VArray& other) {
                if (this != &other) copy_from(other);
                return *this;
            }

            VArray& operator=(VArray&& other) noexcept {
                if (this != &other) {
                    free();
                    move_from(std::move(other));
                }
                return *this;
            }

            bool get(usize i, T& out) const {
                if (i >= length) {
                    err = VArrayError::OutOfBounds;
                    return false;
                }
                out = data[i];
                return true;
            }

            T& operator[](usize i) { return data[i]; }
            const T& operator[](usize i) const { return data[i]; }

            usize size() const { return length; }
            bool empty() const { return length == 0; }

            VArrayError error() const { return err; }
            void clear_error() const { err = VArrayError::None; }

            void free() {
                delete[] data;
                data = nullptr;
                length = 0;
                err = VArrayError::None;
            }

        private:
            bool copy_from(const VArray& other) {
                T* new_data = nullptr;

                if (other.length != 0) {
                    new_data = new (std::nothrow) T[other.length];
                    if (!new_data) {
                        err = VArrayError::OutOfMemory;
                        return false;
                    }

                    for (usize i = 0; i < other.length; i++)
                        new_data[i] = other.data[i];
                }

                delete[] data;
                data = new_data;
                length = other.length;
                err = VArrayError::None;
                return true;
            }

            void move_from(VArray&& other) {
                data = other.data;
                length = other.length;
                err = other.err;

                other.data = nullptr;
                other.length = 0;
                other.err = VArrayError::None;
            }

        private:
            T* data = nullptr;
            usize length = 0;
            mutable VArrayError err = VArrayError::None;
    };
    template <typename T>
    constexpr usize len(const VArray<T>& arr) noexcept {
        return arr.size();
    }
}