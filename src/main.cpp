/*
----------------------------------
	~Moosey Chess Engine~
	   By Chris Moutsos
	      main.cpp
----------------------------------
*/

/*
 *	Encode moveFrom when I copy pieceMoveLists to global moveLists
 *
 *	Stalemates (check if implemented correctly), draw by material
 * 	
 *	prevOnMoveTo needs to be a vector
 *	
 * 	Castling, en passants, promotions
 */

#include <iostream>
#include "board.h"
#include "input.h"
#include "move.h"
#include "movegen.h"
#include "pieces.h"
#include "legal.h"
#include "display.h"

using namespace std;

vector<int> whiteMoveList, blackMoveList;

int main() {
	int moveFrom, moveTo, ply;
	bool side = WHITE, exit = false;

	emptyBoard();
	initializePieces("0");
	displayBoard();
	while (!exit) {
		userInput(moveFrom, moveTo, side);
		movePiece(moveFrom, moveTo);
		changeTurns(ply, side);
		displayBoard();
		moveInfo(moveFrom, moveTo, side);
		generateCompleteMoveLists();
		exit = checkCheck(side);
		exit = checkDraw();

		for (int i = wqR; i <= wPh; i++) {
			cout << piece[i].name << " moveList: ";
			for (int j = 0; j < piece[i].moveListSize; j++) {
				cout << intToSquare(piece[i].moveList[j]);
				if (j < piece[i].moveListSize-1) {
					if (piece[i].moveList[j+1] != 0) {
						cout << ", ";
					}
				}
			}
			cout << ", ";
		}
		cout << endl;
	}

	return 0;
}
