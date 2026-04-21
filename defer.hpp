#pragma once
#include <functional>
#include "assert.hpp"
namespace bedrock {
    namespace types {
        template <class F>
        class defer {
            public:
                explicit defer(F f) : fn(std::move(f)), active(true) {}

                defer(defer&& other) noexcept
                    : fn(std::move(other.fn)), active(other.active) {
                    other.active = false;
                }

                defer(const defer&) = delete;
                defer& operator=(const defer&) = delete;

                void cancel() noexcept {
                    active = false;
                }

                void operator()() {
                    if (active) {
                        fn();
                        active = false;
                    }
                }

                ~defer() noexcept {
                    if (active) fn();
                }

            private:
                F fn;
                bool active;
        };
    }
}