#ifndef MOVEMENT_HH
#define MOVEMENT_HH

#include "Board.hh"

enum Directions { WEST, EAST, NORTH, SOUTH, EMPTY };

// Clase "Jugada".
class Movement {
private:
  // xPos y yPos son indices en la matriz
  int xPos, yPos;
  enum Directions lineDirection;

public:
  Movement(int xPos, int yPos, enum Directions lineDirection);
  ~Movement();
  int getXPos();
  int getYPos();
  enum Directions getLineDirection();

  bool play(class Board &currentBoard, enum OwnerType owner);
  OwnerType playAndAssignOwner(Board& currentBoard, enum OwnerType owner);
};

#endif