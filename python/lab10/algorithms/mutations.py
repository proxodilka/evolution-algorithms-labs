import numpy as np
from .utils import SEED, np_array

# np.random.seed(SEED.value)


def swap_mutation(offspring, **kwargs):
    f_pos, s_pos = np.random.choice(len(offspring), 2, replace=False)
    offspring[f_pos], offspring[s_pos] = (
        offspring[s_pos].copy(),
        offspring[f_pos].copy(),
    )


def reverse_mutation(offspring, **kwargs):
    f_pos, s_pos = np.random.choice(len(offspring), 2, replace=False)
    f_pos, s_pos = min(f_pos, s_pos), max(f_pos, s_pos)
    offspring[f_pos:s_pos] = offspring[f_pos:s_pos][::-1]


def mutate(offspring, mutation_rate, method, consider_incest=False):
    if isinstance(offspring, tuple):
        assert (
            len(offspring) == 2
        ), f"Incorrect tuple of offspring and parrents likeness. Excepted lenght 2, got {len(offspring)}."
        offspring, parrents_likeness = offspring
    else:
        parrents_likeness = 0

    incest_rate = parrents_likeness / (len(offspring) * 2)
    mutation_rate = (
        mutation_rate if not consider_incest else (mutation_rate + incest_rate)
    )

    if np.random.rand() < mutation_rate:
        method(offspring, incest_rate=incest_rate)
    return offspring


def get_mutated(offsprings, methods, **kwargs):
    cumind = np.cumsum(
        [0] + [len(offsprings) * m.probability for m in methods.items(len(offsprings))],
        dtype=np.int,
    )
    bins = [slice(cumind[i - 1], cumind[i]) for i in range(1, len(cumind))]

    for i, bin_slice in enumerate(bins):
        for offspring in offsprings[bin_slice]:
            mutate(offspring, method=methods[i].variant, **kwargs)
    return np_array([o[0] for o in offsprings])
