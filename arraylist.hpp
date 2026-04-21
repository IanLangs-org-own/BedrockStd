#pragma once
#include "types.hpp"
#include "initializer_list.hpp"
#include "Result.hpp"
#include "reference.hpp"
#include "assert.hpp"

#include <utility>
#include <new>
#include <cstddef>

namespace bedrock {
    namespace collections {

        template <typename T>
        class ArrayList {
            size_t capacity = 0;
            size_t length = 0;
            T* data = nullptr;

            private:

                T* allocate(size_t cap) {
                    return static_cast<T*>(::operator new(sizeof(T) * cap));
                }

                void destroy_all() noexcept {
                    if (!data) return;

                    for (size_t i = 0; i < length; i++) {
                        data[i].~T();
                    }

                    length = 0;
                }

                void deallocate() noexcept {
                    if (data) {
                        ::operator delete(data);
                        data = nullptr;
                    }

                    capacity = 0;
                }

                void grow() {
                    size_t new_capacity = capacity ? capacity * 2 : 4;
                    reserve(new_capacity);
                }

                void ensure_capacity() {
                    if (length >= capacity) {
                        grow();
                    }
                }

            public:

                ArrayList() {
                    capacity = 4;
                    length = 0;
                    data = allocate(capacity);
                }

                ArrayList(::bedrock::args::initializer_list<T> init) {
                    capacity = init.size();
                    length = 0;
                    data = allocate(capacity);

                    for (const T& value : init) {
                        new (data + length) T(value);
                        length++;
                    }
                }

                ~ArrayList() {
                    clear();
                    deallocate();
                }

                ArrayList(const ArrayList& other) {
                    capacity = other.capacity;
                    length = other.length;

                    data = allocate(capacity);

                    for (size_t i = 0; i < length; i++) {
                        new (data + i) T(other.data[i]);
                    }
                }

                ArrayList(ArrayList&& other) noexcept {
                    capacity = other.capacity;
                    length = other.length;
                    data = other.data;

                    other.data = nullptr;
                    other.capacity = 0;
                    other.length = 0;
                }

                ArrayList& operator=(const ArrayList& other) {
                    if (this == &other) return *this;

                    clear();
                    deallocate();

                    capacity = other.capacity;
                    length = other.length;
                    data = allocate(capacity);

                    for (size_t i = 0; i < length; i++) {
                        new (data + i) T(other.data[i]);
                    }

                    return *this;
                }

                ArrayList& operator=(ArrayList&& other) noexcept {
                    if (this == &other) return *this;

                    clear();
                    deallocate();

                    capacity = other.capacity;
                    length = other.length;
                    data = other.data;

                    other.data = nullptr;
                    other.capacity = 0;
                    other.length = 0;

                    return *this;
                }

                size_t size() const noexcept {
                    return length;
                }

                size_t get_capacity() const noexcept {
                    return capacity;
                }

                bool empty() const noexcept {
                    return length == 0;
                }

                void reserve(size_t new_capacity) {
                    if (new_capacity <= capacity) return;

                    T* new_data = allocate(new_capacity);

                    size_t i = 0;

                    try {
                        for (; i < length; i++) {
                            new (new_data + i) T(std::move(data[i]));
                        }
                    }
                    catch (...) {
                        for (size_t j = 0; j < i; j++) {
                            new_data[j].~T();
                        }

                        ::operator delete(new_data);
                        throw;
                    }

                    destroy_all();
                    deallocate();

                    data = new_data;
                    capacity = new_capacity;
                }

                void clear() noexcept {
                    destroy_all();
                }

                void append(const T& value) {
                    ensure_capacity();
                    new (data + length) T(value);
                    length++;
                }

                void append(T&& value) {
                    ensure_capacity();
                    new (data + length) T(std::move(value));
                    length++;
                }

                template <typename... Args>
                T& emplace(Args&&... args) {
                    ensure_capacity();

                    T* ptr = new (data + length) T(std::forward<Args>(args)...);
                    length++;

                    return *ptr;
                }

                void remove(size_t index) {
                    Vassert(index < length, "invalid index");

                    data[index].~T();

                    for (size_t i = index; i + 1 < length; i++) {
                        new (data + i) T(std::move(data[i + 1]));
                        data[i + 1].~T();
                    }

                    length--;
                }

                T& operator[](size_t index) {
                    Vassert(index < length, "invalid index");
                    return data[index];
                }

                const T& operator[](size_t index) const {
                    Vassert(index < length, "invalid index");
                    return data[index];
                }

                T& front() {
                    Vassert(length > 0, "empty list");
                    return data[0];
                }

                T& back() {
                    Vassert(length > 0, "empty list");
                    return data[length - 1];
                }

                const T& front() const {
                    Vassert(length > 0, "empty list");
                    return data[0];
                }

                const T& back() const {
                    Vassert(length > 0, "empty list");
                    return data[length - 1];
                }

                T* begin() noexcept {
                    return data;
                }

                const T* begin() const noexcept {
                    return data;
                }

                T* end() noexcept {
                    return data + length;
                }

                const T* end() const noexcept {
                    return data + length;
                }

                ::bedrock::types::Result<
                    ::bedrock::types::reference_t<T>, void
                >
                at(size_t index) {
                    if (index >= length) return {};
                    return ::bedrock::ref(data[index]);
                }

                ::bedrock::types::Result<
                    ::bedrock::types::reference_t<const T>, void
                >
                at(size_t index) const {
                    if (index >= length) return {};
                    return ::bedrock::ref<const T>(data[index]);
                }
        };

    }
}