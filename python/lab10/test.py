import os
from pathlib import Path
from algorithms.individual import PermutationIndividual, init_individ
from algorithms.algorithm import BaseEvolutionAlgo


from utils import TravellingSalesmanDataSets, calc_path_length
import numpy as np

SEEDS = [0, 42, 496, 8128, 33_550_336]

import pickle

from algorithms.genetic_politics.genetic_politics import genetic_politics
from utils import id_generator


def solve_task(name, initial, gp, **kwargs):
    weights = getattr(TravellingSalesmanDataSets, name)
    Individ, score_fn = build_meta(weights)
    base_population_size = int(len(weights) * 5.5)

    populations_path = Path(
        os.environ.get("POPULATIONS_PATH", os.path.abspath("population_dumps"))
    ).joinpath("_".join([name, initial]))
    if populations_path.is_file():
        with open(populations_path, "rb") as file:
            base_population = pickle.load(file)
    else:
        base_population = None

    with BaseEvolutionAlgo(
        base_population=base_population,
        base_population_size=base_population_size,
        individual_type=Individ,
        score_fn=score_fn,
        genetic_politics=gp,
        weights=weights,
        verbose_level=1,
        log=True,
        **kwargs,
    ) as algo:
        if base_population is None:
            population = algo.generate_population()
            algo.base_population = population
            with open(populations_path, "wb") as file:
                pickle.dump(population, file)
        algo.execute(450)


def build_meta(weights):
    def score_fn(arr):
        return calc_path_length(weights, arr, is_pure_perm=True)

    Individ = init_individ(PermutationIndividual, len(weights), score_fn)
    return Individ, score_fn


tasks = [
    # {"name": "att48", "optimum": 33523, "greedy solution": 37928},
    {"name": "DANTZIG42", "optimum": 699, "greedy solution": 864},
]
methods = [("Basic", "monte"), ("Greedy", "greedy")]

reports_storage = Path(r"C:\Users\rp-re\OneDrive\Desktop\logs\reports")


def prepare_report(task, politic_name, method, filenames):
    import pandas
    from timeit import default_timer as timer
    from matplotlib import pyplot as plt

    report_name = "_".join([task["name"], politic_name, method, str(timer())])
    report_folder = reports_storage.joinpath(report_name)
    report_folder.mkdir(exist_ok=False)

    concat_keys = [f"run-{i}" for i in range(len(filenames))]
    df = pandas.concat(
        [pandas.read_csv(fname) for fname in filenames], axis=1, keys=concat_keys
    )
    df.to_csv(report_folder.joinpath("raw_data.csv"))

    df.columns = df.columns.droplevel(0)
    mean_df = pandas.DataFrame({col: df[col].mean(axis=1) for col in df.columns})
    mean_df.to_csv(report_folder.joinpath("mean_data.csv"), index=False)

    min_df = pandas.DataFrame({col: df[col].min(axis=1) for col in df.columns})
    min_df.to_csv(report_folder.joinpath("min_data.csv"), index=False)

    mean_df["optimum"], min_df["optimum"] = task["optimum"], task["optimum"]
    mean_df["greedy solution"], min_df["greedy solution"] = (
        task["greedy solution"],
        task["greedy solution"],
    )

    fig1, ax1 = plt.subplots()
    mean_df["mean solution"] = mean_df["solution"]
    mean_df[["mean solution", "optimum", "greedy solution"]].plot(ax=ax1)
    ax1.set_xlabel("Generation")
    ax1.set_ylabel("Adaptation")
    fig1.savefig(report_folder.joinpath("mean_plot"))

    fig2, ax2 = plt.subplots()
    min_df[["solution", "optimum", "greedy solution"]].plot(ax=ax2)
    ax2.set_xlabel("Generation")
    ax2.set_ylabel("Adaptation")
    fig2.savefig(report_folder.joinpath("min_plot"))


politics = [
    ("Monte_Wheel", "monte"),
    # ("Monte_WheelTournament", "monte"),
    # ("Monte_Tournament", "monte"),
    # ("Greedy_Wheel", "greedy"),
    # ("Greedy_WheelTournament", "greedy"),
    # ("Greedy_Tournament", "greedy"),
]

logs_dir = r"C:\Users\rp-re\Documents\EGA_LOGS"
for task in tasks:
    for gp in politics:
        filenames = [id_generator(12) for _ in range(len(SEEDS))]
        for i, seed in enumerate(SEEDS):
            np.random.seed(seed)
            solve_task(
                task["name"],
                gp[1],
                genetic_politics[gp[0]],
                logger_kwargs={"filename": filenames[i]},
            )
        filenames = [Path(logs_dir).joinpath(fname) for fname in filenames]
        prepare_report(task, gp[0], gp[1], filenames)


# deb_fns = [
#     r"C:\Users\rp-re\Documents\EGA_LOGS\2020-12-28 020209.631481.txt",
#     r"C:\Users\rp-re\Documents\EGA_LOGS\2020-12-28 020110.762877.txt",
#     r"C:\Users\rp-re\Documents\EGA_LOGS\2020-12-28 020013.518469.txt",
#     r"C:\Users\rp-re\Documents\EGA_LOGS\2020-12-28 015913.429750.txt",
# ]

# prepare_report(tasks[0], "test_politic", "test_method", deb_fns)
