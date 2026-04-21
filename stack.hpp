#pragma once
#include "arraylist.hpp"
#include <utility>
namespace bedrock::collections {
    template <typename T>
    class Stack {
        ArrayList<T> data;
        public:
            Stack() = default;
            Stack(::bedrock::args::initializer_list<T> init) : data(init) {}
            void push(const T& value) { data.append(value); }
            void push(T&& value) { data.append(std::move(value)); }
            template <typename... Args>
            void emplace(Args&&... args) { data.emplace(std::forward<Args>(args)...); }
            T pop() { T val = std::move(data[data.size() - 1]); data.remove(data.size() - 1); return val;}
            T& top() { return data[data.size() - 1]; }
            const T& top() const { return data[data.size() - 1]; }
            size_t size() const { return data.size(); }
    };
}