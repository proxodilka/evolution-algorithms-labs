#pragma once

#include <climits>
#include <vector>
#include <unordered_map>
#include <memory>
#include "solver/verbose.h"

namespace Solver{

class IntProxy {
    bool _is_updated;
    int64_t _value;
public:
    IntProxy(int64_t value): _value(value), _is_updated(false) {}

    int64_t value() { return _value; }
    bool is_updated() { return _is_updated; }
    void clear() { _is_updated = false; }

    operator int64_t() { return value(); }

    IntProxy& operator=(const IntProxy& other) {
        _is_updated = true;
        _value = other._value;
        return *this;
    }
};


template<typename Vector, typename Field>
class BaseSolver {
protected:
    Vector best_solution;
    std::function<int(const Vector&)> scorer;
    Field& field;
    std::shared_ptr<VerboseUnit<Vector>> verbose_unit;

    std::unordered_map<int64_t, int64_t> landscape_fitness;
    std::vector<int> history;

    IntProxy best_score;

    bool verbose = true;
    bool finished = false;
    virtual void do_optimization_step(int step) = 0;

    virtual void InitTraining() {
        best_solution = field[::Utils::random(field.__approximate_size())];
        best_score = score(best_solution);

        best_score.clear();
    }

    void PrepareVerboseUnit(Vector best_solution, int best_score) {
        this->verbose_unit = std::make_shared<VerboseUnit<Vector>>(best_solution, best_score); 
    }

    void print_verbose_init(std::ostream& out = std::cout){
        if (!this->verbose){
            return;
        }
        out << "Step " << 0 << ": " << "Initial values" << "\n\tCurrent optimum: " << best_solution << " : " << best_score << "\n";
    }

    void print_verbose_step(int step, const VerboseUnit<Vector>& verbose_unit, std::ostream& out = std::cout) {
        if (!this->verbose){
            return;
        }
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

        bool new_step = true;
        auto variant_printer = [&](bool is_picked, auto variant) {
            static int step = 0;
            if (new_step) {
                new_step = false;
                step = 0;
            }
            if (variant.is_empty) {
                return;
            }
            auto variants = variant.value;
            if (step) {
                out << "\t" << generate_spaces(step_label_len);
            }
            out << variants.first << " : " << variants.second;
            if (is_picked) {
                out << " <--- this variant was picked to compare";
            }
            out << "\n";
            step++;
        };

        bool is_single_variant = verbose_unit.size() == 1;
        for (int i=0; i<verbose_unit.size(); i++){
            variant_printer((picked_id == i), verbose_unit[i]);
        }
    }

    void print_verbose_result(int steps, std::ostream& out = std::cout) {
        if (!this->verbose){
            return;
        }
        out << "\nBest solution that was found for " << steps << " steps:\n\tCurrent optimum: " 
            << best_solution << " : " << best_score << "\n";
    }

public:
    BaseSolver(Field& field, const std::function<int(const Vector&)>& scorer): field(field), scorer(scorer), best_score(-INT_MAX) {}

    void TrainModel(int steps=500, bool verbose=true) {
        this->verbose = verbose;
        history.clear();
        history.reserve(steps);
        InitTraining();
        this->print_verbose_init();
        int i = 0;
        for (; i<steps; i++){
            if (finished) {
                break;
            }
            this->do_optimization_step(i);
            history.push_back(best_score);
            this->print_verbose_step(i + 1, *(this->verbose_unit));
        }
        this->print_verbose_result(i);
    }

    int score(const Vector& vector) { return this->scorer(vector); }

    const std::unordered_map<int64_t, int64_t>& GetLandscapeFitness() {
        if (landscape_fitness.size() > 0) {
            return landscape_fitness;
        }

        int64_t i=0;
        for (auto it = field.begin(); it != field.end(); ++it, ++i) {
            landscape_fitness.insert({i, score(*it)});
        }

        return landscape_fitness;
    }

    void PrintFitnessLandScape(int64_t max=32) {
        verbose_unit->PrintFitnessLandScape(GetLandscapeFitness(), field, max);
    }
};

} // namespace Solver