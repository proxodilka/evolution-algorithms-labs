#pragma once

#include "solver/base_hood.h"
#include "solver/utils.h"
#include "utils/utils.h"

namespace Solver{

template<typename Vector, typename Field>
class BFS : public BaseHoodSolver<Vector, Field> {
protected:
    bool finished = false;

    void do_optimization_step(int step) override {
        if (finished) {
            return;
        }

        auto ptr = Solver::Utils::GetNeighborhoodHeming(this->best_solution, this->field);
        for (auto& neighbor : *ptr) {
            int new_score = this->score(neighbor);
            if (new_score > this->best_score) {
                this->best_score = new_score;
                this->best_solution = neighbor;
            }
        }

        finished = !this->best_score.is_updated();

        this->PrepareVerboseUnit(ptr, -1);
    }

public:
    using BaseHoodSolver<Vector, Field>::BaseHoodSolver;

};

template<typename Vector, typename Field>
class DFS : public BaseHoodSolver<Vector, Field> {
protected:
    bool finished = false;

    void do_optimization_step(int step) override {
        if (finished) {
            return;
        }

        this->TryToUpdateHood();

        auto ptr = this->neighborhood;
        //std::cout << step << " " << this->neighborhood_mask.size() << std::endl;
        int candidate_id = ::Utils::random(this->neighborhood_mask);
        this->PrepareVerboseUnit(candidate_id);
        this->RemoveFromHood(candidate_id);

        int candidate_score = this->neighborhood_scores->at(candidate_id);

        if (candidate_score > this->best_score){
            this->best_solution = (*ptr)[candidate_id];
            this->best_score = candidate_score;
            this->should_update_hood = true;
        }

        finished = this->IsEmptyHood() && !this->best_score.is_updated();

    }

public:
    using BaseHoodSolver<Vector, Field>::BaseHoodSolver;

};

} // namespace Solver
