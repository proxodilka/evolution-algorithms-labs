#pragma once

#include <functional>
#include <stdlib.h>
#include "utils/utils.h"
#include "field/vector.h"
#include "utils/type_helper.h"

namespace Scorers{

// template<typename Vector>
// std::function<int(const Vector&)> build_random(const Vector& alphabet, int min=0, int max=RAND_MAX) {
//     auto fn = [=](const Vector& vector) { return Utils::random(min, max); };
//     return Utils::cache_calls<int>(fn, std::bind(Utils::ToInteger<Vector, Vector>, std::placeholders::_1, alphabet));
// };

template<typename AlphabetType, typename VectorType = typename Utils::rebind_container<AlphabetType, SearchFields::Vector<void>>::value >
std::function<int64_t(const VectorType&)> build_random(const AlphabetType& alphabet) {
    int min = 0, max=RAND_MAX;
    auto fn = [=](const VectorType& vector) { return Utils::random(min, max); };
    return Utils::cache_calls<int64_t>(fn, std::bind(Utils::ToInteger<VectorType, AlphabetType>, std::placeholders::_1, alphabet));
};

// template<typename Vector>
// std::function<int(const Vector&)> build_random(const Vector& alphabet, int max) {
//     return build_random_scorer(alphabet, 0, max);
// };

template<typename AlphabetType, typename VectorType = typename Utils::rebind_container<AlphabetType, SearchFields::Vector<void>>::value >
std::function<int64_t(const VectorType&)> build_bin_to_int(const AlphabetType& alphabet) {
    auto fn = [=](const VectorType& vector) { return Utils::ToInteger(vector, alphabet); };
    return Utils::cache_calls<int64_t>(fn, std::bind(Utils::ToInteger<VectorType, AlphabetType>, std::placeholders::_1, alphabet));
};

template<typename AlphabetType, typename VectorType = typename Utils::rebind_container<AlphabetType, SearchFields::Vector<void>>::value >
std::function<int64_t(const VectorType&)> build_square(const AlphabetType& alphabet, int64_t field_size) {
    auto fn = [=](const VectorType& vector) { return pow(Utils::ToInteger(vector, alphabet) - (1 << (field_size - 1)), 2); };
    return Utils::cache_calls<int64_t>(fn, std::bind(Utils::ToInteger<VectorType, AlphabetType>, std::placeholders::_1, alphabet));
};

} // namespace Scorers
