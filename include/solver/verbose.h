#pragma once

#include <memory>
#include <vector>
#include "field/vector.h"

namespace Solver {

template<typename Vector>
struct VerboseElement {
    std::pair<const Vector&, double> value;
    bool is_empty;

    VerboseElement(std::pair<const Vector&, double> value, bool is_empty): value(value), is_empty(is_empty) {}
};

template<typename Vector>
class VerboseUnit {
    using Variant = std::pair<Vector, double>;

    std::shared_ptr<std::vector<Vector>> variants;
    std::shared_ptr<std::vector<double>> scores;
    std::vector<bool> variants_mask;

    int64_t picked_id;

    const bool empty;
public:
    VerboseUnit(): empty(true) {}

    VerboseUnit(const Variant picked_variant): variants(std::make_shared<Vector>({picked_variant.first})),
        scores(std::make_shared<std::vector<double>>({picked_variant.second})), picked_id(0), empty(false),
        variants_mask(std::make_shared<std::vector<bool>>((*variants).size())) {}
    
    VerboseUnit(const Vector& solution, double score): 
        variants(std::make_shared<std::vector<Vector>>(std::initializer_list<Vector>({solution}))),
        scores(std::make_shared<std::vector<double>>(std::initializer_list<double>({score}))), picked_id(0), empty(false),
        variants_mask(std::vector<bool>(variants->size())) {}

    VerboseUnit(std::shared_ptr<std::vector<Vector>> variants, std::shared_ptr<std::vector<double>> scores,
                int64_t picked_id, const std::vector<bool>& variants_mask):
        variants(variants), scores(scores), picked_id(picked_id), empty(false),
        variants_mask(variants_mask) {}

    bool is_empty() const { return empty; }
    int64_t size() const { return (*variants).size(); }

    int64_t get_picked_id() const { return picked_id; }

    VerboseElement<Vector> operator[](int64_t i) const {
        return VerboseElement<Vector>(std::pair<const Vector&, double>(variants->at(i), scores->at(i)), variants_mask.at(i));
    }

    template<typename Field>
    void PrintFitnessLandScape(const std::unordered_map<int64_t, double>& landscape, Field& field, int64_t n) {
        std::cout << "\n============ Fitness Landscape ============\n";
        int64_t i=0;
        for (auto& pair : landscape) {
            std::cout << field[pair.first] << " : " << pair.second << "\n";
            i++;
            if (i >= n){
                break;
            }
        }
        std::cout << "\n";
    }
};

} // namespace Solver
