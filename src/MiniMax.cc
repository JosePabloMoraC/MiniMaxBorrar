#include "MiniMax.hh"

MiniMax::MiniMax(Board board, bool isMaxPlayer, int depth)
    : myBoard(board),
      maxPlayer(isMaxPlayer),
      myDepth(depth),
      bestMove(-1, -1, EMPTY),
      initialDepth(depth) {
  availableMoves = myBoard.getAvailableMoves();
}

MiniMax::MiniMax(Board board, bool isMaxPlayer, int depth, int initialDepth)
    : myBoard(board),
      maxPlayer(isMaxPlayer),
      myDepth(depth),
      bestMove(-1, -1, EMPTY),
      initialDepth(initialDepth) {
  availableMoves = myBoard.getAvailableMoves();
}

MiniMax::~MiniMax() {}

short MiniMax::performMiniMax(enum OwnerType max_player,
                              enum OwnerType min_player) {
  // Considerar al jugador 1 como max.
  short remainingMoves = availableMoves.size();

  // Si es el final del juego:

  /*
if (remainingMoves == 0){
  // Si Max gana, devuelva 1000
   if(deltaScore > 0){
       return 1000;
   }
   // Si max pierde, devuelva -1000;
   if(deltaScore < 0){
       return -1000;
   }
   // Si hay empate, devuelva 0;
   return 0;
}
*/
  // Devuelva el valor heuristico
  if (myDepth == 0 || remainingMoves == 0) {
    // cout << deltaScore << endl;
    //  Calculamos la diferencia de puntajes para saber quien va ganando el
    //  juego.
    short totalCuadros = myBoard.getBoardRowSize() * myBoard.getBoardColSize();
    short scoreP1 = myBoard.getScoreP1();
    short scoreP2 = myBoard.getScoreP2();
    short deltaScore = PLAYER1 == max_player ? scoreP1*2 - scoreP2*3 : scoreP2*2 - scoreP1*3;
    return deltaScore + totalCuadros - scoreP1 - scoreP2;
  }

  short score;
  // Si el jugador es max
  if (maxPlayer) {
    score = -1000;
    for (int i = 0; i < remainingMoves; i++) {
      // Creamos un nuevo tablero para no modificar el "original".
      Board currentBoard = myBoard;
      Movement* currentMove = &availableMoves[i];
      short childScore = -1000;
      // Si al jugar, se complet� una celda entonces el jugador 1 tiene un turno
      // extra.
      OwnerType result =
          currentMove->playAndAssignOwner(currentBoard, max_player);

      MiniMax childMiniMax(currentBoard, result == max_player, myDepth - 1, initialDepth);
      childScore = childMiniMax.performMiniMax(max_player, min_player);

    /*
      if (myDepth == initialDepth) {
        cout << myDepth << "LVL Score: " << childScore << " row " << currentMove->getXPos() << " col "
             << currentMove->getYPos() << " dir "
             << currentMove->getLineDirection() << endl;
      }
      */

      if (childScore > score) {
        score = childScore;
        // Unicamente guardamos el movimiento cuando estamos en el primer nivel
        // del �rbol.
        setBestMove(*currentMove);
      }
    }
    // Si el jugador es mini
  } else {
    score = 1000;
    for (int i = 0; i < remainingMoves; i++) {
      Board currentBoard = myBoard;
      Movement* currentMove = &availableMoves[i];

      short childScore = 1000;
      // Si al jugar, se complet� una celda entonces el jugador 2 tiene un turno
      // extra.
      OwnerType result =
          currentMove->playAndAssignOwner(currentBoard, min_player);

      MiniMax childMiniMax(currentBoard, result != min_player, myDepth - 1, initialDepth);
      childScore = childMiniMax.performMiniMax(max_player, min_player);

      if (childScore < score) {
        score = childScore;
        setBestMove(*currentMove);
      }
    }
  }
  //cout << "Final Score: " << score << " Row: " << getBestMove().getXPos() << " Column: " << getBestMove().getYPos() << " direction: " << getBestMove().getLineDirection() << " LEVEL: " << myDepth << endl;
  return score;
}











short MiniMax::performProbabilityMiniMax(bool root) {
  short remainingMoves = availableMoves.size();

  // Calculamos la diferencia de puntajes para saber quien va ganando el juego.
  short deltaScore = myBoard.getScoreP1() - myBoard.getScoreP2();

  // Si es el final del juego:
  if (remainingMoves == 0 || myDepth == 0) {
    if (deltaScore > 0) {
      return 1;
    }
    if (deltaScore < 0) {
      return -1;
    }
    // Si hay empate, devuelva 0;
    return 0;
  }

  double probability;
  // Si el jugador es max
  if (maxPlayer) {
    probability = 0;
    for (int i = 0; i < remainingMoves; i++) {
      // Creamos un nuevo tablero para no modificar el "original".
      Board currentBoard = myBoard;
      Movement* currentMove = &availableMoves[i];
      short childProbability;
      // Si al jugar, se complet� una celda entonces el jugador 1 tiene un turno
      // extra.
      if (currentMove->playAndAssignOwner(currentBoard, PLAYER1) == PLAYER1) {
        // El siguiente turno corresponde al jugador max.
        MiniMax childMiniMax = {currentBoard, true, myDepth - 1, initialDepth};
        childProbability = childMiniMax.performProbabilityMiniMax(false);
      } else {
        // El siguiente turno corresponde al jugador min.
        MiniMax childMiniMax = {currentBoard, false, myDepth - 1, initialDepth};
        childProbability = childMiniMax.performProbabilityMiniMax(false);
      }
      // Si estamos en el primer nivel, entonces seleccionamos como mejor
      // movimiento al que tiene la probabilidad m�s alta.
      if (root) {
        if (childProbability > probability) {
          probability = childProbability;
          setBestMove(*currentMove);
        }
      } else {
        if (childProbability > 0) {
          // Solo consideramos la probabilidad de ganar de max.
          probability += childProbability;
        }
      }
    }
    // Con esto devolvemos la probabilidad de de ganar si se selecciona ese
    // "nodo".
    return probability / remainingMoves;
  }
  // Si el jugador es mini
  else {
    probability = 1;
    for (int i = 0; i < remainingMoves; i++) {
      Board currentBoard = myBoard;
      Movement* currentMove = &availableMoves[i];

      short childProbability;
      // Si al jugar, se complet� una celda entonces el jugador 2 tiene un turno
      // extra.
      if (currentMove->playAndAssignOwner(currentBoard, PLAYER2) ==
          PLAYER2) {  // TODO: debo agregar este m�todo a clase Movement
        // El siguiente turno corresponde al jugador min.
        MiniMax childMiniMax = {currentBoard, false, myDepth - 1, initialDepth};
        childProbability = childMiniMax.performProbabilityMiniMax(false);
      } else {
        // El siguiente turno corresponde al jugador max.
        MiniMax childMiniMax = {currentBoard, true, myDepth - 1, initialDepth};
        childProbability = childMiniMax.performProbabilityMiniMax(false);
      }

      // Si estamos en el primer nivel, entonces seleccionamos como mejor
      // movimiento al que tiene la probabilidad m�s baja.
      if (root) {
        if (childProbability < probability) {
          probability = childProbability;
          setBestMove(*currentMove);
        }
      } else {
        if (childProbability > 0) {
          // Solo consideramos la probabilidad de ganar de max.
          probability += childProbability;
        }
      }
    }
    // Restar el 1 inicial
    return (probability - 1) / remainingMoves;
  }
}

short MiniMax::performAlfaBeta(bool root, short alfa, short beta) {
  short remainingMoves = availableMoves.size();
  short deltaScore = myBoard.getScoreP1() - myBoard.getScoreP2();

  if (remainingMoves == 0) {
    if (deltaScore > 0) {
      return 1000;
    }
    if (deltaScore < 0) {
      return -1000;
    }
    return 0;
  }

  if (myDepth == 0) {
    return deltaScore;
  }

  short score;
  // Si el jugador es max
  if (maxPlayer) {
    score = -10000;
    for (int i = 0; i < remainingMoves; i++) {
      Board currentBoard = myBoard;
      Movement* currentMove = &availableMoves[i];
      short childScore;

      if (currentMove->playAndAssignOwner(currentBoard, PLAYER1) == PLAYER1) {
        // El siguiente turno corresponde al jugador max.
        childScore = MiniMax(currentBoard, true, myDepth - 1, initialDepth)
                         .performAlfaBeta(false, alfa, beta);
      } else {
        // El siguiente turno corresponde al jugador min.
        childScore = MiniMax(currentBoard, false, myDepth - 1)
                         .performAlfaBeta(false, alfa, beta);
      }

      if (childScore > score) {
        score = childScore;
        // Unicamente guardamos el movimiento cuando estamos en el primer nivel
        // del �rbol.
        if (root) {
          setBestMove(*currentMove);
        }
      }
      // Si score es mayor que alfa, actualizamos el valor.
      if (score > alfa) {
        alfa = score;
      }

      if (score >= beta) {
        break;
      }
    }
    return score;
  }
  // Si el jugador es Min.
  else {
    score = 10000;
    for (int i = 0; i < remainingMoves; i++) {
      Board currentBoard = myBoard;
      Movement* currentMove = &availableMoves[i];
      short childScore;

      if (currentMove->playAndAssignOwner(currentBoard, PLAYER2) == PLAYER2) {
        // El siguiente turno corresponde al jugador min.
        childScore = MiniMax(currentBoard, false, myDepth - 1, initialDepth)
                         .performAlfaBeta(false, alfa, beta);
      } else {
        // El siguiente turno corresponde al jugador max.
        childScore = MiniMax(currentBoard, true, myDepth - 1, initialDepth)
                         .performAlfaBeta(false, alfa, beta);
      }

      if (childScore < score) {
        score = childScore;
        // Unicamente guardamos el movimiento cuando estamos en el primer nivel
        // del �rbol.
        if (root) {
          setBestMove(*currentMove);
        }
      }
      // Si score es menor que beta, actualizamos el valor.
      if (score < beta) {
        beta = score;
      }

      if (score <= alfa) {
        break;
      }
    }
    return score;
  }
}

void MiniMax::setBestMove(Movement move) {
  bestMove = move;
}

Movement MiniMax::getBestMove() {
  return bestMove;
}
