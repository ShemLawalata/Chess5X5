#pragma once
#include "../state/state.hpp"

class MiMaxAB{
    public:
    static int Alpha_Beta(State* state, int depth, int alpha, int beta, bool MaxiMe);
};