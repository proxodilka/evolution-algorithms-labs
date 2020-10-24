#pragma once

#include <climits>
#include <vector>
#include <unordered_map>
#include <memory>

namespace Solver{

class IntProxy {
    bool _is_updated;
    int _value;
public:
    IntProxy(int value): _value(value), _is_updated(false) {}

    int value() { return _value; }
    bool is_updated() { return _is_updated; }
    void clear() { _is_updated = false; }

    operator int() {return value(); }

    IntProxy& operator=(const IntProxy& other) {
        _is_updated = true;
        _value = other._value;
        return *this;
    }
};

template<typename Vector>
class VerboseUnit {
    using Variant = std::pair<Vector, int>;

    std::shared_ptr<std::vector<Vector>> variants;
    std::shared_ptr<std::vector<int>> scores;

    int picked_id;

    const bool empty;
public:
    VerboseUnit(): empty(true) {}

    VerboseUnit(const Variant picked_variant): variants(std::make_shared<Vector>({picked_variant.first})),
        scores(std::make_shared<std::vector<int>>({picked_variant.second})), picked_id(0), empty(false) {}
    
    VerboseUnit(const Vector& solution, int score): 
        variants(std::make_shared<std::vector<Vector>>(std::initializer_list<Vector>({solution}))),
        scores(std::make_shared<std::vector<int>>(std::initializer_list<int>({score}))), picked_id(0), empty(false) {}

    VerboseUnit(std::shared_ptr<std::vector<Vector>> variants, std::shared_ptr<std::vector<int>> scores, int picked_id):
        variants(variants), scores(scores), picked_id(picked_id) {}

    bool is_empty() const { return empty; }
    int size() const { return (*variants).size(); }

    int get_picked_id() const { return picked_id; }

    std::pair<const Vector&, int> operator[](int i) const {
        if (i >= (*variants).size()){
            throw std::invalid_argument("Incorrect index");
        }

        return std::pair<const Vector&, int>((*variants)[i], (*scores)[i]);
    }
};

template<typename Vector, typename Field>
class BaseSolver {
protected:
    std::vector<Vector> population;
    Vector best_solution;
    std::function<int(const Vector&)> scorer;
    Field& field;
    std::shared_ptr<VerboseUnit<Vector>> verbose_unit;

    std::unordered_map<int, int> landscape_fitness;
    std::vector<int> history;

    IntProxy best_score;

    virtual void do_optimization_step(int step) = 0;

    virtual void InitTraining() {
        best_solution = field[::Utils::random(field.__approximate_size())];
        best_score = score(best_solution);

        best_score.clear();
    }

    void PrepareVerboseUnit(Vector best_solution, int best_score) {
        this->verbose_unit = std::make_shared<VerboseUnit<Vector>>(best_solution, best_score); 
    }

    void print_verbose_step(int step, const VerboseUnit<Vector>& verbose_unit, std::ostream& out = std::cout) {
        std::string extra_str = "";
        if (best_score.is_updated()) {
            extra_str = " Optimum was updated on this step!";
            best_score.clear();
        }

        std::string current_step_label = "Current step:    ";
        int step_label_len = current_step_label.size();

        int picked_id = verbose_unit.get_picked_id();
        out << "Step " << step << ":" << extra_str << "\n\tCurrent optimum: " << best_solution << " : " << best_score
            << "\n\t" << current_step_label;

        auto generate_spaces = [](int n) { 
            std::string result;
            result.reserve(n);
            for (int i=0; i<n; i++){
                result += ' ';
            }
            return result;
        };

        auto variant_printer = [&](int step, bool is_picked, auto variant) {
            if (step) {
                out << "\t" << generate_spaces(step_label_len);
            }
            out << variant.first << " : " << variant.second;
            if (is_picked) {
                out << " <--- this variant was picked to compare";
            }
            out << "\n";
        };

        bool is_single_variant = verbose_unit.size() == 1;
        for (int i=0; i<verbose_unit.size(); i++){
            variant_printer(i, (picked_id == i) && !is_single_variant, verbose_unit[i]);
        }
    }

    void print_verbose_result(int steps, std::ostream& out = std::cout) {
        out << "\nBest solution that was found for " << steps << " steps:\n\tCurrent optimum: " 
            << best_solution << " : " << best_score << "\n";
    }

public:
    BaseSolver(Field& field, const std::function<int(const Vector&)>& scorer): field(field), scorer(scorer), best_score(-INT_MAX) {}

    void TrainModel(int steps=500, bool verbose=true) {
        history.clear();
        history.reserve(steps);
        InitTraining();

        for (int i=0; i<steps; i++){
            this->do_optimization_step(i);
            history.push_back(best_score);
            //std::cout << i << std::endl;
            if (verbose){
                this->print_verbose_step(i, *(this->verbose_unit));
            }
        }
        this->print_verbose_result(steps);
    }

    int score(const Vector& vector) { return this->scorer(vector); }

    const std::unordered_map<int, int>& GetLandscapeFitness() {
        if (landscape_fitness.size() > 0) {
            return landscape_fitness;
        }

        for (auto [it, i] = std::tuple({field.begin(), 0}); ++it, ++i; it != field.end()) {
            landscape_fitness.insert({i, score(*it)});
        }

        return landscape_fitness;
    }
};

template<typename Vector, typename Field>
class BaseBinarySolver : public BaseSolver<Vector, Field> {
protected:
    const typename Field::Alphabet& alphabet;

    void PrepareVerboseUnit(std::shared_ptr<Vector> candidates, int candidate_id) {
        if (!this->verbose){
            return;
        }

        std::vector<int> scores;;

        int size = (*candidates).size();
        scores.reserve(size);
        for (int i=0; i<size; i++){
            scores.push_back(this->score((*candidates)[i]));
        }

        this->verbose_unit = std::make_shared<VerboseUnit<Vector>>(candidates, std::make_shared<std::vector<int>>(std::move(scores)), candidate_id);

    }

public:
    BaseBinarySolver(Field& field, const std::function<int(const Vector&)>& scorer): 
                        BaseSolver<Vector, Field>(field, scorer), alphabet(field.get_alphabet()) {
        if (field.get_alphabet().size() != 2) {
            delete this; // since throwing exception in an object constructor don't triggers
                         // objects destructor to be executed, calling destructor explicitly
            throw std::invalid_argument("Can't construct BinarySolver with Field whose alphabet is not binary.");
        }
    }

};

} // namespace Solver