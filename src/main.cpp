/*
----------------------------------
	~Moosey Chess Engine~
	   By Chris Moutsos
	      main.cpp
----------------------------------
*/
/*
 *	Write inCheck function
 *	Encode moveFrom when I copy pieceMoveLists to global moveLists
 *	Stalemates (check if implemented correctly), draw by material
 * 	Castling, en passants, promotions
 *	Vectors for prevOnMoveTo, pieceMoved, pieceMovedFrom
 *	SDL
 */

#include <iostream>
#include "board.h"
#include "display.h"
#include "input.h"

using namespace std;

int main() {
	bool exit = false;
	int mF = 0, mT = 0;
	Board board;
	displayBoard(board);
	while (!exit) {
		userInput(board, mF, mT);
		board.setMove(mF, mT);
		board.movePiece();
		board.changeTurn();
		displayBoard(board);
		board.moveInfo();
		board.generateMoveLists();
		showMoveLists(board);

		exit = (board.checkCheck() | board.checkDraw());
	}

	return 0;
}
