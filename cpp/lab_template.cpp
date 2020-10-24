#include <iostream>
#include <vector>
#include <math.h>
#include <algorithm>
#include <unordered_map>
#include <functional>

#include <memory> 

#include "utils/utils.h"
#include "field/field.h"
#include "utils/scorers.h"
#include "solver/monte_carlo.h"
#include "menu.h"

const int N = 15;

using VectorType = std::vector<int>;
using FieldType = SerachFields::Field<VectorType, N>;
using SolverType = Solver::BaseSolver<VectorType, FieldType>;



template<typename ScoreFunction>
void execute_lab(ScoreFunction score_fn_builder) {
    VectorType alphabet = {0, 1};
    FieldType field{alphabet};

    auto score_fn = score_fn_builder(alphabet);
    SolverType _solver = SolverType(field, score_fn);

    int nsteps;

    std::cout << "Print number of steps:\n>";
    std::cin >> nsteps;

    _solver.TrainModel(nsteps);
}

std::vector<menuItem> main_menu_options = {
    {"Sample menu item", [](){ execute_lab([](auto x) { return 0; }); }},
};

int main(int argc, char* argv[])
{
    srand(time(0));

    Menu main_menu = Menu(main_menu_options, "Pick option");

    main_menu.run(true);



    return 0;
}