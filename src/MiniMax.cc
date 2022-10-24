#include "MiniMax.hh"

MiniMax::MiniMax(Board board, bool isMaxPlayer, int depth) : myBoard(board), 
            maxPlayer(isMaxPlayer), myDepth(depth), bestMove(-1, -1, EMPTY) {
        availableMoves = myBoard.getAvailableMoves();
        myBoard.scoreUpdater();
    }

MiniMax::~MiniMax() {
}


short MiniMax::performMiniMax(bool root) {
    // Considerar al jugador 1 como max. 
        short remainingMoves = availableMoves.size(); 

    // Calculamos la diferencia de puntajes para saber quien va ganando el juego. 
        short deltaScore = myBoard.getScoreP1() - myBoard.getScoreP2(); 


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
       if (myDepth == 0 || remainingMoves == 0){
           //cout << deltaScore << endl;
        return deltaScore;
       }

        short score; 
        // Si el jugador es max
        if(maxPlayer){
            score = -1000; 
            for (int i = 0; i < remainingMoves; i++)
            {
                //Creamos un nuevo tablero para no modificar el "original". 
                Board currentBoard = myBoard; 
                Movement* currentMove = &availableMoves[i];
                short childScore = -1000;
                //Si al jugar, se completó una celda entonces el jugador 1 tiene un turno extra.
                OwnerType result = currentMove->playAndAssignOwner(currentBoard, PLAYER1);
                if (result == PLAYER1) {
                    MiniMax childMiniMax = { currentBoard, true, myDepth};
                    childScore = childMiniMax.performMiniMax(false);
                }
                else { 
                    //El siguiente turno corresponde al jugador min. 
                    MiniMax childMiniMax = { currentBoard, false, myDepth - 1 };
                    childScore = childMiniMax.performMiniMax(false);
                }

                if (childScore > score){
                    score = childScore;

                    // Unicamente guardamos el movimiento cuando estamos en el primer nivel del árbol. 
                    if (root) { setBestMove(*currentMove); }
                }  
                
            } 
        // Si el jugador es mini 
        } else {
            score = 1000; 
            for (int i = 0; i < remainingMoves; i++)
            {
                Board currentBoard = myBoard; 
                Movement* currentMove = &availableMoves[i]; 

                short childScore = 1000;
                //Si al jugar, se completó una celda entonces el jugador 2 tiene un turno extra. 
                OwnerType result = currentMove->playAndAssignOwner(currentBoard, PLAYER2);
                if ( result == PLAYER2) { 
                    //El siguiente turno corresponde al jugador min. 
                    MiniMax childMiniMax = { currentBoard, false, myDepth};
                    childScore = childMiniMax.performMiniMax(false);
                }
                else {
                    //El siguiente turno corresponde al jugador max. 
                    MiniMax childMiniMax = { currentBoard, true, myDepth - 1 };
                    childScore = childMiniMax.performMiniMax(false);
                }      
                
                if (childScore < score){
                    score = childScore;
                    if (root) { 
                        setBestMove(*currentMove);                     
                    
                    }
                } 
            } 
        }
       // cout << "Score: " << score << endl;
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
        for (int i = 0; i < remainingMoves; i++)
        {
            //Creamos un nuevo tablero para no modificar el "original". 
            Board currentBoard = myBoard;
            Movement* currentMove = &availableMoves[i];
            short childProbability;
            //Si al jugar, se completó una celda entonces el jugador 1 tiene un turno extra. 
            if (currentMove->playAndAssignOwner(currentBoard, PLAYER1) == PLAYER1) {
                //El siguiente turno corresponde al jugador max. 
                MiniMax childMiniMax = { currentBoard, true, myDepth - 1 };
                childProbability = childMiniMax.performProbabilityMiniMax(false);
            }
            else {
                //El siguiente turno corresponde al jugador min. 
                MiniMax childMiniMax = { currentBoard, false, myDepth - 1 };
                childProbability = childMiniMax.performProbabilityMiniMax(false);
            }
            //Si estamos en el primer nivel, entonces seleccionamos como mejor movimiento al que tiene la probabilidad más alta. 
            if (root) {
                if (childProbability > probability) {
                    probability = childProbability; 
                    setBestMove(*currentMove);
                }
            }
            else {
                if (childProbability > 0) {
                    // Solo consideramos la probabilidad de ganar de max. 
                    probability += childProbability;
                }
            }
        }
     // Con esto devolvemos la probabilidad de de ganar si se selecciona ese "nodo". 
     return probability / remainingMoves;
    }
    // Si el jugador es mini 
    else {
        probability = 1;
        for (int i = 0; i < remainingMoves; i++)
        {
            Board currentBoard = myBoard;
            Movement* currentMove = &availableMoves[i];

            short childProbability;
            //Si al jugar, se completó una celda entonces el jugador 2 tiene un turno extra. 
            if (currentMove->playAndAssignOwner(currentBoard, PLAYER2) == PLAYER2) {// TODO: debo agregar este método a clase Movement 
                //El siguiente turno corresponde al jugador min. 
                MiniMax childMiniMax = { currentBoard, false, myDepth - 1 };
                childProbability = childMiniMax.performProbabilityMiniMax(false);
            }
            else {
                //El siguiente turno corresponde al jugador max. 
                MiniMax childMiniMax = { currentBoard, true, myDepth - 1 };
                childProbability = childMiniMax.performProbabilityMiniMax(false);
            }

            //Si estamos en el primer nivel, entonces seleccionamos como mejor movimiento al que tiene la probabilidad más baja. 
            if (root) {
                if (childProbability < probability) {
                    probability = childProbability;
                    setBestMove(*currentMove);
                }
            }
            else {
                if (childProbability > 0) {
                    // Solo consideramos la probabilidad de ganar de max. 
                    probability += childProbability;
                }
            }

        }
        //Restar el 1 inicial
        return (probability-1) / remainingMoves;
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
        for (int i = 0; i < remainingMoves; i++)
        {
            Board currentBoard = myBoard;
            Movement* currentMove = &availableMoves[i];
            short childScore;

            if (currentMove->playAndAssignOwner(currentBoard, PLAYER1) == PLAYER1) {
                //El siguiente turno corresponde al jugador max. 
                childScore = MiniMax(currentBoard, true, myDepth - 1).performAlfaBeta(false, alfa, beta);
            }
            else {
                //El siguiente turno corresponde al jugador min. 
                childScore = MiniMax(currentBoard, false, myDepth - 1).performAlfaBeta(false, alfa, beta);
            }

            if (childScore > score) {
                score = childScore;
                // Unicamente guardamos el movimiento cuando estamos en el primer nivel del árbol. 
                if (root) { setBestMove(*currentMove); }
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
        for (int i = 0; i < remainingMoves; i++)
        {
            Board currentBoard = myBoard;
            Movement* currentMove = &availableMoves[i];
            short childScore;

            if (currentMove->playAndAssignOwner(currentBoard, PLAYER2) == PLAYER2) {
                //El siguiente turno corresponde al jugador min. 
                childScore = MiniMax(currentBoard, false, myDepth - 1).performAlfaBeta(false, alfa, beta);
            }
            else {
                //El siguiente turno corresponde al jugador max. 
                childScore = MiniMax(currentBoard, true, myDepth - 1).performAlfaBeta(false, alfa, beta);
            }

            if (childScore < score) {
                score = childScore;
                // Unicamente guardamos el movimiento cuando estamos en el primer nivel del árbol. 
                if (root) { setBestMove(*currentMove); }
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

void MiniMax::setBestMove(Movement move){
    bestMove = move; 
} 


Movement MiniMax::getBestMove(){
    return bestMove; 
}
