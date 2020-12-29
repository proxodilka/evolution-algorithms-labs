import numpy as np
from .utils import SEED

# np.random.seed(SEED.value)
import abc


class BaseIdividual:
    def __init__(self, gens, score_fn=None):
        self.validate_gens(gens)
        self._gens = gens
        if not hasattr(self, "_score_fn"):
            assert score_fn is not None, "Must pass score_fn"
            self._score_fn = score_fn
        self._recompute_score()

    gens = property(lambda self: self._gens)
    score = property(lambda self: self._score)

    def _recompute_score(self):
        self._score = self._score_fn(self._gens)

    @abc.abstractclassmethod
    def build_random(cls):
        pass

    def distance(self, other):
        return np.count_nonzero(self._gens ^ other._gens)

    @classmethod
    def validate_gens(cls, gens):
        pass

    @classmethod
    def none(cls):
        res = cls([0], lambda x: x)
        res._score = float("inf")
        return res

    def __lt__(self, other):
        return self.score < other.score

    def __le__(self, other):
        return self.score <= other.score

    def __eq__(self, other):
        if isinstance(other, type(self)):
            return np.all(self.gens == other.gens)
        return self.gens == other

    def __ne__(self, other):
        return not self.__eq__(other)

    def __gt__(self, other):
        return self.score > other.score

    def __ge__(self, other):
        return self.score >= other.score

    def __repr__(self):
        return repr(self._gens)

    def __str__(self):
        return str(self._gens)

    def __len__(self):
        return len(self._gens)

    @classmethod
    def shape(cls):
        return cls._size

    def copy(self):
        return type(self)(gens=self._gens.copy(), score_fn=self._score_fn)

    def __setitem__(self, key, value):
        self._gens.__setitem__(key, value)
        self._recompute_score()

    def __getitem__(self, key):
        return self._gens.__getitem__(key)

    # def __getattr__(self, key):
    #     if key.startswith("__array"):
    #         raise AttributeError
    #     return getattr(self._gens, key)


class FloatIndividual(BaseIdividual):
    @classmethod
    def build_random(cls, size, score_fn, low=None, high=None):
        low = low if low else np.iinfo(np.int32).min
        high = high if high else np.iinfo(np.int32).max

        return cls(gens=np.random.randint(low, high, size), score_fn=score_fn)


class PermutationIndividual(BaseIdividual):
    @classmethod
    def build_random(cls, size=None, score_fn=None):
        size, score_fn = getattr(cls, "_size", size), getattr(
            cls, "_score_fn", score_fn
        )
        return cls(gens=np.random.permutation(size), score_fn=score_fn)

    def hood(self):
        s_len = len(self)
        result = []
        for i in range(s_len):
            # skip generation of the same individual
            if i == s_len - i - 1:
                continue
            to_insert = self.copy()
            to_insert[i], to_insert[s_len - i - 1] = (
                to_insert[s_len - i - 1],
                to_insert[i],
            )
            result.append(to_insert)
        return result

    @classmethod
    def is_permutation(cls, array):
        return (
            np.min(array) == 0
            and np.max(array) == (len(array) - 1)
            and len(array) == len(np.unique(array))
        )

    @classmethod
    def validate_gens(cls, gens):
        if not cls.is_permutation(gens):
            raise RuntimeError(f"Passed sequence is not permutation: {gens}")

    def _recompute_score(self):
        if not self.is_permutation(self._gens):
            self._score = float("inf")
        else:
            super()._recompute_score()


def init_individ(cls, size, score_fn):
    setattr(cls, "_size", size)
    setattr(cls, "_score_fn", lambda cls, *args, **kwargs: score_fn(*args, **kwargs))
    return cls
