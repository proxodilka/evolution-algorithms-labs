#pragma once

#include "solver/base_hood.h"
#include "solver/utils.h"
#include "utils/utils.h"

namespace Solver{

template<typename Vector, typename Field>
class BFS : public BaseHoodSolver<Vector, Field> {
protected:

    void do_optimization_step(int step) override {

        this->UpdateHood();

        auto ptr = this->neighborhood;

        int candidate_id = -1;

        for (int i=0; i<this->neighborhood->size(); i++){
            if (this->neighborhood_scores->at(i) > this->best_score){
                this->best_score = this->neighborhood_scores->at(i);
                this->best_solution = this->neighborhood->at(i);
                candidate_id = i;
            }
        }

        this->finished = !this->best_score.is_updated();
        this->PrepareVerboseUnit(candidate_id);

    }

public:
    using BaseHoodSolver<Vector, Field>::BaseHoodSolver;

};

template<typename Vector, typename Field>
class DFS : public BaseHoodSolver<Vector, Field> {
protected:

    void do_optimization_step(int step) override {

        this->TryToUpdateHood();

        auto ptr = this->neighborhood;

        int candidate_id = ::Utils::random(this->neighborhood_mask);
        this->PrepareVerboseUnit(candidate_id);
        this->RemoveFromHood(candidate_id);

        int candidate_score = this->neighborhood_scores->at(candidate_id);

        if (candidate_score > this->best_score){
            this->best_solution = (*ptr)[candidate_id];
            this->best_score = candidate_score;
            this->should_update_hood = true;
        }

        this->finished = this->IsEmptyHood() && !this->best_score.is_updated();

    }

public:
    using BaseHoodSolver<Vector, Field>::BaseHoodSolver;

};

} // namespace Solver
