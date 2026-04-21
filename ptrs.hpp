#pragma once
#include <memory>
#include <iostream>
#include "assert.hpp"
#include "types.hpp"

namespace bedrock {
    namespace ptrs {
        using std::unique_ptr, std::weak_ptr, std::shared_ptr;

        template <typename T = void>
        void default_deleter_fn(T* p) {
            delete p;
        }

        template <typename T = void, auto deleterfn = default_deleter_fn<T>>
        struct ptr_deleter {
            void operator()(T* p) {
                deleterfn(p);
            }
        };

    
        template <typename T = void>
        using default_deleter = ptr_deleter<T>;

        template <typename T = void>
        void array_deleter_fn(T* p) {
            delete[] p;
        }
        template <typename T = void>
        using array_deleter = ptr_deleter<T, array_deleter_fn<T>>;

        template <typename T>
        class observer_ptr;

        template <typename T, typename deleter = default_deleter<T>>
        struct manual_ptr {
            manual_ptr(T* p = nullptr) : data(p) {}
            manual_ptr(const manual_ptr& p) = delete;
            
            void free() {
                if (data) {
                    deleter{}(data);
                    data = nullptr;
                }
            }

            void reset() {
                free();
            }

            ~manual_ptr() {
                if (data != nullptr) std::cout << "unreleased ptr";
            }

            observer_ptr<T> observe() const {
                return observer_ptr<T>{data};
            }

            T* cptr() const noexcept {
                return data;
            }

            manual_ptr& operator=(T* p) {
                if (p != data) {
                    data = p;
                }
                return *this;
            }

            manual_ptr& operator=(const manual_ptr& p) = delete;

            T& operator*() const {
                return *data;
            }
            T* operator->() const {
                return data;
            }

            operator bool() const noexcept {
                return data != nullptr;
            }

            private:
                T* data{nullptr};
        };

        template <typename T>
        class observer_ptr {
            T* data{nullptr};
            public:
                observer_ptr() = default;
                observer_ptr(T* p) : data(p) {}
                observer_ptr(const manual_ptr<T>& p) {
                    data = p.cptr();
                }
                T* cptr() const { return data; }

                observer_ptr& operator=(T* p) {
                    data = p;
                    return *this;
                }

                observer_ptr& operator=(const manual_ptr<T>& p) {
                    data = p.cptr();
                    return *this;
                }

                T& operator*() const { Vassert(data != nullptr, "the ptr are nullptr"); return *data; }
                T* operator->() const  { Vassert(data != nullptr, "the ptr are nullptr"); return data; }

                operator bool() const { return data != nullptr; }
        };
    }
}