#pragma once
#include <optional>

namespace bedrock {
    struct None_t {};

    None_t None;

    template <typename T>
    class optional {
        std::optional<T> value = std::nullopt;
        public:
            optional(T &o) : value(o) {}
            optional() {}
            optional& operator=(T &o) {
                value = o;
                return *this;
            }
            optional& operator=(None_t &) {
                value = std::nullopt;
                return *this;
            }

            constexpr operator bool() const noexcept {
                return value.has_value();
            }

            T& operator*() {
                return *value;
            }
            T* operator->() {
                return value.operator->();
            }
            const T& operator*() const {
                return *value;
            }
            const T* const operator->() const {
                return value.operator->();
            }
    };
}