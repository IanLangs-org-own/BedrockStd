#pragma once
#include <functional>
namespace bedrock {
    struct defer {
        std::function<void(void)> fn;
        defer(std::function<void(void)> f) : fn(f) {}
        ~defer() {
            fn();
        }
    };
}