#include "PoolingList.hpp"
#include <stdexcept>

template<class T>
T& PoolingList::operator[](int32_t n) override
{
    if (n < 0 || tail <= n) {
        throw std::range_error("Argument value is out of range of array.")
    }
    return v[n];
}