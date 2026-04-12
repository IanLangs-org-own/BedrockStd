#pragma once
#include <cassert>
#include <iostream>
#include <cstdlib>
#ifdef NDEBUG
#define Vassert(cond, msg)  ((void)0)
#define unreachable         ((void)0)
#define assume(cond)        ((void)0)
#define panic()             ((void)0)
#define debugtime(code)     ((void)0)
#else

#define Vassert(cond, msg) \
    do { \
        if (!(cond)) { \
            std::cerr << "[ASSERT] " << msg << "\n" << \
                "file: " << __FILE__ << ":" << __LINE__ << "\n" << \
                "cond: " << #cond << "\n" << \
                "[ASSERT END]\n"; \
            exit(1); \
        } \
    } while (0)


#define unreachable \
    do { \
        std::cerr << "[UNREACHABLE]\n" << \
            "file: " << __FILE__ << ":" << __LINE__ << "\n" << \
            "[UNREACHABLE END]\n"; \
        exit(1); \
    } while(0)


#define panic() \
    do { \
        std::cerr << "[PANIC]\n" << \
            "file: " << __FILE__ << ":" << __LINE__ << "\n" << \
            "[PANIC END]\n"; \
        exit(1); \
    } while(0)


#define assume(cond) \
    do { \
       if(!(cond)) unreachable; \
    } while(0)


#define debugtime(code) do {code;} while(0)

#endif