import os

os.environ[
    "DATASETS_PATH"
] = r"C:\Users\rp-re\OneDrive\Desktop\rep\shame_on_me\datasets"

from algorithms.individual import PermutationIndividual, init_individ
from algorithms.algorithm import BaseEvolutionAlgo

from dataclasses import dataclass

from utils import TravellingSalesmanDataSets, calc_path_length
import numpy as np


from algorithms.genetic_politics import (
    genetic_politics,
    heuristic_politics,
    crossover_politics,
    pairs_generation_politics,
    mutation_politics,
    selection_politics,
)
from algorithms.genetic_politics.mutation_politics import (
    methods_dict as mutation_methods_dict,
)
from algorithms.genetic_politics.pairs_generation_politics import (
    methods_dict as pg_methods_dict,
)

from algorithms.utils import Picker, rest


def build_meta(weights):
    def score_fn(arr):
        return calc_path_length(weights, arr, is_pure_perm=True)

    Individ = init_individ(PermutationIndividual, len(weights), score_fn)
    return Individ, score_fn


@dataclass
class Option:
    text: str
    action: object


def options(options, header=None, raw=False):
    if header is None:
        header = "Pick option"
    to_print = header + ":\n"
    for i, option in enumerate(options):
        to_print += f"\t{i + 1}. {option.text}\n"
    if not raw:
        to_print += f"\t{len(options) + 1}. Back"
    print(to_print)
    ans = input()
    if raw:
        return ans
    try:
        ans = int(ans)
        if ans > len(options) + 1 or ans < 1:
            raise RuntimeError(
                f"Answer index is out of bounce: Expected value in range [1, {len(options) + 1}] got {ans}"
            )
        if ans == len(options) + 1:
            return -1
        options[ans - 1].action()
    except:
        return ans


settings = {}


def setup():
    global settings
    return_code = options(
        [
            Option(
                "Simple 5 cities dataset (optimum: 23)",
                lambda *args: settings.update(
                    {"weights": TravellingSalesmanDataSets.simple_5x5}
                ),
            ),
            Option(
                "Simple 15 cities dataset (optimum: 43.45)",
                lambda *args: settings.update(
                    {"weights": TravellingSalesmanDataSets.tsp_dataset}
                ),
            ),
            Option(
                "Simple 17 cities dataset (optimum: 2085)",
                lambda *args: settings.update(
                    {"weights": TravellingSalesmanDataSets.tsp_dataset}
                ),
            ),
            Option(
                "Simple 47 cities dataset (optimum: 33523)",
                lambda *args: settings.update(
                    {"weights": TravellingSalesmanDataSets.gr_47}
                ),
            ),
        ],
        header="Select dataset or print its filename",
    )
    if return_code is -1:
        return return_code
    elif isinstance(return_code, str):
        settings.update({"weights": getattr(TravellingSalesmanDataSets, return_code)})

    settings["base_population_size"] = int(len(settings["weights"]) * 5.5)
    settings["individual_type"], settings["score_fn"] = build_meta(settings["weights"])

    politics = {}
    _heuristic_politics = {}
    return_code = options(
        [
            Option(
                "MonteCarlo",
                lambda *args: _heuristic_politics.update(
                    heuristic_politics[["MonteCarlo"]]
                ),
            ),
            Option(
                "MonteCarlo + Greedy",
                lambda *args: _heuristic_politics.update(
                    heuristic_politics[["MonteCarlo", "NearestCity"]]
                ),
            ),
        ],
        header="Select method to fill base population with",
    )
    politics = {"heuristic_politics": _heuristic_politics}

    _pg_politics = {}
    return_code = options(
        [
            Option(
                "Inbriding",
                lambda *args: _pg_politics.update(
                    {
                        "methods": Picker([(pg_methods_dict["inbriding"], 1)]),
                        "nbest": 5,
                    }
                ),
            ),
            Option(
                "Outbriding",
                lambda *args: _pg_politics.update(
                    {
                        "methods": Picker([(pg_methods_dict["outbriding"], 1)]),
                        "nbest": 5,
                    }
                ),
            ),
            Option(
                "Tournament",
                lambda *args: _pg_politics.update(
                    {
                        "methods": Picker([(pg_methods_dict["tournament"], 1)]),
                        "nbest": 5,
                    }
                ),
            ),
        ],
        header="Select method to fill base population with",
    )
    politics["pairs_generation_politics"] = _pg_politics

    _crossover_methods = {}
    return_code = options(
        [
            Option(
                "PMX crossover",
                lambda *args: _crossover_methods.update(
                    crossover_politics[["pmx_crossover"]]
                ),
            ),
            Option(
                "Cycle crossover",
                lambda *args: _crossover_methods.update(
                    crossover_politics[["cycle_crossover"]]
                ),
            ),
        ],
        header="Select crossover method",
    )
    politics["crossover_politics"] = _crossover_methods

    _mutation_methods = {}
    return_code = options(
        [
            Option(
                "Reverse mutation",
                lambda *args: _mutation_methods.update(
                    {
                        "methods": Picker(
                            [(mutation_methods_dict["reverse_mutation"], 1)]
                        ),
                    }
                ),
            ),
            Option(
                "Swap mutation",
                lambda *args: _mutation_methods.update(
                    {
                        "methods": Picker(
                            [(mutation_methods_dict["swap_mutation"], 1)]
                        ),
                    }
                ),
            ),
        ],
        header="Select mutation method",
    )
    return_code = float(options([], header="Enter mutation chance", raw=True))
    _mutation_methods["mutation_rate"] = return_code
    politics["mutation_politics"] = _mutation_methods

    _selection_methods = {}
    return_code = options(
        [
            Option(
                "Wheel",
                lambda *args: _selection_methods.update(selection_politics[["wheel"]]),
            ),
            Option(
                "Tournament",
                lambda *args: _selection_methods.update(
                    selection_politics[["tournament"]]
                ),
            ),
        ],
        header="Select selection method",
    )
    politics["selection_politics"] = _selection_methods

    settings["genetic_politics"] = politics.copy()

    return_code = int(options([], header="Enter verbose level", raw=True))
    settings["verbose_level"] = return_code

    return 0


setup()
n_steps = int(input("Input number of steps\n"))

with BaseEvolutionAlgo(**settings) as algo:
    algo.execute(n_steps)
