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

template <typename Vector, int N>
class iterator;

template <typename LetterType, int N>
class Field {
public:
    using Alphabet = Alphabet<LetterType>;
    using Vector = Vector<LetterType>;

    Field(const Alphabet& alphabet)
        : alphabet(alphabet)
        , max_translatable(pow(alphabet.size(), N)) {}

    Vector& generate_element(int i) const
    {
        i %= this->max_translatable;
        return *(this->_generate_element(i));
    }

    std::vector<int> generate_raw_element(int i) const {
        return generate_raw_element(generate_element(i));
    }

    std::vector<int> generate_raw_element(const Vector& src) const {
        std::vector<int> result = std::vector<int>(src.size());

        for (int i=0; i<src.size(); i++){
            result[i] = alphabet.letter_to_int(src[i]);
        }

        return result;
    }

    Vector generate_element_from_raw(const std::vector<int>& raw_vector) const
    {
        Vector result = Vector(raw_vector.size());

        for (int i=0; i<raw_vector.size(); i++){
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

    Vector& operator[](int i) { return this->generate_element(i); }
    // Vector& operator[](int i) { return this->generate_element(i); }

    int size() const { return this->__approximate_size(); }

    /// \brief Returns upper bound of field size
    int __approximate_size() const { return this->max_translatable; }

    void __fill_cache() {
        this->caching_enabled = true;
        for (int i=0; i<this->__approximate_size(); i++){
            this->generate_element(i);
        }
    }

    iterator<Vector, N> begin() { return iterator<Vector, N>(this); }
    iterator<Vector, N> end() { return iterator<Vector, N>(this, true); }


protected:
    int max_translatable;
    const Alphabet& alphabet;
    mutable std::unordered_map<int, std::shared_ptr<Vector> > cache;

    virtual std::shared_ptr<Vector> _try_to_generate_element(int i) const {
        return Utils::FromInteger<N, Vector>(i, this->alphabet);
    }

    std::shared_ptr<Vector> _generate_element(int i) const {
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

template <typename Vector, int N>
class iterator{
    friend Field<Vector, N>;
    Field<Vector, N>* field;
    int pos;
    bool invalid;

    iterator(Field<Vector, N>* field, int pos=0): field(field), pos(pos), invalid(false) {}
    iterator(Field<Vector, N>* field, bool invalid): field(nullptr), pos(0), invalid(true) {}

public:
    Vector& operator*() { return (*field)[pos]; }
    Vector& operator[](int offset) { return (*field)[pos + offset]; }

    bool operator==(const iterator& it) {
        if (this->invalid && it.invalid && this->field == it.field) {
            return true;
        }
        return this->invalid == it.invalid && this->field == it.field && this->pos == it.pos;
    }
    bool operator!=(const iterator& it){ return !this->operator==(it); }

    iterator& operator++(int) {
        std::shared_ptr<Vector> result;
        do {
            result = field->_generate_element(++pos);
        } while (!result.get() && pos < field->__approximate_size());

        invalid = (pos == field->__approximate_size());
        return *this;
    }

    iterator operator++() {
        iterator ret_it = *this;
        ++(*this);
        return ret_it;
    }


};
   
} // namespace field