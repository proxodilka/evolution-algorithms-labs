#pragma once

#include <vector>
#include <math.h>
#include <algorithm>
#include <unordered_map>
#include <memory>

#include "field/alphabet.h"
#include "field/vector.h"

#include "utils/utils.h"
#include "utils/type_helper.h"

template <typename T>
std::ostream& operator<<(std::ostream& out, const std::vector<T>& value)
{
    char sep = ' ';
    for (auto& x : value) {
        out << x << sep;
    }
    return out;
}

namespace SearchFields {

template <typename LetterType, int N>
class iterator;

template <typename LetterType, int N>
class Field {
public:
    using Alphabet = Alphabet<LetterType>;
    using Vector = Vector<LetterType>;
    friend iterator<LetterType, N>;

    Field(const Alphabet& alphabet)
        : alphabet(alphabet)
        , max_translatable(pow(alphabet.size(), N)) {}

    Vector& generate_element(int64_t i) const
    {
        i %= this->max_translatable;
        return *(this->_generate_element(i));
    }

    std::vector<int> generate_raw_element(int64_t i) const {
        return generate_raw_element(generate_element(i));
    }

    std::vector<int> generate_raw_element(const Vector& src) const {
        std::vector<int> result = std::vector<int>(src.size());

        for (int64_t i=0; i<src.size(); i++){
            result[i] = alphabet.letter_to_int(src[i]);
        }

        return result;
    }

    Vector generate_element_from_raw(const std::vector<int>& raw_vector) const
    {
        Vector result = Vector(raw_vector.size());

        for (int64_t i=0; i<raw_vector.size(); i++){
            result[i] = alphabet.int_to_letter(raw_vector[i]);
        }

        return result;
    }

    void set_alphabet(const Alphabet& new_alphabet)
    {
        this->alphabet = new_alphabet;
        this->max_translatable = pow(this->alphabet.size(), N);
		cache.clear();
    }

    const Alphabet& get_alphabet() const { return this->alphabet; }

    Vector& operator[](int64_t i) { return this->generate_element(i); }
    const Vector& operator[](int64_t i) const { return this->generate_element(i); }

    int64_t size() const { return this->__approximate_size(); }

    /// \brief Returns upper bound of field size
    int64_t __approximate_size() const { return this->max_translatable; }

    void __fill_cache() {
        this->caching_enabled = true;
        for (int64_t i=0; i<this->__approximate_size(); i++){
            this->generate_element(i);
        }
    }

    iterator<LetterType, N> begin() { return iterator<LetterType, N>(this); }
    iterator<LetterType, N> end() { return iterator<LetterType, N>(this, true); }


protected:
    int64_t max_translatable;
    const Alphabet& alphabet;
    mutable std::unordered_map<int64_t, std::shared_ptr<Vector> > cache;

    virtual std::shared_ptr<Vector> _try_to_generate_element(int64_t i) const {
        return Utils::FromInteger<N, Vector>(i, this->alphabet);
    }

    std::shared_ptr<Vector> _generate_element(int64_t i) const {
        if (i > this->max_translatable){
            return std::shared_ptr<Vector>(nullptr);
        }

        auto res = cache.find(i);
        if (res != cache.end()) {
            return res->second;
        }

        auto result = this->_try_to_generate_element(i);
        cache.insert({ i, result });
        return result;
    }

};

template <typename LetterType, int N>
class iterator{
    using Vector = Vector<LetterType>;
    friend Field<LetterType, N>;
    Field<LetterType, N>* field;
    int64_t pos;
    bool invalid;

    iterator(Field<LetterType, N>* field, int64_t pos=0): field(field), pos(pos), invalid(false) {}
    iterator(Field<LetterType, N>* field, bool invalid): field(nullptr), pos(0), invalid(true) {}

public:
    Vector& operator*() { return (*field)[pos]; }
    Vector& operator[](int offset) { return (*field)[pos + offset]; }

    bool operator==(const iterator& it) {
        if (this->invalid && it.invalid) {
            return true;
        }
        return this->invalid == it.invalid && this->field == it.field && this->pos == it.pos;
    }
    bool operator!=(const iterator& it){ return !this->operator==(it); }

    iterator& operator++() {
        std::shared_ptr<Vector> result;
        do {
            result = field->_generate_element(pos++);
        } while (result.get() == nullptr && pos < field->__approximate_size());

        invalid = (pos == field->__approximate_size());
        return *this;
    }

    iterator operator++(int) {
        iterator ret_it = *this;
        ++(*this);
        return ret_it;
    }


};
   
} // namespace field