#pragma once
#include <utility>
namespace bedrock {
    template <typename T>
    class reference_t {
        private:
            T *ref = nullptr;
        public:
            reference_t(T& o) : ref(&o) {}
            reference_t(T&&) = delete;
            constexpr T* operator->() const noexcept {
                return ref;
            }

            constexpr T& operator*() const noexcept {
                return *ref;
            }

            constexpr T* get_ptr() const noexcept {
                return ref;
            }

            constexpr T& get() const noexcept {
                return *ref;
            }

            constexpr operator T&() const noexcept {
                return *ref;
            }

            template<class... Args>
            constexpr decltype(auto) operator()(Args&&... args) const noexcept(noexcept((*ref)(std::forward<Args>(args)...))) {
                return (*ref)(std::forward<Args>(args)...);
            }
    };
    
}