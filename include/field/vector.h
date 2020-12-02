#pragma once

#include <vector>
#include <cassert>

namespace SearchFields {

enum CodeType {
    REGULAR,
    GRAY
};

template<typename T>
class Vector : public std::vector<T> {
    using std::vector<T>::vector;

public:
    CodeType decode_helper = CodeType::REGULAR;

    Vector operator>>(int32_t offset) const {
        Vector out(this->size());

        if (offset >= this->size()) {
            return out;
        }

        for (size_t i = 0; i < this->size() - offset + 1; i++) {
            out[i + offset] = this->operator[](i);
        }

        return out;
    }

    Vector& operator^=(const Vector& other) {
        assert(this->size() == other.size());

        for (size_t i = 0; i < other.size(); i++){
            this->operator[](i) ^= other[i];
        }

        return *this;
    }

    friend Vector operator^(const Vector& l, const Vector& r) {
        Vector out = l;
        out ^= r;
        return out;
    }

    size_t first_non_zero() const {
        size_t i = 0;
        for (; i < this->size(); i++) {
            if (this->operator[](i)) {
                break;
            }
        }
        return i;
    }
};

} // namespace SearchFields
