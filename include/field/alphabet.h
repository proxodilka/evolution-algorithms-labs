#pragma once

#include <vector>
#include <tuple>
#include <unordered_map>
#include <type_traits>

#include "utils/type_helper.h"

namespace SearchFields {

template <typename T>
class Alphabet : public std::vector<T>
{
public:
    using LetterType = T;
    using VectorType = std::vector<T>;

    template <
    typename Container,
    typename = typename std::enable_if<
        std::is_same<typename Utils::determine_container_type<Container>::value, T>::value,
        void>::type>
    Alphabet(const Container& values)
    : VectorType(values.begin(), values.end())
    {
        for (auto [it, i] = std::tuple(values.begin(), 0); it != values.end(); ++it, ++i) {
            _int_to_letter.insert({ i, *it });
            _letter_to_int.insert({ *it, i });
        }
    }


    const LetterType int_to_letter(int i) const { return _int_to_letter.at(i); }
    const int letter_to_int(LetterType let) const { return _letter_to_int.at(let); }
  

private:
    std::unordered_map<int, LetterType> _int_to_letter;
    std::unordered_map<LetterType, int> _letter_to_int;
};

} // namespace SearchFields 
