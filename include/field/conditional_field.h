#pragma once

#include "field/field.h"

#include <vector>
#include <functional>

namespace SearchFields {

template <typename LetterType, int N>
class ConditionalField : public Field<LetterType, N> {
    std::function<bool(const Vector&)> filter;

	std::shared_ptr<Vector> _try_to_generate_element(int64_t i) const {
        int alphabet_power = this->alphabet.size();
        std::vector<int> translated = this->translate_int(i, alphabet_power);

        auto result = Utils::FromInteger<N, Vector>(i, this->alphabet);
		return this->filter(*result) ? result : std::shared_ptr<Vector>(nullptr);
    }

public:
    ConditionalField(const Alphabet& alphabet,
        std::function<bool(const Vector&)> filter,
        bool enable_caching = true)
        : Field<Vector, N>(alphabet, enable_caching), filter(filter)
         {}

	int64_t size() = delete;

	Vector& generate_element(int64_t i) const {
		std::shared_ptr<Vector> result;
		do {
			result = Field<T, N>::generate_element(i++);
		} while (!result.get());
		return *result;
	}
};

} // namespace SearchFields
