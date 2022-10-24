#ifndef MINIMAX_HH
#define MINIMAX_HH

#include "Board.hh"
#include "Movement.hh"
#include "Cell.hh"
using namespace std;

class MiniMax{
    private: 
        bool maxPlayer;
        int myDepth; 
        Movement bestMove; 
        vector <Movement> availableMoves;       
        Board myBoard; 
        void setBestMove(Movement move);

    public:
        MiniMax(Board board, bool isMaxPlayer, int depth); 
        short performMiniMax(bool root); 
        short performProbabilityMiniMax(bool root);
        //Primera llamada debe ser minimax.performAlfaBeta(true, -15000, 15000);
        short performAlfaBeta(bool root, short alfa, short beta);
        Movement getBestMove();

        ~MiniMax(); 
}; 

#endif

