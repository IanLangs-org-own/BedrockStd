#pragma once
#include <utility>

namespace bedrock {
    namespace types {

        template <typename T>
        class reference_t {
            T* ref = nullptr;

        public:
            reference_t(T& o) : ref(&o) {}
            reference_t(T&&) = delete;

            constexpr T* operator->() const noexcept { return ref; }
            constexpr T& operator*() const noexcept { return *ref; }

            constexpr T* get_ptr() const noexcept { return ref; }
            constexpr T& get() const noexcept { return *ref; }

            constexpr operator T&() const noexcept { return *ref; }

            template<class... Args>
            constexpr decltype(auto) operator()(Args&&... args) const {
                return (*ref)(std::forward<Args>(args)...);
            }
        };

    } // namespace types

    template <typename T>
    types::reference_t<T> ref(T& obj) {
        return types::reference_t<T>(obj);
    }
}