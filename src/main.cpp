/*
----------------------------------
	~Moosey Chess Engine~
	   By Chris Moutsos
	      main.cpp
----------------------------------
*/

/*
 *	Write inCheck function
 * 
 *	Encode moveFrom when I copy pieceMoveLists to global moveLists
 *
 *	Stalemates (check if implemented correctly), draw by material

 *	prevOnMoveTo needs to be a vector
 *	
 * 	Castling, en passants, promotions
 */

#include <iostream>
#include "board.h"

using namespace std;

int main() {
	bool exit = false;

	Board board;
	board.displayBoard();

	while (!exit) {
		board.userInput();
		board.movePiece(board.getMoveFrom(), board.getMoveTo());
		board.changeTurn();
		board.displayBoard();
		board.moveInfo();
		board.generateMoveLists();
/*
		for (int i = wqR; i <= wPh; i++) {
			cout << board.piece[i].name << " moveList: ";
			for (int j = 0; j < board.piece[i].moveListSize; j++) {
				cout << intToSquare(board.piece[i].moveList[j]);
				if (j < board.piece[i].moveListSize-1) {
					if (board.piece[i].moveList[j+1] != 0) {
						cout << ", ";
					}
				}
			}
			cout << ", ";
		}
		cout << endl;
*/
		exit = (board.checkCheck() | board.checkDraw());
	}

	return 0;
}
