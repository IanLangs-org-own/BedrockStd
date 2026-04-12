#pragma once
#include <utility>
namespace bedrock {
    using std::move;
    using std::forward;
    template <typename T>
    using rvalue_ref = T&&;
    template <typename T>
    using lvalue_ref = T&;
    template <typename T>
    using remove_ref = std::remove_reference_t<T>;
    template <typename T>
    using decay = std::decay_t<T>;
}