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
#include "macros.h"
#include "board.h"
#include "input.h"
#include "move.h"
#include "pieces.h"
#include "legal.h"

using namespace std;

bool side = WHITE, //BLACK = 0, WHITE = 1 
     exit = false;
int ply = 0; //Half-moves
vector<int> whiteMoveList, blackMoveList;

int main() {
	emptyBoard();
	initializePieces("0");
	displayBoard();
	while (!exit) {
		userInput();
		movePiece();
		changeTurns();
		displayBoard();
		moveInfo();
		generateCompleteMoveLists();
		checkCheck();
		checkDraw();

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
		cout << endl << "White movelist: ";
		for (int i = 0; i < (int)whiteMoveList.size(); i++) {
			cout << intToSquare(whiteMoveList[i]) << ", ";
		}
		cout << endl;
	}

	return 0;
}
