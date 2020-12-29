from .heuristic_politics import heuristic_politics
from .pairs_generation_politics import pairs_generation_politics
from .crossover_politics import crossover_politics, crossover_pc_politics
from .mutation_politics import mutation_politics
from .selection_politics import selection_politics

from copy import deepcopy

basic_politics = {
    "normal": {
        "heuristic_politics": heuristic_politics[["MonteCarlo"]],
        "pairs_generation_politics": pairs_generation_politics["light"],
        "crossover_politics": crossover_pc_politics["basic"],
        "mutation_politics": mutation_politics["light"],
        "selection_politics": selection_politics[["tournament"]],
    },
    "stuck": {
        "pairs_generation_politics": pairs_generation_politics["kicker"],
        "mutation_politics": mutation_politics["kicker"],
    },
}

basic_greedy_politics = deepcopy(basic_politics)
basic_greedy_politics["normal"]["heuristic_politics"] = heuristic_politics[
    ["MonteCarlo", "NearestCity"]
]

basic_dfs_politics = deepcopy(basic_politics)
basic_dfs_politics["normal"]["heuristic_politics"] = heuristic_politics[["DFS"]]

basic_dfs_greedy_politics = deepcopy(basic_politics)
basic_dfs_greedy_politics["normal"]["heuristic_politics"] = heuristic_politics[
    ["DFS", "NearestCity"]
]

p1 = deepcopy(basic_politics)
p1["normal"]["selection_politics"] = selection_politics[["tournament", "wheel"]]

p2 = deepcopy(basic_politics)
p2["normal"]["selection_politics"] = selection_politics[["wheel"]]

p3 = deepcopy(basic_greedy_politics)
p3["normal"]["selection_politics"] = selection_politics[["tournament", "wheel"]]

p4 = deepcopy(basic_greedy_politics)
p4["normal"]["selection_politics"] = selection_politics[["wheel"]]


genetic_politics = {
    "Basic": basic_politics,
    "Greedy": basic_greedy_politics,
    "DFS": basic_dfs_politics,
    "DFS, Greedy": basic_dfs_greedy_politics,
    "Monte_Wheel": p2,
    "Monte_WheelTournament": p1,
    "Monte_Tournament": basic_politics,
    "Greedy_Wheel": p4,
    "Greedy_WheelTournament": p3,
    "Greedy_Tournament": basic_greedy_politics,
}
