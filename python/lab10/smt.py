import numpy as np


class some(type):
    def __add__(cls, other):
        return other

    def __radd__(cls, other):
        return other

    def __int__(cls):
        return 0

    def __float__(cls):
        return 0.0


class Some(metaclass=some):
    pass
    # def __add__(self, other):
    #     return other


# breakpoint()
2 + Some
print(int(Some))
print(np.sum([2, Some, 0, 1]))
