#include "Player.hh"
#include <gtest/gtest.h>

TEST(CellTest, BoxCheckTest) {
  Cell c1;
  EXPECT_FALSE(c1.boxChecker(PLAYER1));

  *(c1.getLine(0)) = PLAYER1;
  *(c1.getLine(1)) = PLAYER2;
  *(c1.getLine(2)) = PLAYER1;
  *(c1.getLine(3)) = PLAYER1;
  EXPECT_TRUE(c1.boxChecker(PLAYER1));
  EXPECT_EQ(c1.getBoxOwner(), PLAYER1);
  EXPECT_EQ(c1.availableMovesCount(), 0);
}

TEST(CellTest, LineOwnerTest) {
  Cell c1;
  *(c1.getLine(0)) = PLAYER1;
  EXPECT_EQ(*(c1.getLine(0)), PLAYER1);
  *(c1.getLine(1)) = PLAYER2;
  EXPECT_EQ(*(c1.getLine(1)), PLAYER2);
  *(c1.getLine(2)) = PLAYER1;
  EXPECT_EQ(*(c1.getLine(2)), PLAYER1);
  EXPECT_EQ(*(c1.getLine(3)), NO_OWNER);
  EXPECT_EQ(c1.availableMovesCount(), 1);
  Cell c2;
  EXPECT_EQ(c2.availableMovesCount(), 4);
  *(c2.getLine(0)) = PLAYER1;
  *(c2.getLine(3)) = PLAYER2;
  EXPECT_EQ(c2.availableMovesCount(), 2);
}

TEST(BoardTest, BoxesSizeTest) {
  Board b1(13, 6);
  EXPECT_EQ(b1.getBoardRowSize(), 12);
  EXPECT_EQ(b1.getBoardColSize(), 5);
  EXPECT_EQ(b1.getAvailableMoves().size(), 137);
  for (int i = 0; i < b1.getBoardRowSize(); i++) {
    for (int j = 0; j < b1.getBoardColSize(); j++) {
      *(b1.getCell(i, j)->getLine(0)) = PLAYER1;
      *(b1.getCell(i, j)->getLine(2)) = PLAYER1;
    }
  }
  EXPECT_EQ(b1.getAvailableMoves().size(), 120);
}

TEST(BoardTest, ScoreTest) {
  Board b1(15, 10);
  EXPECT_EQ(b1.getScoreP1(), 0);
  EXPECT_EQ(b1.getScoreP2(), 0);
  for (int i = 0; i < b1.getBoardRowSize(); i++) {
    for (int j = 0; j < b1.getBoardColSize() - 1; j++) {
      *(b1.getCell(i, j)->getLine(0)) = PLAYER1;
      *(b1.getCell(i, j)->getLine(1)) = PLAYER1;
      *(b1.getCell(i, j)->getLine(2)) = PLAYER1;
      *(b1.getCell(i, j)->getLine(3)) = PLAYER1;
      b1.getCell(i, j)->boxChecker(PLAYER1);
    }
  }
  for (int i = 0; i < b1.getBoardRowSize(); i++) {
    *(b1.getCell(i, b1.getBoardColSize() - 1)->getLine(0)) = PLAYER2;
    *(b1.getCell(i, b1.getBoardColSize() - 1)->getLine(1)) = PLAYER2;
    *(b1.getCell(i, b1.getBoardColSize() - 1)->getLine(2)) = PLAYER2;
    *(b1.getCell(i, b1.getBoardColSize() - 1)->getLine(3)) = PLAYER2;
    b1.getCell(i, b1.getBoardColSize() - 1)->boxChecker(PLAYER2);
  }
  b1.scoreUpdater();
  EXPECT_EQ(b1.getScoreP1(), 14 * 8);
  EXPECT_EQ(b1.getScoreP2(), 14);
}

TEST(BoardTest, SEGFPrevisionTest) {
  Board b1(14, 14);
  EXPECT_EQ(b1.getCell(500, 6), nullptr);
  EXPECT_EQ(b1.getCell(0, 789899), nullptr);
  EXPECT_EQ(b1.getCell(666, 777), nullptr);
  EXPECT_EQ(b1.getCell(14, 14), nullptr);
  EXPECT_NE(b1.getCell(0, 0), nullptr);
}

void modifyBoardTestMethod(Board board) {
  *(board.getCell(0, 0)->getLine(0)) = PLAYER1;
  *(board.getCell(0, 0)->getLine(1)) = PLAYER1;
  *(board.getCell(0, 0)->getLine(2)) = PLAYER1;
  *(board.getCell(0, 0)->getLine(3)) = PLAYER1;
  board.getCell(0, 0)->boxChecker(PLAYER1);
}

TEST(BoardTest, CopyTest) {
  Board b1(20, 5);
  modifyBoardTestMethod(b1);
  b1.scoreUpdater();
  EXPECT_EQ(b1.getScoreP1(), 0);
  *(b1.getCell(0, 1)->getLine(0)) = PLAYER2;
  Board b1copy = b1;
  EXPECT_EQ(*(b1copy.getCell(0, 1)->getLine(0)), PLAYER2);
  EXPECT_EQ(b1copy.getScoreP1(), 0);
}

bool duplicadeMovesTestMethod(vector<Movement> &moves) {
  for (int i = 0; i < moves.size(); i++) {
    for (int j = 0; j < moves.size(); j++) {
      // No esta el mismo move.
      if (i != j && moves[i].getXPos() == moves[j].getXPos() &&
          moves[i].getYPos() == moves[j].getYPos() &&
          moves[i].getLineDirection() == moves[j].getLineDirection()) {
        return true;
      }
      // Solo hay una forma de hacer el move.
      if (i != j && moves[i].getXPos() == moves[j].getXPos() &&
          moves[j].getYPos() == moves[i].getYPos() - 1 &&
          moves[j].getLineDirection() == EAST &&
          moves[i].getLineDirection() == WEST) {
        return true;
      } else if (i != j && moves[i].getYPos() == moves[j].getYPos() &&
                 moves[j].getXPos() == moves[i].getXPos() - 1 &&
                 moves[j].getLineDirection() == SOUTH &&
                 moves[i].getLineDirection() == NORTH) {
        return true;
      }
    }
  }
  return false;
}

TEST(BoardTest, AvailableMovesTest) {
  Board b1(12, 8);
  vector<Movement> moves = b1.getAvailableMoves();
  EXPECT_FALSE(duplicadeMovesTestMethod(moves));

  Board b2(15, 15);
  for (int i = 0; i < 3; i++) {
    *(b2.getCell(i, i)->getLine(0)) = PLAYER2;
    *(b2.getCell(i, i)->getLine(1)) = PLAYER2;
    *(b2.getCell(i, i)->getLine(2)) = PLAYER2;
    *(b2.getCell(i, i)->getLine(3)) = PLAYER2;
    b2.getCell(i, i)->boxChecker(PLAYER2);
  }
  b2.scoreUpdater();
  EXPECT_EQ(b2.getScoreP2(), 3);
  moves = b2.getAvailableMoves();
  EXPECT_EQ(moves.size(), 412);
  EXPECT_FALSE(duplicadeMovesTestMethod(moves));
}

TEST(MovementTest, OnBoardPlayTest) {
  Board b1(5, 5);
  Movement m1(45, 78, EAST);
  Movement m2(0, 1, NORTH);
  Movement m3(1, 1, NORTH);
  Movement m4(0, 2, WEST);
  Movement m5(0, 1, EAST);
  EXPECT_FALSE(m1.play(b1, PLAYER1));
  EXPECT_TRUE(m2.play(b1, PLAYER1));
  EXPECT_TRUE(m3.play(b1, PLAYER1));
  EXPECT_TRUE(m4.play(b1, PLAYER1));
  EXPECT_FALSE(m5.play(b1, PLAYER1));
  EXPECT_EQ(*(b1.getCell(0, 2)->getLine(0)), PLAYER1);
  EXPECT_EQ(*(b1.getCell(0, 1)->getLine(1)), PLAYER1);
}

TEST(MovementTest, playAndAssignOwner) {
	Board board = { 3,3 };
	Movement moveN = { 1,0, NORTH };
	EXPECT_EQ(moveN.playAndAssignOwner(board, PLAYER1), NO_OWNER);
	Movement moveW = { 1,0, WEST };
	EXPECT_EQ(moveW.playAndAssignOwner(board, PLAYER2), NO_OWNER);
	Movement moveS = { 1,0, SOUTH };
	EXPECT_EQ(moveS.playAndAssignOwner(board, PLAYER1), NO_OWNER);
	Movement moveN2 = { 1,1, NORTH };
	moveN2.playAndAssignOwner(board, PLAYER2);
	Movement moveS2 = { 1,1, SOUTH };
	moveS2.playAndAssignOwner(board, PLAYER1);
	Movement moveE = { 1,1, EAST };
	EXPECT_EQ(moveE.playAndAssignOwner(board, PLAYER2), NO_OWNER);
	Movement moveE2 = { 1, 0, EAST };


	EXPECT_EQ(moveE2.playAndAssignOwner(board, PLAYER2), PLAYER2);
	EXPECT_EQ(moveE2.playAndAssignOwner(board, PLAYER2), NO_VALID);
	EXPECT_EQ(board.getCell(1, 0)->getBoxOwner(), PLAYER2);
	EXPECT_EQ(board.getCell(1, 1)->getBoxOwner(), PLAYER2);


}


// Para miniMax voy a usar una matriz de 2x2 celdas. 
TEST(MiniMaxTest, FirstTwoMovements ) {
	//Matriz 2x2 celdas 
	Board board = { 3,3 };
	MiniMax miniMax = { board, true, 4 };
	miniMax.performMiniMax(true);
	Movement bestMove = miniMax.getBestMove();
	bestMove.play(board, PLAYER1);

	// Primer movimiento 
	EXPECT_TRUE(bestMove.getXPos() == 0 && bestMove.getYPos() == 0 && bestMove.getLineDirection() == EAST); //TODO: puede cambiar con nuevo m�todo getMovements(). 


	MiniMax secondMiniMax = { board, false, 4 };
	secondMiniMax.performMiniMax(true);
	bestMove = secondMiniMax.getBestMove();

	// Segundo Movimiento
	EXPECT_TRUE(bestMove.getXPos() == 0 && bestMove.getYPos() == 0 && bestMove.getLineDirection() == WEST);
}

// B�sicamente que no regale una caja al completar una tercera l�nea. 
TEST(MiniMaxTest, NotCompleteThirdLine) {
	Board board = { 3,3 };
	Movement moveN = { 0,0, NORTH }; 
	moveN.playAndAssignOwner(board, PLAYER1);
	Movement moveS = { 0,0, SOUTH };
	moveS.playAndAssignOwner(board, PLAYER2);

	MiniMax miniMax = { board, false, 5 };
	miniMax.performMiniMax(true);
	Movement bestMove = miniMax.getBestMove(); 

	EXPECT_FALSE(bestMove.getXPos() == 0 && bestMove.getYPos() == 0 && bestMove.getLineDirection() == WEST);
	EXPECT_FALSE(bestMove.getXPos() == 0 && bestMove.getYPos() == 0 && bestMove.getLineDirection() == EAST);
	EXPECT_FALSE(bestMove.getXPos() == 0 && bestMove.getYPos() == 1 && bestMove.getLineDirection() == WEST);
}

// Que complete una celda 
TEST(MiniMaxTest, CompleteCell) {
	Board board = { 3,3 };
	Movement moveN = { 0,0, NORTH };
	moveN.play(board, PLAYER1);
	Movement moveS = { 0,0, SOUTH };
	moveS.play(board, PLAYER2);
	Movement moveE = { 0,0, EAST };
	moveE.play(board, PLAYER1);

	MiniMax miniMax = { board, false, 4 };
	miniMax.performMiniMax(true);
	Movement bestMove = miniMax.getBestMove();

	EXPECT_TRUE(bestMove.getXPos() == 0 && bestMove.getYPos() == 0 && bestMove.getLineDirection() == WEST);//TODO: puede cambiar con nuevo m�todo getMovements(). 
}

TEST(MiniMaxTest, CompleteTwoCell) {
	Board board = { 3,3 };
	Movement moveN = { 1,0, NORTH };
	moveN.play(board, PLAYER1);
	Movement moveW = { 1,0, WEST };
	moveW.play(board, PLAYER2);
	Movement moveS = { 1,0, SOUTH };
	moveS.play(board, PLAYER1);
	Movement moveN2 = { 1,1, NORTH };
	moveN2.play(board, PLAYER2);
	Movement moveS2 = { 1,1, SOUTH };
	moveS2.play(board, PLAYER1);
	Movement moveE2 = { 1,1, EAST };
	moveE2.play(board, PLAYER2);

	MiniMax miniMax = { board, true, 4 };
	miniMax.performMiniMax(true);
	Movement bestMove = miniMax.getBestMove();

	EXPECT_TRUE(bestMove.getXPos() == 1 && bestMove.getYPos() == 0 && bestMove.getLineDirection() == EAST); 
}

// Alpha-beta prunning
TEST(AlfaBetaPruning, FirstTwoMovements) {
	//Matriz 2x2 celdas 
	Board board = { 3,3 };
	MiniMax miniMax = { board, true, 4 };
	miniMax.performAlfaBeta(true, -15000, 15000);
	Movement bestMove = miniMax.getBestMove();
	bestMove.play(board, PLAYER1);

	// Primer movimiento 
	EXPECT_TRUE(bestMove.getXPos() == 0 && bestMove.getYPos() == 0 && bestMove.getLineDirection() == EAST); 


	MiniMax secondMiniMax = { board, false, 4 };
	secondMiniMax.performAlfaBeta(true, -15000, 15000);
	bestMove = secondMiniMax.getBestMove();

	// Segundo Movimiento
	EXPECT_TRUE(bestMove.getXPos() == 0 && bestMove.getYPos() == 0 && bestMove.getLineDirection() == WEST);
}

// B�sicamente que no regale una caja al completar una tercera l�nea. 
TEST(AlfaBetaPruning, NotCompleteThirdLine) {
	Board board = { 3,3 };
	Movement moveN = { 0,0, NORTH };
	moveN.playAndAssignOwner(board, PLAYER1);
	Movement moveS = { 0,0, SOUTH };
	moveS.playAndAssignOwner(board, PLAYER2);

	MiniMax miniMax = { board, false, 5 };
	miniMax.performAlfaBeta(true, -15000, 15000);
	Movement bestMove = miniMax.getBestMove();

	EXPECT_FALSE(bestMove.getXPos() == 0 && bestMove.getYPos() == 0 && bestMove.getLineDirection() == WEST);
	EXPECT_FALSE(bestMove.getXPos() == 0 && bestMove.getYPos() == 0 && bestMove.getLineDirection() == EAST);
	EXPECT_FALSE(bestMove.getXPos() == 0 && bestMove.getYPos() == 1 && bestMove.getLineDirection() == WEST);
}

// Que complete una celda 
TEST(AlfaBetaPruning, CompleteCell) {
	Board board = { 3,3 };
	Movement moveN = { 0,0, NORTH };
	moveN.play(board, PLAYER1);
	Movement moveS = { 0,0, SOUTH };
	moveS.play(board, PLAYER2);
	Movement moveE = { 0,0, EAST };
	moveE.play(board, PLAYER1);

	MiniMax miniMax = { board, false, 4 };
	miniMax.performAlfaBeta(true, -15000, 15000);
	Movement bestMove = miniMax.getBestMove();

	EXPECT_TRUE(bestMove.getXPos() == 0 && bestMove.getYPos() == 0 && bestMove.getLineDirection() == WEST);
}

TEST(AlfaBetaPruning, CompleteTwoCell) {
	Board board = { 3,3 };
	Movement moveN = { 1,0, NORTH };
	moveN.play(board, PLAYER1);
	Movement moveW = { 1,0, WEST };
	moveW.play(board, PLAYER2);
	Movement moveS = { 1,0, SOUTH };
	moveS.play(board, PLAYER1);
	Movement moveN2 = { 1,1, NORTH };
	moveN2.play(board, PLAYER2);
	Movement moveS2 = { 1,1, SOUTH };
	moveS2.play(board, PLAYER1);
	Movement moveE2 = { 1,1, EAST };
	moveE2.play(board, PLAYER2);

	MiniMax miniMax = { board, true, 4 };
	miniMax.performAlfaBeta(true, -15000, 15000);
	Movement bestMove = miniMax.getBestMove();

	EXPECT_TRUE(bestMove.getXPos() == 1 && bestMove.getYPos() == 0 && bestMove.getLineDirection() == EAST);
}
