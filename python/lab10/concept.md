class GA:
	def __init__(
		Population=None,
		IndividualClass=None,
		PopulationClass=None,
		MateFn,
		SelectPairsFn,
		MutationFn,
		SelectNewPopulationFn,
	):
		popu

	def init_algo():
		self._population = create_base_population()
		evo

	evolutin_loop(n) -> (Population):
		init_if_not_inited()
		for i in range(n):
			self.step = i
			pairs = SelectPairs() -> np.array<(Individ, Individ...)]>
			MatePair(pairs, mutation_fn) -> np.array<Individ>
			self._population = CreateNewPopulation(old_population, childrens)
			Verbose()
		return self._population

	MatePair():
		...
		mutation_fn()
		...

	solve() -> (best_solution, best_score):
		while not stop_condition():
			evolution_loop(1)
		return best()

	def __exit__():
		output()

	