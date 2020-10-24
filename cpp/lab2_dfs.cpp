#include <iostream>
#include <vector>
#include <math.h>
#include <algorithm>
#include <unordered_map>
#include <functional>

#include <memory> 

#include "utils/utils.h"
#include "field/field.h"
#include "field/alphabet.h"
#include "field/vector.h"
#include "utils/scorers.h"
#include "solver/hill_climbing.h"
#include "menu.h"

const int N = 5;

using VectorType = SerachFields::Vector<int>;
using AlphabetType = SerachFields::Alphabet<int>;
using FieldType = SerachFields::Field<int, N>;
using SolverType = Solver::DFS<VectorType, FieldType>;



template<typename ScoreFunction>
void execute_lab(ScoreFunction score_fn_builder) {
    AlphabetType alphabet = std::vector<int>({0, 1});
    FieldType field{alphabet};
    auto score_fn = score_fn_builder(alphabet);

    SolverType _solver = SolverType(field, score_fn);

    int nsteps;

    std::cout << "Print number of steps:\n>";
    std::cin >> nsteps;

    _solver.TrainModel(nsteps);
}

std::vector<menuItem> main_menu_options = {
    {"Random score function", [](){ execute_lab(Scorers::build_random<AlphabetType>); }},
    {"Binary to int score function", [](){ execute_lab(std::bind(Scorers::build_bin_to_int<AlphabetType>, std::placeholders::_1)); }},
    {"Square deviation score function", [](){ execute_lab(std::bind(Scorers::build_square<AlphabetType>, std::placeholders::_1, N)); }}
};

int main(int argc, char* argv[])
{
    srand(time(0));

    Menu main_menu = Menu(main_menu_options, "Pick option");
    main_menu.run(true);
    return 0;
}