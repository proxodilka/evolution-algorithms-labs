import numpy as np

from utils import (
    static_vars,
    BaseDataSet,
    TravellingSalesmanDataSets,
    get_nearest_city,
    calc_path_length,
)

weights = TravellingSalesmanDataSets.simple_5x5


def solve(weights, start_city):
    n_cities = weights.shape[0]
    result = np.zeros(n_cities + 1, dtype=np.int)
    visited_mask = ~np.zeros(n_cities, dtype=np.bool)

    result[0] = start_city
    visited_mask[start_city] = False
    path_length = 0

    for i in range(1, len(result) - 1):
        city_from = result[i - 1]
        nearest_city = get_nearest_city(weights, city_from, visited_mask)

        result[i] = nearest_city
        visited_mask[nearest_city] = False

        path_length += weights[city_from, nearest_city]
        verbose(i, result, nearest_city, path_length)

    result[-1] = start_city
    path_length += weights[result[-2], result[-1]]
    verbose(len(result) - 1, result, start_city, path_length)
    return result


@static_vars(path_length=0)
def verbose(index, current_path, picked_city, path_length):
    print(
        f"Step {index}:\n\tCity {picked_city} was picked, weight from current city: "
        + f"{path_length - verbose.path_length}\n\tCurrent path: {current_path[:index + 1]}, its length: {path_length}"
    )
    verbose.path_length = path_length


if __name__ == "__main__":
    print(weights)
    solve(weights, 0)
