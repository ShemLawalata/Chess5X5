#include <iostream>
#include <fstream>
#include<list>
#include<queue>
#include<set>
#include "../config.hpp"
#include "../state/state.hpp"
#include "../policy/MiMaxAB.hpp"
using namespace std;
State* root;
queue<list<State*> > New_list;
list<State*>To_explore;
set<State*> Explored;
void read_board(std::ifstream& fin) {
  Board board;
  int player;
  fin >> player;

  for (int pl=0; pl<2; pl++) {
    for (int i=0; i<BOARD_H; i++) {
      for (int j=0; j<BOARD_W; j++) {
        int c; fin >> c;
        // std::cout << c << " ";
        board.board[pl][i][j] = c;
      }
      // std::cout << std::endl;
    }
  }
  root = new State(board, player);
  root->get_legal_actions();//Each board or state expanded by many move "board"
  //cout << "masuk\n"; 

}

void write_best_spot(std::ofstream& fout) {
    //Generating all the chances by all legal moves
    int Val = -2100000000;
    Move best_move, last_best;
    MiMaxAB generator;
    for (auto moves : root->legal_actions)
    {
        State* kid_state = root->next_state(moves);
        int compare = generator.Alpha_Beta(kid_state, 5, -2100000000, 2100000000, false);
        if (compare > Val)
        {
            last_best = best_move;
            best_move = moves;
            Val = compare;
        }
        
    Move mov;
    if (best_move.first == best_move.second) mov = last_best;
    else mov = best_move;
    
    fout << mov.first.first << " " << mov.first.second << " "\
         << mov.second.first << " " << mov.second.second << std::endl;
    
    fout.flush();
    }
    
}

  // Keep updating the output until getting killed.
//   while(true) {
//     // Choose a random spot.
//     auto move = Random::get_move(root, 0);
//     fout << move.first.first << " " << move.first.second << " "\
//          << move.second.first << " " << move.second.second << std::endl;
    
//     // Remember to flush the output to ensure the last action is written to file.
//     fout.flush();
//     break;
//   }

/**
 * @brief Main function for player
 * 
 * @param argv 
 * @return int 
 */
int main(int, char** argv) {
  srand(RANDOM_SEED);
  std::ifstream fin(argv[1]);
  std::ofstream fout(argv[2]);

  read_board(fin);
  write_best_spot(fout);

  fin.close();
  fout.close();
  return 0;
}