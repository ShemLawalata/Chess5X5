#include <iostream>
#include <sstream>
#include <cstdint>

#include "./state.hpp"
#include "../config.hpp"
using namespace std;

/**
 * @brief evaluate the state
 * 
 * @return int 
 */
int State::evaluate(){
  // [TODO] design your own evaluation function
  int total = 0;
  auto self_board = this->board.board[this->player];
  auto oppn_board = this->board.board[1 - this->player];
  int player_piece = 0, oppn_piece = 0;
  int array_value[7] = {0, 2, 4, 8, 16, 32, 64};//empty -> king
  for(int i=0; i<BOARD_H; i+=1){
    for(int j=0; j<BOARD_W; j+=1){
      player_piece += array_value[(int)self_board[i][j]];
      //oppn_piece += array_value[(int)oppn_board[i][j]];
      if (this->player == 0)//white
      {
        /* code */
      }
      
      // if(int my_piece = self_board[i][j])
      // {
      //   if(i == 3 && j == 2){
      //    if(my_piece == 3) player_piece += 40;
      //    if(my_piece == )
      //   }
      // } 
      //if(i == 3 && j == 1 && (int)self_board[i][j]) player_piece += 20;
     // if(i == 3 && j == 3 && (int)self_board[i][j]) player_piece += 20;
    }
  }
    for(int i=0; i<BOARD_H; i+=1){
    for(int j=0; j<BOARD_W; j+=1){
      oppn_piece += array_value[(int)oppn_board[i][j]];
      
    }
  }
    total = player_piece - oppn_piece;
 //if(total > 0) cout << total << endl;
  return total;
}


/**
 * @brief return next state after the move
 * 
 * @param move 
 * @return State* 
 */
State* State::next_state(Move move){
  Board next = this->board;
  Point from = move.first, to = move.second;
  
  int8_t moved = next.board[this->player][from.first][from.second];
  //promotion for pawn
  if(moved == 1 && (to.first==BOARD_H-1 || to.first==0)){
    moved = 5;
  }
  //Ciak lawan
  if(next.board[1-this->player][to.first][to.second]){
    next.board[1-this->player][to.first][to.second] = 0;
  }
  
  next.board[this->player][from.first][from.second] = 0;
  next.board[this->player][to.first][to.second] = moved;
  
  State* next_state = new State(next, 1-this->player);//ganti gantian board
  
  if(this->game_state != WIN)
    next_state->get_legal_actions(); //Expanding possible moves for next state
  return next_state;
}


static const int move_table_rook_bishop[8][7][2] = {
  {{0, 1}, {0, 2}, {0, 3}, {0, 4}, {0, 5}, {0, 6}, {0, 7}},
  {{0, -1}, {0, -2}, {0, -3}, {0, -4}, {0, -5}, {0, -6}, {0, -7}},
  {{1, 0}, {2, 0}, {3, 0}, {4, 0}, {5, 0}, {6, 0}, {7, 0}},
  {{-1, 0}, {-2, 0}, {-3, 0}, {-4, 0}, {-5, 0}, {-6, 0}, {-7, 0}},
  {{1, 1}, {2, 2}, {3, 3}, {4, 4}, {5, 5}, {6, 6}, {7, 7}},
  {{1, -1}, {2, -2}, {3, -3}, {4, -4}, {5, -5}, {6, -6}, {7, -7}},
  {{-1, 1}, {-2, 2}, {-3, 3}, {-4, 4}, {-5, 5}, {-6, 6}, {-7, 7}},
  {{-1, -1}, {-2, -2}, {-3, -3}, {-4, -4}, {-5, -5}, {-6, -6}, {-7, -7}},
};
static const int move_table_knight[8][2] = {
  {1, 2}, {1, -2},
  {-1, 2}, {-1, -2},
  {2, 1}, {2, -1},
  {-2, 1}, {-2, -1},
};
static const int move_table_king[8][2] = {
  {1, 0}, {0, 1}, {-1, 0}, {0, -1}, 
  {1, 1}, {1, -1}, {-1, 1}, {-1, -1},
};


/**
 * @brief get all legal actions of now state
 * 
 */
void State::get_legal_actions(){
  // [Optional]
  // This method is not very efficient
  // You can redesign it
  this->game_state = NONE;
  std::vector<Move> all_actions;
  auto self_board = this->board.board[this->player];
  auto oppn_board = this->board.board[1 - this->player];
  
  int now_piece, oppn_piece;
  for(int i=0; i<BOARD_H; i+=1){
    for(int j=0; j<BOARD_W; j+=1){
      if((now_piece=self_board[i][j])){
        // std::cout << this->player << "," << now_piece << ' ';
        switch (now_piece){
          case 1: //pawn
            if(this->player && i<BOARD_H-1){
              //black
              if(!oppn_board[i+1][j] && !self_board[i+1][j]) //Move forward by one
                all_actions.push_back(Move(Point(i, j), Point(i+1, j)));
              if(j<BOARD_W-1 && (oppn_piece=oppn_board[i+1][j+1])>0){
                all_actions.push_back(Move(Point(i, j), Point(i+1, j+1)));
                if(oppn_piece==6){
                  this->game_state = WIN;
                  this->legal_actions = all_actions;
                  return;
                }
              }
              if(j>0 && (oppn_piece=oppn_board[i+1][j-1])>0){
                all_actions.push_back(Move(Point(i, j), Point(i+1, j-1)));
                if(oppn_piece==6){
                  this->game_state = WIN;
                  this->legal_actions = all_actions;
                  return;
                }
              }
            }else if(!this->player && i>0){
              //white
              if(!oppn_board[i-1][j] && !self_board[i-1][j])
                all_actions.push_back(Move(Point(i, j), Point(i-1, j)));
              if(j<BOARD_W-1 && (oppn_piece=oppn_board[i-1][j+1])>0){
                all_actions.push_back(Move(Point(i, j), Point(i-1, j+1)));
                if(oppn_piece==6){
                  this->game_state = WIN;
                  this->legal_actions = all_actions;
                  return;
                }
              }
              if(j>0 && (oppn_piece=oppn_board[i-1][j-1])>0){
                all_actions.push_back(Move(Point(i, j), Point(i-1, j-1)));
                if(oppn_piece==6){
                  this->game_state = WIN;
                  this->legal_actions = all_actions;
                  return;
                }
              }
            }
            break;
          
          case 2: //rook
          case 4: //bishop
          case 5: //queen
            int st, end;
            switch (now_piece){
              case 2: st=0; end=4; break; //rook
              case 4: st=4; end=8; break; //bishop
              case 5: st=0; end=8; break; //queen
              default: st=0; end=-1;
            }
            for(int part=st; part<end; part+=1){
              auto move_list = move_table_rook_bishop[part];
              for(int k=0; k<std::max(BOARD_H, BOARD_W); k+=1){
                int p[2] = {move_list[k][0] + i, move_list[k][1] + j};
                
                if(p[0]>=BOARD_H || p[0]<0 || p[1]>=BOARD_W || p[1]<0) break;
                now_piece = self_board[p[0]][p[1]];
                if(now_piece) break;
                
                all_actions.push_back(Move(Point(i, j), Point(p[0], p[1])));
                
                oppn_piece = oppn_board[p[0]][p[1]];
                if(oppn_piece){
                  if(oppn_piece==6){
                    this->game_state = WIN;
                    this->legal_actions = all_actions;
                    return;
                  }else
                    break;
                };
              }
            }
            break;
          
          case 3: //knight
            for(auto move: move_table_knight){
              int x = move[0] + i;
              int y = move[1] + j;
              
              if(x>=BOARD_H || x<0 || y>=BOARD_W || y<0) continue;
              now_piece = self_board[x][y];
              if(now_piece) continue;
              all_actions.push_back(Move(Point(i, j), Point(x, y)));
              
              oppn_piece = oppn_board[x][y];
              if(oppn_piece==6){
                this->game_state = WIN;
                this->legal_actions = all_actions;
                return;
              }
            }
            break;
          
          case 6: //king
            for(auto move: move_table_king){ //Continnue if ketemu ratu or sth di depan
              int p[2] = {move[0] + i, move[1] + j};
              
              if(p[0]>=BOARD_H || p[0]<0 || p[1]>=BOARD_W || p[1]<0) continue;
              now_piece = self_board[p[0]][p[1]];
             // cout << now_piece << " iniloh\n";
              //Not Covered and same axis with Queen, Bishop, and Rook, Knight?
              if(now_piece) continue;//Covered
              else
              {
                  //Check along the axis blocked or not if blocked continue 
                  //else check clash with Q,B, R or K
                  bool clash = false;
                  if (move[0] == 1 && move[1] == 0) //bawah R&Q
                  {
                      for (int i = p[0]+1; i < BOARD_H; i++)
                      {
                         if(self_board[i][p[1]]) break; //castle == save
                         if(oppn_board[i][p[1]] == 5 || oppn_board[i][p[1]] == 2){
                            clash = true;
                            break;
                         }
                      }
                      
                  }
                  else if (move[0] == 0 && move[1] == 1)// kanan R&Q
                  {
                    for (int j = p[1]+1; j < BOARD_W; j++)
                    {
                      if(self_board[p[0]][j])break;
                      if(oppn_board[p[0]][j] == 5 || oppn_board[p[0]][j] == 2){
                        clash = true;
                        break;
                      }
                    }
                    
                  }
                  else if (move[0] == -1 && move[1] == 0)//atas R&Q
                  {
                    for (int i = p[0]-1; i >= 0; i--)
                      {
                         if(self_board[i][p[1]]) break; //castle == save
                         if(oppn_board[i][p[1]] == 5 || oppn_board[i][p[1]] == 2){
                            clash = true;
                            break;
                         }
                      }
                  }
                  else if (move[0] == 0 && move[1] == -1)//kiri R&Q
                  {
                    for (int j = p[1]-1; j >= 0; j--)
                    {
                      if(self_board[p[0]][j])break;
                      if(oppn_board[p[0]][j] == 5 || oppn_board[p[0]][j] == 2){
                        clash = true;
                        break;
                      }
                    }
                  }
                  else if (move[0] == 1 && move[1] == 1)//kanan bawah B&Q
                  {
                    for (int i = p[0]+1, j = p[1]+1; (i < BOARD_H && j < BOARD_W); i++, j++)
                    {
                      if(self_board[i][j])break;
                      if(oppn_board[i][j] == 5 || oppn_board[i][j] == 4){
                        clash = true;
                        break;
                      }
                    }
                    
                  }
                  else if (move[0] == 1 && move[1] == -1)//kiri bawah B&Q
                  {
                    for (int i = p[0]+1, j = p[1]-1; (i < BOARD_H && j >= 0); i++, j--)
                    {
                      if(self_board[i][j])break;
                      if(oppn_board[i][j] == 5 || oppn_board[i][j] == 4){
                        clash = true;
                        break;
                      }
                    }
                  }
                  else if (move[0] == -1 && move[1] == 1)//atas kanan B&Q
                  {
                   for (int i = p[0]-1, j = p[1]+1; (i >= 0 && j < BOARD_W); i--, j++)
                    {
                      if(self_board[i][j])break;
                      if(oppn_board[i][j] == 5 || oppn_board[i][j] == 4){
                        clash = true;
                        break;
                      }
                    }
                  }
                  else if (move[0] == -1 && move[1] == -1)//atas kiri B&Q
                  {
                   for (int i = p[0]-1, j = p[1]-1; (i >= 0 && j >= 0); i--, j--)
                    {
                      if(self_board[i][j])break;
                      if(oppn_board[i][j] == 5 || oppn_board[i][j] == 4){
                        clash = true;
                        break;
                      }
                    }
                  }
                  if(clash)continue;
              }
              
              all_actions.push_back(Move(Point(i, j), Point(p[0], p[1])));
              
              oppn_piece = oppn_board[p[0]][p[1]];
              if(oppn_piece==6){
                this->game_state = WIN;
                this->legal_actions = all_actions;
                return;
              }
            }
            break;
        }
      }
    }
  }
  std::cout << "\n";
  this->legal_actions = all_actions;
}


const char piece_table[2][7][5] = {
  {" ", "♙", "♖", "♘", "♗", "♕", "♔"},
  {" ", "♟", "♜", "♞", "♝", "♛", "♚"}
};
/**
 * @brief encode the output for command line output
 * 
 * @return std::string 
 */
std::string State::encode_output(){
  std::stringstream ss;
  int now_piece;
  for(int i=0; i<BOARD_H; i+=1){
    for(int j=0; j<BOARD_W; j+=1){
      if((now_piece = this->board.board[0][i][j])){
        ss << std::string(piece_table[0][now_piece]);
      }else if((now_piece = this->board.board[1][i][j])){
        ss << std::string(piece_table[1][now_piece]);
      }else{
        ss << " ";
      }
      ss << " ";
    }
    ss << "\n";
  }
  return ss.str();
}


/**
 * @brief encode the state to the format for player
 * 
 * @return std::string 
 */
std::string State::encode_state(){
  std::stringstream ss;
  ss << this->player;
  ss << "\n";
  for(int pl=0; pl<2; pl+=1){
    for(int i=0; i<BOARD_H; i+=1){
      for(int j=0; j<BOARD_W; j+=1){
        ss << int(this->board.board[pl][i][j]);
        ss << " ";
      }
      ss << "\n";
    }
    ss << "\n";
  }
  return ss.str();
}
