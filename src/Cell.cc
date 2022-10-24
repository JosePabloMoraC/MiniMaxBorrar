#include "Cell.hh"

Cell::Cell()
    : west(NO_OWNER), east(NO_OWNER), north(NO_OWNER), south(NO_OWNER),
      boxOwner(NO_OWNER) {}

Cell::~Cell() {}

enum OwnerType Cell::getBoxOwner() { return boxOwner; }

enum OwnerType* Cell::getLine(int direction){
  switch (direction)
  {
  case 0:
    return &west;
    break;
  case 1:
    return &east;
    break;
  case 2:
    return &north;
    break;
  case 3:
    return &south;
    break;
  default:
    return nullptr;
    break;
  }
}

bool Cell::boxChecker(enum OwnerType owner) {
  if (east != NO_OWNER && north != NO_OWNER && south != NO_OWNER &&
      west != NO_OWNER && boxOwner == NO_OWNER) {
    boxOwner = owner;
    return true;
  }
  return false;
}

int Cell::availableMovesCount() {
  int count = 0;
  if (east == NO_OWNER) {
    count++;
  }
  if (west == NO_OWNER) {
    count++;
  }
  if (north == NO_OWNER) {
    count++;
  }
  if (south == NO_OWNER) {
    count++;
  }
  return count;
}