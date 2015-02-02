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

void showMoveLists(Board board);

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

		if (board.inCheckmate(board.getSide())) 
			exit = true;
	}

	return 0;
}

void showMoveLists(Board board) {
	int mF, mT;

	cout << "White movelist\n";
	for (int i = 0; i < (int)board.whiteMoveList.size(); i++) {
		cout << board.whiteMoveList[i] << " ";
	}
	cout << "\nWhite piece moves\n";
	for (int i = wqR; i <= wPh; i++) {
		cout << board.piece[i].name << ": ";
		for (int j = 0; j < board.piece[i].moveListSize; j++) {
			if (board.piece[i].moveList[j] != 0) {
				mT = board.piece[i].moveList[j]%100;
				mF = (board.piece[i].moveList[j] - mT) / 100;
				cout << intToSquare(mF) << " to " << intToSquare(mT) << ", ";
			}
		}
		cout <<"... ";
	}
	cout << endl;
	cout << "\nBlack movelist\n";
	for (int i = 0; i < (int)board.blackMoveList.size(); i++) {
		cout << board.blackMoveList[i] << " ";
	}
	cout << "\nBlack piece moves\n";
	for (int i = bqR; i <= bPh; i++) {
		cout << board.piece[i].name << ": ";
		for (int j = 0; j < board.piece[i].moveListSize; j++) {
			if (board.piece[i].moveList[j] != 0) {
				mT = board.piece[i].moveList[j]%100;
				mF = (board.piece[i].moveList[j] - mT) / 100;
				cout << intToSquare(mF) << " to " << intToSquare(mT) << ", ";
			}
		}
		cout << "... ";
	}
	cout << endl;	
}
