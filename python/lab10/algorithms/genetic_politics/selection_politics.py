from ..population_updaters import wheel, tournament

from .utils import politics_template, set_methods


methods_dict = {
    "wheel": wheel,
    "tournament": tournament,
}

__selection_politics = set_methods(politics_template, methods_dict)


class selection_politics(metaclass=__selection_politics):
    pass
