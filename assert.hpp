#pragma once
#include <iostream>
#include <cstdlib>

#ifdef NDEBUG
#define Vassert(cond, msg)  ((void)0)
#define Unreachable()       ((void)0)
#define assume(cond)        ((void)0)
#define panic()             ((void)0)
#define debugtime(...)      ((void)0)
#define Assert(cond,msg)    ((void)0)
#else

#define Vassert(cond, msg) \
    do { \
        if (!(cond)) { \
            std::cerr << "[VASSERT] " << msg << "\n" << \
                "file: " << __FILE__ << ":" << __LINE__ << "\n" << \
                "cond: " << #cond << "\n" << \
                "[VASSERT END]\n"; \
            exit(1); \
        } \
    } while (0)

#define Assert(cond, msg) static_assert(cond, msg)

#define ToDO() \
    Assert(false, "ToDo not resolved") 

#define Unreachable() \
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
       if(!(cond)) Unreachable(); \
    } while(0)


#define debugtime(...) do {__VA_ARGS__;} while(0)

#endif