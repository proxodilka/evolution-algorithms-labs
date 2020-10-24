#pragma once

#include <vector>

namespace SerachFields {

template<typename T>
class Vector : public std::vector<T> {
    using std::vector<T>::vector;
};

} // namespace SerachFields
