#pragma once

#include "solver/base_solver.h"
#include "solver/utils.h"
#include "utils/utils.h"

namespace Solver{

template<typename Vector, typename Field>
class BFS : public BaseBinarySolver<Vector, Field> {
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
    using BaseBinarySolver<Vector, Field>::BaseBinarySolver;

};

template<typename Vector, typename Field>
class DFS : public BaseBinarySolver<Vector, Field> {
protected:
    bool finished = false;

    void do_optimization_step(int step) override {
        if (finished) {
            return;
        }

        auto ptr = Solver::Utils::GetNeighborhoodHeming(this->best_solution, this->field);
        //std::cout << (*ptr).size()  << " " << step << std::endl;
        int candidate_id = ::Utils::random((*ptr).size());

        auto new_candidate = (*ptr)[candidate_id];
        int candidate_score = this->score(new_candidate);

        if (candidate_score > this->best_score){
            this->best_solution = new_candidate;
            this->best_score = candidate_score;
        }

        finished = !this->best_score.is_updated();

        this->PrepareVerboseUnit(ptr, candidate_id);
    }

public:
    using BaseBinarySolver<Vector, Field>::BaseBinarySolver;

};

} // namespace Solver
