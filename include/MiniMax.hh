#ifndef MINIMAX_HH
#define MINIMAX_HH

#include "Board.hh"
#include "Cell.hh"
#include "Movement.hh"
using namespace std;

class MiniMax {
 private:
  bool maxPlayer;
  int myDepth;
  Movement bestMove;
  vector<Movement> availableMoves;
  Board myBoard;
  void setBestMove(Movement move);
  int initialDepth;

 public:
  MiniMax(Board board, bool isMaxPlayer, int depth);
  MiniMax(Board board, bool isMaxPlayer, int depth, int initialDepth);
  short performMiniMax(enum OwnerType max_player, enum OwnerType min_player);
  short performProbabilityMiniMax(bool root);
  // Primera llamada debe ser minimax.performAlfaBeta(true, -15000, 15000);
  short performAlfaBeta(bool root, short alfa, short beta);
  Movement getBestMove();

  ~MiniMax();
};

#endif
