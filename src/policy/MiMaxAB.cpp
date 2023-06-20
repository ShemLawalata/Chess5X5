#include <cstdlib>
#include<iostream>
using namespace std;
#include "../state/state.hpp"
#include "../policy/MiMaxAB.hpp"

int MiMaxAB::Alpha_Beta(State* state, int depth, int alpha, int beta, bool MaxiMe)
{
    if(depth == 0) return state->evaluate();
    //if (state->game_state == WIN) return state->evaluate() + 400;
  
    else if(MaxiMe){
        int value = -2100000000;
        state->get_legal_actions();
        for (auto move : state->legal_actions)//&
        {
            State* child = state->next_state(move);
            value = max(value, Alpha_Beta(child, depth-1, alpha, beta, false));//state should be the child one
            alpha = max(alpha, value);
            if(alpha >= beta) break;
        }
        return value;
    }
    else{
        int value = 2100000000;
        state->get_legal_actions();
        for(auto move : state->legal_actions){
            State* child = state->next_state(move);
            value = min(value, Alpha_Beta(child, depth-1, alpha, beta, true));
            beta = min(value, beta);
            if(beta <= alpha)break;
        }
        return value;
    }
}