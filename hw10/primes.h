#pragma once

#include <cmath>

// TODO:
// Exercise 2:
// Implement greated common divisor using constexpr, such that it can be evaluated at compile time.
// Your implementation should use variadic templates, to enable calling it
// with any number of arguments, ensure that at least two arguments are provided.
//
// Exercise 3:
// Implement minimum common multiple using constexpr, such that it can be evaluated at compile time.
// As with GCD, your implementation should use variadic templates, to enable calling it
// with arbitrary number of arguments, but at least two
//
// Exercise 4:
// Implement power using template metaprogramming. The template struct must be
// called `Power`, and be callable with `Power<base, exponent, modulus>::value`.
// I.e. it needs a compile time evaluation `value`, which holds the result of
// the computation.
// Think about your base cases

template<typename Head>
constexpr Head eucledianGCD(Head a, Head b)
{
    return (b == 0) ? a : eucledianGCD(b, a % b);
}

template<typename Head>
constexpr Head gcd(Head a)
{
    return a;
}

template<typename Head, typename... Tail>
constexpr Head gcd(const Head& head, const Tail&... tail) {
    return eucledianGCD(head, gcd(tail...));
}

template<typename Head>
constexpr Head mcm(Head a)
{
    return a;
}

template<typename Head, typename... Tail>
constexpr Head mcm(const Head& head, const Tail&... tail)
{
    constexpr auto base_case = [](Head a, Head b) -> Head {
        return a * b / gcd(a, b);
    };

    return base_case(head, mcm(tail...));
}

template <int Base, int Exponent, int Modulus>
struct Power {
    static constexpr int value = (Power<Base, Exponent - 1, Modulus>::value * Base) % Modulus;
};

template <int Base, int Modulus>
struct Power<Base, 0, Modulus> {
    static constexpr int value = 1;
};

template <>
struct Power<1, 0, 1> {
    static constexpr int value = 0;
};