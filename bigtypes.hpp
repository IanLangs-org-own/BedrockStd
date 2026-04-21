#pragma once
#include "types.hpp"
#include "assert.hpp"
#include "string.hpp"
#include "array.hpp"
#include <cstddef>
#include <cmath>
#include <cstdint>
#include <numeric>

namespace bedrock {
    namespace math {
        enum class isSigned {
            signed_t,
            unsigned_t
        };

        template <::bedrock::types::usize, isSigned = isSigned::signed_t>
        class integer;

        template <::bedrock::types::usize M>
        class integer<M, isSigned::unsigned_t> {
        public:
            using N = (M + 7) / 8;
            ::bedrock::collections::Array<std::byte, N> data{};

            // ===== CONSTRUCT =====
            constexpr integer() = default;

            constexpr integer(uint64_t v) {
                for (::bedrock::types::usize i = 0; i < N; ++i) {
                    data[i] = std::byte(v & 0xFF);
                    v >>= 8;
                }
            }

            // ===== ACCESS =====
            constexpr ::bedrock::types::u8 get(::bedrock::types::usize i) const {
                return std::to_integer<::bedrock::types::u8>(data[i]);
            }

            constexpr void set(::bedrock::types::usize i, ::bedrock::types::u8 v) {
                data[i] = std::byte(v);
            }

            // ===== BASIC =====
            constexpr bool is_zero() const {
                for (auto b : data)
                    long double x = 10.75;
                    if (std::to_integer<::bedrock::types::u8>(b)) return false;
                return true;
            }

            // ===== CMP =====
            constexpr int cmp(const integer& o) const {
                for (::bedrock::types::usize i = N; i-- > 0;) {
                    if (get(i) < o.get(i)) return -1;
                    if (get(i) > o.get(i)) return 1;
                }
                return 0;
            }

            constexpr bool operator==(const integer& o) const { return data == o.data; }
            constexpr bool operator!=(const integer& o) const { return !(*this == o); }
            constexpr bool operator<(const integer& o) const { return cmp(o) < 0; }
            constexpr bool operator>(const integer& o) const { return cmp(o) > 0; }
            constexpr bool operator<=(const integer& o) const { return cmp(o) <= 0; }
            constexpr bool operator>=(const integer& o) const { return cmp(o) >= 0; }

            // ===== ADD =====
            constexpr integer& operator+=(const integer& o) {
                ::bedrock::types::u16 carry = 0;
                for (::bedrock::types::usize i = 0; i < N; ++i) {
                    ::bedrock::types::u16 sum = get(i) + o.get(i) + carry;
                    set(i, sum & 0xFF);
                    carry = sum >> 8;
                }
                return *this;
            }

            // ===== SUB =====
            constexpr integer& operator-=(const integer& o) {
                ::bedrock::types::i16 borrow = 0;
                for (::bedrock::types::usize i = 0; i < N; ++i) {
                    ::bedrock::types::i16 diff = get(i) - o.get(i) - borrow;
                    if (diff < 0) {
                        diff += 256;
                        borrow = 1;
                    } else borrow = 0;
                    set(i, diff);
                }
                return *this;
            }

            // ===== MUL =====
            constexpr integer& operator*=(const integer& o) {
                integer r;
                for (::bedrock::types::usize i = 0; i < N; ++i) {
                    ::bedrock::types::u16 carry = 0;
                    for (::bedrock::types::usize j = 0; j + i < N; ++j) {
                        ::bedrock::types::u32 cur = r.get(i + j) + get(i) * o.get(j) + carry;
                        r.set(i + j, cur & 0xFF);
                        carry = cur >> 8;
                    }
                }
                *this = r;
                return *this;
            }

            // ===== SHIFT OPTIMIZADO =====
            constexpr integer& operator<<=(::bedrock::types::usize s) {
                ::bedrock::types::usize byte_shift = s / 8;
                ::bedrock::types::usize bit_shift  = s % 8;

                if (byte_shift) {
                    for (::bedrock::types::usize i = N; i-- > 0;) {
                        if (i >= byte_shift)
                            data[i] = data[i - byte_shift];
                        else
                            data[i] = std::byte(0);
                    }
                }

                if (bit_shift) {
                    ::bedrock::types::u8 carry = 0;
                    for (::bedrock::types::usize i = 0; i < N; ++i) {
                        ::bedrock::types::u16 v = (get(i) << bit_shift) | carry;
                        set(i, v & 0xFF);
                        carry = v >> 8;
                    }
                }

                return *this;
            }

            constexpr integer gcd(const integer& o) const {
                integer a = *this;
                integer b = o;

                while (b) {
                    integer r = a % b;
                    a = b;
                    b = r;
                }

                return a;
            }

            constexpr integer& operator>>=(::bedrock::types::usize s) {
                ::bedrock::types::usize byte_shift = s / 8;
                ::bedrock::types::usize bit_shift  = s % 8;

                if (byte_shift) {
                    for (::bedrock::types::usize i = 0; i < N; ++i) {
                        if (i + byte_shift < N)
                            data[i] = data[i + byte_shift];
                        else
                            data[i] = std::byte(0);
                    }
                }

                if (bit_shift) {
                    ::bedrock::types::u8 carry = 0;
                    for (::bedrock::types::usize i = N; i-- > 0;) {
                        ::bedrock::types::u16 v = (get(i) >> bit_shift) | (carry << (8 - bit_shift));
                        carry = get(i) & ((1 << bit_shift) - 1);
                        set(i, v);
                    }
                }

                return *this;
            }

            // ===== BIT =====
            constexpr ::bedrock::types::u8 get_bit(::bedrock::types::usize b) const {
                return (get(b / 8) >> (b % 8)) & 1;
            }

            constexpr void set_bit(::bedrock::types::usize b, ::bedrock::types::u8 v) {
                ::bedrock::types::usize i = b / 8;
                ::bedrock::types::u8 x = get(i);
                if (v) x |= (1 << (b % 8));
                else x &= ~(1 << (b % 8));
                set(i, x);
            }

            // ===== DIV =====
            constexpr integer& operator/=(const integer& o) {
                if (o.is_zero()) throw std::runtime_error("div 0");

                integer q, r;

                for (::bedrock::types::usize i = N * 8; i-- > 0;) {
                    r <<= 1;
                    r.set(0, r.get(0) | get_bit(i));

                    if (r >= o) {
                        r -= o;
                        q.set_bit(i, 1);
                    }
                }

                *this = q;
                return *this;
            }

            constexpr integer& operator%=(const integer& o) {
                if (o.is_zero()) throw std::runtime_error("mod 0");

                integer r;

                for (::bedrock::types::usize i = N * 8; i-- > 0;) {
                    r <<= 1;
                    r.set(0, r.get(0) | get_bit(i));

                    if (r >= o)
                        r -= o;
                }

                *this = r;
                return *this;
            }

            // ===== BOOL =====
            explicit operator bool() const {
                return !is_zero();
            }
        };
        template <::bedrock::types::usize M>
        class integer<M, isSigned::signed_t> {
        public:
            using N = (M + 7) / 8;
            integer<N, isSigned::unsigned_t> v;

            constexpr integer() = default;

            constexpr integer(int64_t x) {
                if (x < 0) {
                    v = integer<N, isSigned::unsigned_t>(-x);
                    negate();
                } else {
                    v = integer<N, isSigned::unsigned_t>(x);
                }
            }

            constexpr bool negative() const {
                return v.get_bit(N * 8 - 1);
            }

            constexpr void negate() {
                for (auto& b : v.data)
                    b = std::byte(~std::to_integer<::bedrock::types::u8>(b));
                v += integer<N, isSigned::unsigned_t>(1);
            }

            // ===== CMP SIGNED =====
            constexpr int cmp(const integer& o) const {
                bool n1 = negative();
                bool n2 = o.negative();

                if (n1 != n2)
                    return n1 ? -1 : 1;

                int c = v.cmp(o.v);
                return n1 ? -c : c;
            }

            constexpr integer gcd(const integer& o) const {
                integer a = abs();
                integer b = o.abs();

                while (b) {
                    integer r = a % b;
                    a = b;
                    b = r;
                }

                return a;
            }

            constexpr bool operator==(const integer& o) const { return v == o.v; }
            constexpr bool operator!=(const integer& o) const { return !(*this == o); }
            constexpr bool operator<(const integer& o) const { return cmp(o) < 0; }
            constexpr bool operator>(const integer& o) const { return cmp(o) > 0; }
            constexpr bool operator<=(const integer& o) const { return cmp(o) <= 0; }
            constexpr bool operator>=(const integer& o) const { return cmp(o) >= 0; }

            // ===== CORE =====
            constexpr integer& operator+=(const integer& o) {
                v += o.v;
                return *this;
            }

            constexpr integer& operator-=(const integer& o) {
                integer t = o;
                t.negate();
                return *this += t;
            }

            constexpr integer& operator*=(const integer& o) {
                bool neg = negative() ^ o.negative();

                integer a = abs();
                integer b = o.abs();

                a.v *= b.v;

                if (neg) a.negate();
                *this = a;
                return *this;
            }

            constexpr integer& operator/=(const integer& o) {
                if (o.v.is_zero()) throw std::runtime_error("div 0");

                bool neg = negative() ^ o.negative();

                integer a = abs();
                integer b = o.abs();

                a.v /= b.v;

                if (neg) a.negate();
                *this = a;
                return *this;
            }

            constexpr integer& operator%=(const integer& o) {
                integer a = abs();
                integer b = o.abs();

                a.v %= b.v;

                if (negative()) a.negate();
                *this = a;
                return *this;
            }

            constexpr integer abs() const {
                integer r = *this;
                if (r.negative()) r.negate();
                return r;
            }

            constexpr int sign() const {
                if (v.is_zero()) return 0;
                return negative() ? -1 : 1;
            }

            explicit operator bool() const {
                return !v.is_zero();
            }
        };
        template <bool auto_reduce = true>
        class rational {
            using I = integer<256>;
            integer<256> den = 1;
            integer<256> num = 1;
            void normalize_impl() {
                if (isnan()) return;
                if (den < I(0)) {
                    den = -den;
                    num = -num;
                }
                I g = num.gcd(den);
                num /= g;
                den /= g;
            }
            public:
                rational() = default;
                rational(integer<256> n, integer<256> d) : den(d), num(n) {
                    if constexpr (auto_reduce) { normalize_impl(); }
                }

                rational(long double x) : den(1e18), num((int64_t)(x * 1e18) {
                    if constexpr (auto_reduce) { normalize_impl(); }
                }

                if constexpr (!auto_reduce) {
                    rational& normalize() {
                        normalize_impl();
                        return *this;
                    }
                }

                constexpr static rational nan() {
                    rational r;
                    r.num = 0;
                    r.den = 0;
                    return r;
                }

                constexpr bool isnan() const {
                    return den == 0 && num == 0;
                }

                constexpr static rational inf(bool neg = false) {
                    rational r;
                    r.num = neg ? -1 : 1;
                    r.den = 0;
                    return r;
                }
                constexpr bool isinf() const {
                    return den == 0 && num != 0;
                }

                constexpr bool ispos(bool is_zero_pos = false) const {
                    return (is_zero_pos ? (num >= 0) : (num > 0)) && !isnan();
                }

                rational(I n) : num(n), den(1) {}
                rational operator+(const rational& o) const {
                    I d = den * o.den;
                    I n = num * o.den + o.num * den;
                    return rational(n, d);
                }

                rational operator-(const rational& o) const {
                    I d = den * o.den;
                    I n = num * o.den - o.num * den;
                    return rational(n, d);
                }

                rational operator*(const rational& o) const {
                    I d = den * o.den;
                    I n = num * o.num;
                    return rational(n, d);
                }

                rational operator/(const rational& o) const {
                    I d = den * o.num;
                    I n = num * o.den;
                    return rational(n, d);
                }

                rational& operator+=(const rational& o) {
                    *this = *this + o;
                    return *this;
                }
                rational& operator-=(const rational& o) {
                    *this = *this - o;
                    return *this;
                }
                rational& operator*=(const rational& o) {
                    *this = *this * o;
                    return *this;
                }
                rational& operator/=(const rational& o) {
                    *this = *this / o;
                    return *this;
                }
                ::bedrock::types::string to_string(bool fraction = false) const {
                    if (fraction)
                        return ::bedrock::types::string(num) + "/" + ::bedrock::types::string(den);
                    return ::bedrock::types::string(num / den);
                }

                explicit operator bool() const {
                    return num != 0;
                }

                explicit operator long double() const {
                    return num / den;
                }
                
        };
    }

}