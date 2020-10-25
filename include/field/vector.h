#pragma once

#include <vector>

namespace SearchFields {

template<typename T>
class Vector : public std::vector<T> {
    using std::vector<T>::vector;
};

} // namespace SearchFields
