import numpy as np
import os

DATASETS_FOLDER = os.environ.get("DATASETS_FOLDER", os.path.abspath("datasets"))


def path(path, ext):
    return f"{DATASETS_FOLDER}/{path}.{ext}"


class ReaderMeta(type):
    def __getattr__(cls, key):
        if key in dir(cls):
            return object.__getattribute__(cls, key)
        if os.path.isfile(path(key, "txt")):
            return cls.read(path(key, "txt"))
        elif os.path.isfile(path(key, "csv")):
            return cls.read(path(key, "csv"), sep=",")
        else:
            return object.__getattribute__(cls, key)


class BaseDataSet(metaclass=ReaderMeta):
    @staticmethod
    def read(filepath, sep=" "):
        raise NotImplementedError()

    @staticmethod
    def random_dataset(*args, **kwargs):
        raise NotImplementedError()


class TravellingSalesmanDataSets(BaseDataSet):
    @staticmethod
    def read(filepath, sep=" "):
        with open(filepath, "r") as file:
            return np.vstack(
                [
                    list(map(float, line.split(sep)))
                    for line in file
                    if line.strip() != ""
                ]
            )

    simple_5x5 = np.array(
        [
            [0, 4, 3, 2, 5],
            [4, 0, 6, 5, 7],
            [3, 6, 0, 6, 7],
            [2, 5, 6, 0, 5],
            [5, 7, 7, 5, 0],
        ]
    )


class KnackPackDataSets(BaseDataSet):
    simple_5d = (14, np.array([4, 5, 6, 11, 5]), np.array([4, 5, 6, 11, 5]))

    @staticmethod
    def read(filepath, sep=" "):
        values, weights = [], []
        max_weight = 0
        with open(filepath, "r") as file:
            for line in file:
                if line.strip() == "":
                    continue
                items_info = list(map(float, line.split(sep)[1:]))
                if len(items_info) > 0:
                    values.append(items_info[0])
                    weights.append(items_info[1])
                else:
                    # reached last line
                    max_weight = float(line)
        all_items_weight = np.sum(weights)
        if max_weight >= all_items_weight:
            raise RuntimeError(
                f"Data is inconsistent: max_weight >= all_items_weight ({max_weight} >= {all_items_weight})"
            )
        return (max_weight, np.array(values), np.array(weights))


def get_nearest_city(weights, city, visited_mask, return_weight=False):
    masked_min_idx = weights[city][visited_mask].argmin()
    nearest_city = np.where(visited_mask)[0][masked_min_idx]
    if return_weight:
        return nearest_city, weights[city, nearest_city]
    else:
        return nearest_city


def calc_path_length(weights, path):
    res = 0
    for i in range(1, len(path)):
        res += weights[path[i - 1], path[i]]
    return res


def static_vars(**kwargs):
    def wrapper(func):
        for key, value in kwargs.items():
            setattr(func, key, value)
        return func

    return wrapper
