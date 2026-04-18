#pragma once
#include "types.hpp"
#include "assert.hpp"
#include "array.hpp"
#include <cstddef>
#include <cstdint>
#include <stdexcept>

namespace bedrock {

    enum class isSigned {
        signed_t,
        unsigned_t
    };

    template <usize, isSigned>
    class integer;

    template <usize N>
    class integer<N, isSigned::unsigned_t> {
    public:
        Array<std::byte, N> data{};

        // ===== CONSTRUCT =====
        constexpr integer() = default;

        constexpr integer(uint64_t v) {
            for (usize i = 0; i < N; ++i) {
                data[i] = std::byte(v & 0xFF);
                v >>= 8;
            }
        }

        // ===== ACCESS =====
        constexpr u8 get(usize i) const {
            return std::to_integer<u8>(data[i]);
        }

        constexpr void set(usize i, u8 v) {
            data[i] = std::byte(v);
        }

        // ===== BASIC =====
        constexpr bool is_zero() const {
            for (auto b : data)
                if (std::to_integer<u8>(b)) return false;
            return true;
        }

        // ===== CMP =====
        constexpr int cmp(const integer& o) const {
            for (usize i = N; i-- > 0;) {
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
            u16 carry = 0;
            for (usize i = 0; i < N; ++i) {
                u16 sum = get(i) + o.get(i) + carry;
                set(i, sum & 0xFF);
                carry = sum >> 8;
            }
            return *this;
        }

        // ===== SUB =====
        constexpr integer& operator-=(const integer& o) {
            i16 borrow = 0;
            for (usize i = 0; i < N; ++i) {
                i16 diff = get(i) - o.get(i) - borrow;
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
            for (usize i = 0; i < N; ++i) {
                u16 carry = 0;
                for (usize j = 0; j + i < N; ++j) {
                    u32 cur = r.get(i + j) + get(i) * o.get(j) + carry;
                    r.set(i + j, cur & 0xFF);
                    carry = cur >> 8;
                }
            }
            *this = r;
            return *this;
        }

        // ===== SHIFT OPTIMIZADO =====
        constexpr integer& operator<<=(usize s) {
            usize byte_shift = s / 8;
            usize bit_shift  = s % 8;

            if (byte_shift) {
                for (usize i = N; i-- > 0;) {
                    if (i >= byte_shift)
                        data[i] = data[i - byte_shift];
                    else
                        data[i] = std::byte(0);
                }
            }

            if (bit_shift) {
                u8 carry = 0;
                for (usize i = 0; i < N; ++i) {
                    u16 v = (get(i) << bit_shift) | carry;
                    set(i, v & 0xFF);
                    carry = v >> 8;
                }
            }

            return *this;
        }

        constexpr integer& operator>>=(usize s) {
            usize byte_shift = s / 8;
            usize bit_shift  = s % 8;

            if (byte_shift) {
                for (usize i = 0; i < N; ++i) {
                    if (i + byte_shift < N)
                        data[i] = data[i + byte_shift];
                    else
                        data[i] = std::byte(0);
                }
            }

            if (bit_shift) {
                u8 carry = 0;
                for (usize i = N; i-- > 0;) {
                    u16 v = (get(i) >> bit_shift) | (carry << (8 - bit_shift));
                    carry = get(i) & ((1 << bit_shift) - 1);
                    set(i, v);
                }
            }

            return *this;
        }

        // ===== BIT =====
        constexpr u8 get_bit(usize b) const {
            return (get(b / 8) >> (b % 8)) & 1;
        }

        constexpr void set_bit(usize b, u8 v) {
            usize i = b / 8;
            u8 x = get(i);
            if (v) x |= (1 << (b % 8));
            else x &= ~(1 << (b % 8));
            set(i, x);
        }

        // ===== DIV =====
        constexpr integer& operator/=(const integer& o) {
            if (o.is_zero()) throw std::runtime_error("div 0");

            integer q, r;

            for (usize i = N * 8; i-- > 0;) {
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

            for (usize i = N * 8; i-- > 0;) {
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
    template <usize N>
    class integer<N, isSigned::signed_t> {
    public:
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
                b = std::byte(~std::to_integer<u8>(b));
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

}