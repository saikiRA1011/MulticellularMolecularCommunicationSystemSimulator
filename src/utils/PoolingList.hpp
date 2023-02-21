#pragma once

#include <vector>

template<class T>
class PoolingVector : public std::vector
{
    private:
    int32_t tail = 0;

    public:
    T& operator[](int32_t n) override
};