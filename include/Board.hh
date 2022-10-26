#ifndef BOARD_HH
#define BOARD_HH

#include "Cell.hh"
#include "Movement.hh"
#include <vector>

class Board {
private:
  // matriz
  vector<vector<Cell>> boxes;
  int player1Score, player2Score;

public:
  // los size son el numero de columnas o filas de puntos.
  Board(int rowSize, int colSize);
  ~Board();
  // Recorre el vector para darle los puntos de cada caja que posee cada
  // jugador.
  // Puntuacion individual (puede ser opcional dependiendo de quien la ocupe).
  int getScoreP1();
  int getScoreP2();
  void increaseScore(enum OwnerType player, int diff);
  // Cantidad de filas y columnas.
  int getBoardRowSize();
  int getBoardColSize();
  // xPos y yPos son indices en el tablero.
  Cell *getCell(int xPos, int yPos);
  // jugadas disponibles.
  vector<class Movement> getAvailableMoves();
};

#endif