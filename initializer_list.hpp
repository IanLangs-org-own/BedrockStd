#pragma once
#include <initializer_list>
#include "types.hpp"
namespace bedrock {
    template <typename T>
    class initializer_list : public withlength_t {
        private:
            T* data{nullptr};
            usize length{};
        public:
            initializer_list(std::initializer_list<T> list) {
                data = new T[list.size()];
                for (const auto& elem : list) {
                    data[length++] = elem;
                }
            }

            initializer_list(const initializer_list& list) {
                data = new T[list.length];
                length = list.length;
                for (usize i = 0; i < list.length; ++i) {
                    data[i] = list.data[i];
                }
            }

            initializer_list(initializer_list&& list) {
                data = list.data;
                length = list.length;
                list.data = nullptr;
                list.length = 0;
            }

            template <typename T>
            initializer_list& operator=(T&&) = delete;

            usize size() const noexcept override {
                return length;
            }

            ~initializer_list() {
                delete[] data;
                data = nullptr; 
            }

            const T* const ptr() const {
                return data;
            }
    };
}