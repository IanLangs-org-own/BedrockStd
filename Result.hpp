#pragma once
#include "assert.hpp"
namespace bedrock {
    namespace types {
        template <typename Ok, typename Err = void>
        class Result;

        template <typename Ok>
        class Result<Ok, void> {
            Ok value;
            bool is_ok;
            public:
                Result() : is_ok(false) {}
                Result(const Ok& val) : value(val), is_ok(true) {}
                Result(Ok&& val) : value(std::move(val)), is_ok(true) {}
                operator bool() const { return is_ok; }
                Result& operator=(const Ok& val) { value = val; is_ok = true; return *this; }
                Result& operator=(Ok&& val) { value = std::move(val); is_ok = true; return *this; }
                Result& seterr() { is_ok = false; return *this; }
                Ok& unwrap() { if (!is_ok) panic();return value; }
                const Ok& unwrap() const { if (!is_ok) panic(); return value; }
        };

        template <typename Ok, typename Err>
        class Result {
            Ok value;
            Err error;
            bool is_ok;
            public:
                Result(const Ok& val) : value(val), is_ok(true) {}
                Result(Ok&& val) : value(std::move(val)), is_ok(true) {}
                Result(const Err& err) : error(err), is_ok(false) {}
                Result(Err&& err) : error(std::move(err)), is_ok(false) {}
                operator bool() const { return is_ok; }
                Result& operator=(const Ok& val) { value = val; is_ok = true; return *this; }
                Result& operator=(Ok&& val) { value = std::move(val); is_ok = true; return *this; }
                Result& operator=(const Err& err) { error = err; is_ok = false; return *this; }
                Result& operator=(Err&& err) { error = std::move(err); is_ok = false; return *this; }
                Ok& unwrap() { if (!is_ok) panic();return value; }
                const Ok& unwrap() const { if (!is_ok) panic(); return value; }
                Err& unwrap_err() { if (is_ok) panic(); return error; }
                const Err& unwrap_err() const { if (is_ok) panic(); return error; }
        };
    } 
}