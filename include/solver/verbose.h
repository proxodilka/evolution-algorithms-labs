#pragma once

#include <memory>
#include <vector>
#include "field/vector.h"

namespace Solver {

template<typename Vector>
struct VerboseElement {
    std::pair<const Vector&, int> value;
    bool is_empty;

    VerboseElement(std::pair<const Vector&, int> value, bool is_empty): value(value), is_empty(is_empty) {}
};

template<typename Vector>
class VerboseUnit {
    using Variant = std::pair<Vector, int>;

    std::shared_ptr<std::vector<Vector>> variants;
    std::shared_ptr<std::vector<int>> scores;
    std::shared_ptr<std::vector<bool>> variants_mask;

    int picked_id;

    const bool empty;
public:
    VerboseUnit(): empty(true) {}

    VerboseUnit(const Variant picked_variant): variants(std::make_shared<Vector>({picked_variant.first})),
        scores(std::make_shared<std::vector<int>>({picked_variant.second})), picked_id(0), empty(false),
        variants_mask(std::make_shared<std::vector<bool>>((*variants).size())) {}
    
    VerboseUnit(const Vector& solution, int score): 
        variants(std::make_shared<std::vector<Vector>>(std::initializer_list<Vector>({solution}))),
        scores(std::make_shared<std::vector<int>>(std::initializer_list<int>({score}))), picked_id(0), empty(false),
        variants_mask(std::make_shared<std::vector<bool>>((*variants).size())) {}

    VerboseUnit(std::shared_ptr<std::vector<Vector>> variants, std::shared_ptr<std::vector<int>> scores, int picked_id):
        variants(variants), scores(scores), picked_id(picked_id), empty(false),
        variants_mask(std::make_shared<std::vector<bool>>((*variants).size())) {}

    bool is_empty() const { return empty; }
    int size() const { return (*variants).size(); }

    int get_picked_id() const { return picked_id; }

    VerboseElement<Vector> operator[](int i) const {
        return VerboseElement<Vector>(std::pair<const Vector&, int>((*variants)[i], (*scores)[i]), (*variants_mask)[i]);
    }
};

} // namespace Solver
