#include <iostream>
#include <vector>
#include <math.h>
#include <algorithm>
#include <unordered_map>
#include <functional>

#include <memory> 

#include "utils/utils.h"
#include "field/gray_field.h"
#include "field/alphabet.h"
#include "field/vector.h"
#include "utils/scorers.h"
#include "menu.h"

#include "solver/monte_carlo.h"
#include "solver/hill_climbing.h"

const int MAX_NLANDSCAPE = 32;
const int N = 7;

using VectorType = SearchFields::Vector<int>;
using AlphabetType = SearchFields::Alphabet<int>;
using FieldType = SearchFields::GrayField<int, N>;

using Base = Solver::BaseSolver<VectorType, FieldType>;
using BFS = Solver::BFS<VectorType, FieldType>;
using DFS = Solver::DFS<VectorType, FieldType>;
using MonteCarlo = Solver::MonteCarlo<VectorType, FieldType>;

struct Solution {
    double score;
    VectorType solution;
    std::string method_name;

    Solution(const std::string& method_name, const VectorType& solution, double score) : method_name(method_name), solution(solution), score(score) {}
    Solution(const std::string& method_name, const std::pair<VectorType, double>& solution) : method_name(method_name), solution(solution.first), score(solution.second) {}
};

template<typename ScoreFunction>
void execute_lab(ScoreFunction score_fn_builder) {
    AlphabetType alphabet = std::vector<int>({0, 1});
    FieldType field{alphabet};
    auto score_fn = score_fn_builder(alphabet);

    std::vector<std::shared_ptr<Base>> solvers = {
        std::make_shared<MonteCarlo>(MonteCarlo(field, score_fn)),
        std::make_shared<BFS>(BFS(field, score_fn)),
        std::make_shared<DFS>(DFS(field, score_fn))
    };

    int nsteps, msteps;

    solvers[0]->PrintFitnessLandScape(MAX_NLANDSCAPE);
    std::cout << "Print number of steps:\n>";
    std::cin >> nsteps >> msteps;

    std::vector<Solution> solutions;
    solutions.reserve(msteps);
    for (size_t i = 0; i < msteps; i++) {
        int solver_id = Utils::random(0, 3);
        auto _solver = solvers[solver_id];

        std::cout << "\n====== " << _solver->get_name() << " solver was picked ======\n\n";

        auto solution = _solver->TrainModel(nsteps);
        solutions.push_back(Solution(_solver->get_name(), solution));
    }

    std::cout << "\nAll solutions that were found for " << msteps << " runs:\n";
    Solution best_solution {"None", {0}, -INT_MAX};
    for (auto& pair : solutions){
        std::cout << "\t" << pair.solution << " : " << pair.score << " | " << pair.method_name << "\n";
        if (best_solution.score < pair.score) {
            best_solution = pair;
        }
    }
    std::cout << "\nBest solution that was found:\n\t" << best_solution.solution << " : " << best_solution.score << " | " << best_solution.method_name << std::endl;
    
}

std::vector<menuItem> main_menu_options = {
    {"Random score function", [](){ execute_lab(Scorers::build_random<AlphabetType>); }},
    {"Binary to int score function", [](){ execute_lab(std::bind(Scorers::build_bin_to_int<AlphabetType>, std::placeholders::_1)); }},
    {"Square deviation score function", [](){ execute_lab(std::bind(Scorers::build_square<AlphabetType>, std::placeholders::_1, N)); }},
    {"Score = 5sin(x) + ln(x)", []() { execute_lab(std::bind(Scorers::build_sin_ln<AlphabetType>, std::placeholders::_1, N)); }}
};

int main(int argc, char* argv[])
{
    srand(time(0));

    Menu main_menu = Menu(main_menu_options, "Pick option");
    main_menu.run(true);
    return 0;
}