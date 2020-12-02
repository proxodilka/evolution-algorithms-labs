#pragma once

#include <memory>
#include <vector>
#include <functional>

#include "solver/base_solver.h"
#include "solver/utils.h"

namespace Solver {

template<typename Vector, typename Field>
class BaseHoodSolver : public BaseSolver<Vector, Field> {
private:
    int64_t in_hood = 0;

    void ResetMask() {
        neighborhood_mask.clear();
        neighborhood_mask.resize(neighborhood->size());
    }

    void ResetScores() {
        if (!neighborhood_scores.use_count()){
            neighborhood_scores = std::make_shared<std::vector<double>>();
        }
        neighborhood_scores->clear();

        int64_t size = neighborhood->size();
        neighborhood_scores->reserve(size);

        for (int64_t i=0; i<size; i++){
            neighborhood_scores->push_back(this->score(neighborhood->at(i)));
        }
    }

protected:
    const typename Field::Alphabet& alphabet;

    std::shared_ptr<std::vector<Vector>> neighborhood;
    std::shared_ptr<std::vector<double>> neighborhood_scores;
    std::vector<bool> neighborhood_mask;

    bool should_update_hood = true;

    void PrepareVerboseUnit(int64_t candidate_id) {
        if (!this->verbose){
            return;
        }

        this->verbose_unit = std::make_shared<VerboseUnit<Vector>>(this->neighborhood, this->neighborhood_scores,
                            candidate_id, this->neighborhood_mask);
    }

    void TryToUpdateHood() {
        if (this->should_update_hood || this->IsEmptyHood()) {
            this->UpdateHood();
            this->should_update_hood = false;
        }
    }

    bool IsEmptyHood() {
        return !this->in_hood;
    }

    void RemoveFromHood(int64_t i){
        neighborhood_mask[i] = true;
        this->in_hood--;
    }

    void UpdateHood() {
        this->neighborhood = Solver::Utils::GetNeighborhoodHeming(this->best_solution, this->field);
        this->in_hood = this->neighborhood->size();
        this->ResetScores();
        this->ResetMask();
    }

    void InitTraining() override {
        BaseSolver<Vector, Field>::InitTraining();
        this->UpdateHood();
    }

public:
    BaseHoodSolver(Field& field, const std::function<double(const Vector&)>& scorer): 
                        BaseSolver<Vector, Field>(field, scorer), alphabet(field.get_alphabet()) {
        if (field.get_alphabet().size() != 2) {
            delete this; // since throwing exception in an object constructor don't triggers
                         // objects destructor to be executed, calling destructor explicitly
            throw std::invalid_argument("Can't construct BinarySolver with Field whose alphabet is not binary.");
        }
    }

    constexpr static const char* name = "Base Hood Solver";

    virtual std::string get_name() {
        return this->name;
    }

};

} // namespace Solver