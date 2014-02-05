#include "Randomizer.hh"

#include <cassert>
#include <iostream>

namespace Core
{

///specialized instanciation for each built-in type
/// built in type list from http://en.literateprograms.org/Basic_constructs_%28C_Plus_Plus%29#Built-in_types
template<> Randomizer<char> * Randomizer<char>::instance = 0;
template<> Randomizer<signed char> * Randomizer<signed char>::instance = 0;
template<> Randomizer<unsigned char> * Randomizer<unsigned char>::instance = 0;

template<> Randomizer<wchar_t> * Randomizer<wchar_t>::instance = 0;

template<> Randomizer<short> * Randomizer<short>::instance = 0;
template<> Randomizer<unsigned short> * Randomizer<unsigned short>::instance = 0;

template<> Randomizer<int> * Randomizer<int>::instance = 0;
template<> Randomizer<unsigned int> * Randomizer<unsigned int>::instance = 0;

template<> Randomizer<long> * Randomizer<long>::instance = 0;
template<> Randomizer<unsigned long> * Randomizer<unsigned long>::instance = 0;

//TODO : need specialization for bool.
template<> Randomizer<bool> * Randomizer<bool>::instance = 0;

template<> Randomizer<float> * Randomizer<float>::instance = 0;
template<> Randomizer<double> * Randomizer<double>::instance = 0;
template<> Randomizer<long double> * Randomizer<long double>::instance = 0;

// the following types are not in the current C++ standard, but a very common extension,
// part of C99, and will be part of next version of the C++ standard
template<> Randomizer<long long> * Randomizer<long long>::instance = 0;
template<> Randomizer<unsigned long long> * Randomizer<unsigned long long>::instance = 0;

}
