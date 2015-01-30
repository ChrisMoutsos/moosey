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
 *	Vectors for prevOnMoveTo, pieceMoved, pieceMovedFrom
 * 	Castling, en passants, promotions
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
		do {
			board.getSide() ? cout << "White" : cout << "Black";
			cout << " to move:\n\t";
		} while (!getInput(mF, mT) || !board.legalMove(mF, mT, 1));

		if (board.inCheck()) {
			board.getSide() ? cout << "White" : cout << "Black";
			cout << " is in check!";
		}

		board.setMove(mF, mT);
		board.movePiece();
		board.changeTurn();
		displayBoard(board);
		board.moveInfo();
		board.generateMoveLists();
	
		exit = (board.checkCheck() | board.checkDraw());
	}

	return 0;
}
