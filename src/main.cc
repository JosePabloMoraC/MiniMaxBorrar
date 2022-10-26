#include "MiniMax.hh"
#include <iostream>
#include <time.h>

using namespace std; 
int main() {
	Board board = { 4, 4 };

	Movement(0, 0, WEST).playAndAssignOwner(board, PLAYER1);
	Movement(1, 0, WEST).playAndAssignOwner(board, PLAYER1);
	Movement(1, 0, EAST).playAndAssignOwner(board, PLAYER1);
	Movement(2, 1, EAST).playAndAssignOwner(board, PLAYER1);
	Movement(0, 0, EAST).playAndAssignOwner(board, PLAYER2);
	Movement(0, 1 , EAST).playAndAssignOwner(board, PLAYER2);
	Movement(0, 2, EAST).playAndAssignOwner(board, PLAYER2);

	cout << "Movimientos disponibles en el tablero: " << board.getAvailableMoves().size() << endl;
	// False indica que siguiente movimiento es de Min, profundidad 3.
	MiniMax mm(board, true, 5);
	// siempre que se llama el minimax debe ser true para indicar que es el primer nivel.
	mm.performMiniMax(PLAYER2, PLAYER1);
	Movement bestMove = mm.getBestMove();
	bestMove.playAndAssignOwner(board, PLAYER2);

	// Dirección 3 es "SOUTH"
	cout << "The best move is \nx (row): " << bestMove.getXPos() << "\ny (column): " << bestMove.getYPos()
		<< "\nDirection: " << bestMove.getLineDirection() << endl;
	
    return 0;
}