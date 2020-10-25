#pragma once

#include <vector>
#include <math.h>
#include <algorithm>
#include <tuple>
#include <memory>
#include <stdlib.h>
#include <time.h>


namespace Utils{

int random(int min, int max) {
    return rand() % (max - min) + min;
}

int random(int max) {
    return random(0, max);
}

int random(const std::vector<int>& restrict_mask) {
    int value = random(0, restrict_mask.size());
    for (; restrict_mask[value % restrict_mask.size()]; value++) {}
    return value % restrict_mask.size();
}

int random() {
    return random(0, RAND_MAX);
}

template<typename Container, typename T>
int GetOffset(const Container& vec, const T& value) {
    auto it = std::find(vec.cbegin(), vec.cend(), value);
    return it - vec.cbegin();
}

template<typename Vector, typename AlphabetType>
int ToInteger(const Vector& value, const AlphabetType& alphabet) {
    int power = alphabet.size();
    int result = 0;

    for (int r_i = value.size() - 1, i = 0; r_i>=0; r_i--, i++) {
        result += GetOffset(alphabet, value[r_i]) * std::pow(power, i);
    }

    return result;
}

template<int N=0, typename Vector, typename AlphabetType>
std::shared_ptr<Vector> FromInteger(int x, const AlphabetType& alphabet){
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
    std::reverse(translated.begin(), translated.end());

    auto result = std::make_shared<Vector>(translated.size());

    for (int r_i = translated.size() - 1, i = 0; r_i>=0; r_i--, i++) {
        result->at(i) = alphabet[translated[r_i]];
    }

    return result;
}


template<typename Value, typename FuncType, typename HasherType>
auto cache_calls(FuncType fn, HasherType hash_fn) {
    return [fn, hash_fn](auto... args) {
        static std::unordered_map<int, Value> cache;
        int hash = hash_fn(args...);

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
