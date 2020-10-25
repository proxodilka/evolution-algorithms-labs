#pragma once

#include <memory>
#include <vector>
#include <functional>

#include "solver/base_solver.h"
#include "solver/utils.h"

namespace Solver {

template<typename Vector, typename Field>
class BaseHoodSolver : public BaseSolver<Vector, Field> {
protected:
    const typename Field::Alphabet& alphabet;

    std::shared_ptr<std::vector<Vector>> neighborhood;
    std::shared_ptr<std::vector<int>> neighborhood_scores;
    std::vector<bool> neighborhood_mask;

    int in_hood = 0;

    void PrepareVerboseUnit(int candidate_id) {
        if (!this->verbose){
            return;
        }


        this->verbose_unit = std::make_shared<VerboseUnit<Vector>>(this->neighborhood, this->neighborhood_scores, candidate_id);
    }

    void TryToUpdateHood() {
        if (this->IsEmptyHood()) {
            this->UpdateHood();
        }
    }

    bool IsEmptyHood() {
        return this->in_hood;
    }

    void ResetMask() {
        neighborhood_mask.clear();
        neighborhood_mask.resize(neighborhood->size());
    }

    void ResetScores() {
        if (!neighborhood_scores.use_count()){
            neighborhood_scores = std::make_shared<std::vector<int>>();
        }
        neighborhood_scores->clear();

        int size = neighborhood->size();
        neighborhood_scores->reserve(size);

        for (int i=0; i<size; i++){
            neighborhood_scores->push_back(this->score(neighborhood->at(i)));
        }
    }

    void RemoveFromHood(int i){
        this->neighborhood_mask[i] = true;
        this->in_hood--;
    }

    void UpdateHood() {
        this->neighborhood = Solver::Utils::GetNeighborhoodHeming(this->best_solution, this->field);
        this->in_hood = this->neighborhood->size();
        this->ResetScores();
        this->ResetMask();
    }

public:
    BaseHoodSolver(Field& field, const std::function<int(const Vector&)>& scorer): 
                        BaseSolver<Vector, Field>(field, scorer), alphabet(field.get_alphabet()) {
        if (field.get_alphabet().size() != 2) {
            delete this; // since throwing exception in an object constructor don't triggers
                         // objects destructor to be executed, calling destructor explicitly
            throw std::invalid_argument("Can't construct BinarySolver with Field whose alphabet is not binary.");
        }
    }

};

} // namespace Solver