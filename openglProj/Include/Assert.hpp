#pragma once

#include <stdexcept>

void Assert(bool cond)
{
    if (not cond) {
        throw std::runtime_error("runtime error");
    }
}
