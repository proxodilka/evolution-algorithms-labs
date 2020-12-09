import numpy as np

from utils import (
    static_vars,
    TravellingSalesmanDataSets,
    get_nearest_city,
    calc_path_length,
)

weights = TravellingSalesmanDataSets.simple_5x5


def get_candidates(weights, path, visited_mask):
    return [
        get_nearest_city(weights, x, visited_mask, return_weight=True)[::-1]
        for x in path
    ]


def solve(weights, start_city):
    n_cities = weights.shape[0]
    result = np.zeros(n_cities + 1, dtype=np.int)
    visited_mask = ~np.zeros(n_cities, dtype=np.bool)

    result = [start_city]
    visited_mask[start_city] = False
    path_length = 0

    for i in range(1, n_cities):
        candidates = get_candidates(weights, result, visited_mask)
        best_candidate_idx = np.argmin(candidates, axis=0)[0]

        to_insert = candidates[best_candidate_idx][1]

        result = np.concatenate(
            [
                result[: best_candidate_idx + 1],
                [to_insert],
                result[best_candidate_idx + 1 :],
            ]
        ).astype(np.int)
        visited_mask[to_insert] = False

        path_length = calc_path_length(weights, result)
        verbose(i, candidates, result, to_insert, path_length)

    result = np.concatenate([result, [start_city]])
    path_length = calc_path_length(weights, result)
    verbose(len(result) - 1, [[0]], result, start_city, path_length)
    return result


@static_vars(previous_path=[0], path_length=0)
def verbose(index, candidates, current_path, picked_city, path_length):
    candidates_str = " | ".join(
        [
            f"{verbose.previous_path[i]}: {v[::-1]}"
            if not i or i % 5
            else f"{i}:{v[::-1]}\n\t\t"
            for i, v in enumerate(candidates)
        ]
    )
    print(
        f"Step {index}:\n\tCandidates: {candidates_str}\n\tCity {picked_city} was picked, path lenght delta: "
        + f"{path_length - verbose.path_length}\n\tCurrent path: {current_path[:index + 1]}, its length: {path_length}"
    )
    verbose.path_length = path_length
    verbose.previous_path = current_path


if __name__ == "__main__":
    print(weights)
    solve(weights, 0)
