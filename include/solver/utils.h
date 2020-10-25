#pragma once

#include <vector>
#include <memory>
#include <unordered_set>

namespace Solver{ 
namespace Utils{

template<typename Vector>
int operator^(const Vector& l, const Vector& r){

    if (l.size() != r.size()){
        throw std::invalid_argument("Can't compute XOR on Vectors with different lengths.");
    }

    int diff_counter = 0;
    std::unordered_set<int> different_values(3);

    for (int i=0; i<l.size(); i++){
        diff_counter += l[i] != r[i];
        if (different_values.size() > 2) {
            throw std::invalid_argument("Passed Vectors are not binary.");
        }
    }

    return diff_counter;
}

template<typename Vector, typename Field>
std::shared_ptr<std::vector<Vector>> GetNeighborhoodHeming(const Vector& vector, const Field& field) {
    std::vector<Vector>* result_vector = new std::vector<Vector>();
    result_vector->reserve(vector.size());

    std::vector<int> raw = field.generate_raw_element(vector);

    for (int i=0; i<raw.size(); i++){
        int prev = raw[i];
        raw[i] = (raw[i] + 1) % 2;
        result_vector->push_back(field.generate_element_from_raw(raw));
        raw[i] = prev;
    }

    return std::shared_ptr<std::vector<Vector>>(result_vector);
}


} // namespace Utils
} // namespace Solver