#pragma once

#include <vector>
#include <math.h>
#include <algorithm>
#include <tuple>
#include <memory>
#include <stdlib.h>
#include <time.h>

#include "field/vector.h"


namespace Utils{

int random(int min, int max) {
    return rand() % (max - min) + min;
}

int random(int max) {
    return random(0, max);
}

int random(const std::vector<bool>& restrict_mask) {
    int value = random(0, restrict_mask.size());
    for (; restrict_mask[value % restrict_mask.size()]; value++) {}
    return value % restrict_mask.size();
}

int random() {
    return random(0, RAND_MAX);
}

template<typename Container, typename T>
int64_t GetOffset(const Container& vec, const T& value) {
    auto it = std::find(vec.cbegin(), vec.cend(), value);
    return it - vec.cbegin();
}

template<typename Vector>
std::shared_ptr<Vector> FromGray(const Vector& vector) {
    Vector out = vector;
    size_t n = vector.size() - vector.first_non_zero();

    for (size_t i = 1; i <= n; i++) {
        out ^= (vector >> i);
    }

    return std::make_shared<Vector>(out);
}

template<typename Vector, typename AlphabetType>
int64_t ToIntegerFromRegular(const Vector& value, const AlphabetType& alphabet) {
    int power = alphabet.size();
    int64_t result = 0;

    for (int r_i = value.size() - 1, i = 0; r_i>=0; r_i--, i++) {
        result += GetOffset(alphabet, value[r_i]) * std::pow(power, i);
    }

    return result;
}

template<typename Vector, typename AlphabetType>
int64_t ToIntegerFromGray(const Vector& vector, const AlphabetType& alphabet) {
    auto result = FromGray(vector);
    return ToIntegerFromRegular(*result, alphabet);
}

template<typename Vector>
int64_t ToIntegerFromGray(const Vector& vector) {
    auto result = FromGray(vector);
    return ToIntegerFromRegular(*result, std::vector<int>{0, 1});
}

template<typename Vector, typename AlphabetType>
int64_t ToInteger(const Vector& vector, const AlphabetType& alphabet) {
    switch (vector.decode_helper)
    {
    case SearchFields::CodeType::REGULAR:
        return ToIntegerFromRegular(vector, alphabet);
        break;
    case SearchFields::CodeType::GRAY:
        return ToIntegerFromGray(vector, alphabet);
        break;
    default:
        return ToIntegerFromRegular(vector, alphabet);
        break;
    }
}

template<int N=0, typename Vector, typename AlphabetType>
std::shared_ptr<Vector> FromInteger(int64_t x, const AlphabetType& alphabet){
    int alphabet_power = alphabet.size();
    std::vector<int> translated;
    translated.reserve(N);
    int rest = 0;

    while (x >= alphabet_power) {
        rest = x % alphabet_power;
        x = x / alphabet_power;
        translated.push_back(rest);
    }
    translated.push_back(x);
    for (int i = translated.size(); i < N; i++) {
        translated.push_back(0);
    }

    auto result = std::make_shared<Vector>(translated.size());

    for (int r_i = translated.size() - 1, i = 0; r_i>=0; r_i--, i++) {
        result->at(i) = alphabet[translated[r_i]];
    }

    return result;
}

template<typename Vector>
std::shared_ptr<Vector> ToGray(const Vector& vector) {
    auto out = (vector ^ (vector >> 1));
    out.decode_helper = SearchFields::CodeType::GRAY;
    return std::make_shared<Vector>(std::move(out));
}

template<int N=0, typename Vector, typename AlphabetType>
std::shared_ptr<Vector> ToGray(int64_t x, const AlphabetType& alphabet) {
    auto vector = FromInteger<N, Vector, AlphabetType>(x, alphabet);
    return ToGray(*vector);
}

template<typename Value, typename FuncType, typename HasherType>
auto cache_calls(FuncType fn, HasherType hash_fn) {
    return [fn, hash_fn](auto... args) {
        static std::unordered_map<int64_t, Value> cache;
        int64_t hash = hash_fn(args...);

        auto cache_res = cache.find(hash);
        if (cache_res != cache.end()) {
            return cache_res->second;
        }

        Value res = fn(args...);
        cache.insert({hash, res});

        return res;
    };
}

} // namespace Utils
