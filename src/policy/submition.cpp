#include <cstdlib>
#include<iostream>
using namespace std;
#include "../state/state.hpp"
#include "../policy/MiMaxAB.hpp"

int MiMaxAB::Alpha_Beta(State* state, int depth, int alpha, int beta, bool MaxiMe)
{ 
    if(!MaxiMe){
         int value = 2100000000;
         //Checking and switching to enemy perspective
         State* enemy_view = new State(state->board, 1-state->player);
         if(!depth) return enemy_view->evaluate();
         //still we control but in enemy pov
         //So we don't go to the "Path of Death"
         if (state->game_state == WIN) return enemy_view->evaluate() - 400;
         

        state->get_legal_actions();
        for(auto move : state->legal_actions){
            State* child = state->next_state(move);
            value = min(value, Alpha_Beta(child, depth-1, alpha, beta, true));
            beta = min(value, beta);
            if(beta <= alpha)break;
        }
        return value;
        
    }
    //Maximizing
    else{
       int value = -2100000000;
       if(!depth) return state->evaluate();
       if(state->game_state == WIN)return state->evaluate()+400;


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
}
