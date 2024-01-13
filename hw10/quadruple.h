#pragma once

#include <array>

// TODO: Implement the Quadruple struct, with the required specialisations
// - The general case should store types and have getters for each called `get_first`, `get_second`,
// etc
// - Implement once specialization if all types are ints. Store them in a publicly accessible
// `std::array`, again provide the getters.
// - Implement once specialization if all types are the same. Store them in a publicly accessible
// `std::array`, again provide the getters.
template <typename T, typename U, typename V, typename W> struct Quadruple {
public:
    Quadruple(T first, U second, V third, W fourth) :
    first_{first}, second_{second}, third_{third}, fourth_{fourth} {}

    T get_first() { return first_; }
    U get_second() { return second_; }
    V get_third() { return third_; }
    W get_fourth() { return fourth_; }

private:
    T first_;
    U second_;
    V third_;
    W fourth_;
};

// specilisation if all types are ints
template <> 
struct Quadruple<int, int, int, int> {
public:
    Quadruple(int first, int second, int third, int fourth) : 
    members{{first, second, third, fourth}} {}

    int get_first() { return members[0]; }
    int get_second() { return members[1]; }
    int get_third() { return members[2]; }
    int get_fourth() { return members[3]; }

public:
    std::array<int, 4> members;

};

// specilisation if all types are the same
template <typename T> 
struct Quadruple<T, T, T, T> {
public:
    Quadruple(T first, T second, T third, T fourth) : 
    members{{first, second, third, fourth}} {}

    T get_first() { return members[0]; }
    T get_second() { return members[1]; }
    T get_third() { return members[2]; }
    T get_fourth() { return members[3]; }

public:
    std::array<T, 4> members;

};
