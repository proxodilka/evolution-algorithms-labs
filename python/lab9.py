import numpy as np

from utils import (
    static_vars,
    KnackPackDataSets,
)

max_w, value, weights = KnackPackDataSets.knackpack_dataset


def solve(weights, values, max_w):
    c_values = [v / w for v, w in zip(values, weights)]
    print(f"Value per weight: {c_values}\n\n")

    sorted_ids = np.argsort(c_values)[::-1]
    result = np.zeros(len(values), dtype=np.int)
    weight, step, value = 0, 0, 0

    for i in range(len(values)):
        to_grab_idx = sorted_ids[i]
        to_grab_weight = weights[to_grab_idx]

        if weight + to_grab_weight > max_w:
            continue

        result[to_grab_idx] = 1
        weight += to_grab_weight
        value += values[to_grab_idx]

        verbose(step, result, to_grab_idx, weight, value)
        step += 1

    return result


@static_vars(weight=0, value=0)
def verbose(index, knackpack, picked_item, weight, value):
    print(
        f"Step {index}:\n\tItem {picked_item} was picked, weight of item: "
        + f"{weight - verbose.weight}, its value: {value - verbose.value}"
        + f"\n\tCurrent knackpack: {knackpack}, its weight: {weight}, its value: {value}"
    )
    verbose.weight = weight
    verbose.value = value


if __name__ == "__main__":
    solve(weights, value, max_w)
