#pragma once
#include <initializer_list>
#include "types.hpp"
namespace bedrock {
    template <typename T>
    class ArrayView : public withlength_t {
        private:
            T* data{nullptr};
            usize length{};
        public:
            ArrayView(std::initializer_list<T> list) {
                data = new T[list.size()];
                for (const auto& elem : list) {
                    data[length++] = elem;
                }
            }
            usize size() const {
                return length;
            }

            ~ArrayView() {
                delete[] data;
                data = nullptr; //para permitir copy sin ub
            }

            const T* const ptr() const {
                return data;
            }
    };
}