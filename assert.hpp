#pragma once
#include <cassert>
#include <iostream>
#include <cstdlib>
#ifdef NDEBUG
#define Vassert(cond, msg) ((void)0)
#define unreachable ((void)0)
#else
#define Vassert(cond, msg) \
    do { \
        if (!(cond)) { \
            std::cerr << "[ASSERT] " << msg << "\n" << \
                "file: " << __FILE__ << ":" << __LINE__ << "\n" << \
                "cond: " << #cond << "\n" << \
                "[ASSERT END]\n"; \
            std::abort(); \
        } \
    } while (0)

#define unreachable /*unreachable*/\
    do { \
        std::cerr << "[UNREACHABLE]\n" << \
            "file: " << __FILE__ << ":" << __LINE__ << "\n" << \
            "[UNREACHABLE END]\n"; \
        std::abort(); \
    } while(0)
#endif