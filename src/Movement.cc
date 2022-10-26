#include "Movement.hh"

Movement::Movement(int xPos, int yPos, enum Directions lineDirection)
    : xPos(xPos), yPos(yPos), lineDirection(lineDirection), valid(true) {}

Movement::Movement(bool valid) : valid(valid) {}

Movement::~Movement() {}

int Movement::getXPos() { return xPos; }

int Movement::getYPos() { return yPos; }

enum Directions Movement::getLineDirection() { return lineDirection; }

// Metodo "Jugar":
bool Movement::play(class Board &currentBoard, enum OwnerType owner) {
  if (!currentBoard.getCell(xPos, yPos)) {
    return false;
  }
  switch (lineDirection) {
  case WEST:
    if (currentBoard.getCell(xPos, yPos)->west != NO_OWNER) {
      return false;
    }
    currentBoard.getCell(xPos, yPos)->west = owner;
    if (yPos > 0) {
      currentBoard.getCell(xPos, yPos - 1)->east = owner;
    }
    break;
  case EAST:
    if (currentBoard.getCell(xPos, yPos)->east != NO_OWNER) {
      return false;
    }
    currentBoard.getCell(xPos, yPos)->east = owner;
    if (yPos < currentBoard.getBoardColSize() - 1) {
      currentBoard.getCell(xPos, yPos + 1)->west = owner;
    }
    break;
  case NORTH:
    if (currentBoard.getCell(xPos, yPos)->north != NO_OWNER) {
      return false;
    }
    currentBoard.getCell(xPos, yPos)->north = owner;
    if (xPos > 0) {
      currentBoard.getCell(xPos - 1, yPos)->south = owner;
    }
    break;
  case SOUTH:
    if (currentBoard.getCell(xPos, yPos)->south != NO_OWNER) {
      return false;
    }
    currentBoard.getCell(xPos, yPos)->south = owner;
    if (xPos < currentBoard.getBoardRowSize() - 1) {
      currentBoard.getCell(xPos + 1, yPos)->north = owner;
    }
    break;

  default:
    return false;
  }
  return true;
}

/*
Función creada a la medida de MiniMax. Se encarga de realizar la jugada y en caso de que se complete
la celda devuelve el dueño de la esta. */
OwnerType Movement::playAndAssignOwner(Board& currentBoard, enum OwnerType owner) {
    Cell* currentCell = currentBoard.getCell(xPos, yPos);
    int completados = 0;
    switch (lineDirection) {
    case WEST:
        if (currentCell->west != NO_OWNER) {
            return NO_VALID; //TODO
        }
        currentCell->west = owner;
        completados += currentCell->boxChecker(owner) ? 1 : 0;
        if (yPos > 0) {
            currentBoard.getCell(xPos, yPos - 1)->east = owner; 
            completados += currentBoard.getCell(xPos, yPos - 1)->boxChecker(owner) ? 1 : 0;
        }
        break;
    case EAST:
        if (currentCell->east != NO_OWNER) {
            return NO_VALID;
        }
        currentCell->east = owner;
        completados += currentCell->boxChecker(owner) ? 1 : 0;
        if (yPos + 1 < currentBoard.getBoardColSize()) {
            currentBoard.getCell(xPos, yPos + 1)->west = owner;
            completados += currentBoard.getCell(xPos, yPos + 1)->boxChecker(owner) ? 1 : 0;
        }
        break;
    case NORTH:
        if (currentBoard.getCell(xPos, yPos)->north != NO_OWNER) {
            return NO_VALID;
        }
        currentBoard.getCell(xPos, yPos)->north = owner;
        completados += currentCell->boxChecker(owner) ? 1 : 0;
        if (xPos > 0) { 
            currentBoard.getCell(xPos - 1, yPos)->south = owner;
            completados += currentBoard.getCell(xPos - 1, yPos)->boxChecker(owner) ? 1 : 0;
        }
        break;
    case SOUTH:
        if (currentBoard.getCell(xPos, yPos)->south != NO_OWNER) {
            return NO_VALID;
        }
        currentBoard.getCell(xPos, yPos)->south = owner;
        completados += currentCell->boxChecker(owner) ? 1 : 0;

        if (xPos < currentBoard.getBoardRowSize() - 1) { 
            currentBoard.getCell(xPos + 1, yPos)->north = owner;
            completados += currentBoard.getCell(xPos + 1, yPos)->boxChecker(owner) ? 1 : 0;
        }
        break;

    default:
        return NO_OWNER;
    }

    if (completados > 0) {
      currentBoard.increaseScore(owner, completados);
      return owner;
    }

    return NO_OWNER;
}

bool Movement::isValid() { return valid; }