#pragma once

#include "field/field.h"
#include "field/vector.h"

#include <vector>
#include <functional>

namespace SearchFields {

template <typename LetterType, int N>
class GrayField : public Field<LetterType, N> {
    using Vector = typename Field<LetterType, N>::Vector;

	std::shared_ptr<Vector> _try_to_generate_element(int64_t i) const {
		return Utils::ToGray<N, Vector>(i, this->alphabet);
    }

public:
    using Field<LetterType, N>::Field;

    Vector generate_element_from_raw(const std::vector<int>& raw_vector) const
    {
        Vector result = Field<LetterType, N>::generate_element_from_raw(raw_vector);
        result.decode_helper = CodeType::GRAY;
        return result;
    }

};

} // namespace SearchFields
