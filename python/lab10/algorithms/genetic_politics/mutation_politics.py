from ..mutations import reverse_mutation, swap_mutation
from ..utils import Picker

methods_dict = {
    "reverse_mutation": reverse_mutation,
    "swap_mutation": swap_mutation,
}

mutation_politics = {
    "light": {
        "methods": Picker([(reverse_mutation, 0.9), (swap_mutation, 0.1)]),
        "mutation_rate": 0.1,
    },
    "kicker": {
        "methods": Picker([(reverse_mutation, 0.5), (swap_mutation, 0.5)]),
        "mutation_rate": 1,
    },
}
