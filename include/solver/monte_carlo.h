#pragma once

#include "solver/base_solver.h"

namespace Solver{

template<typename Vector, typename Field>
class MonteCarlo : public BaseSolver<Vector, Field> {
protected:

    void do_optimization_step(int step) override {
        int64_t rand_index = Utils::random(this->field.__approximate_size());
        auto& new_candidate = this->field[rand_index];

        int64_t new_score = this->score(new_candidate);
        if (new_score > this->best_score) {
            this->best_score = new_score;
            this->best_solution = new_candidate;
        }

        this->PrepareVerboseUnit(new_candidate, new_score);
    }

public:
    using BaseSolver<Vector, Field>::BaseSolver;

};

} // namespace Solver
