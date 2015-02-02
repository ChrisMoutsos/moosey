/*
----------------------------------
	~Moosey Chess Engine~
	      move.cpp
----------------------------------
*/

#include <iostream>
#include "board.h"

void Board::movePiece() {
	movePiece(moveFrom, moveTo);
}

void Board::movePiece(int mF, int mT) {
	prevOnMoveTo = board120[mT];
	pieceMovedFrom = mF;
	pieceMoved = board120[mF];
	if (board120[mT] != empty) {
		piece[board120[mT]].alive = false;
		piece[board120[mT]].pos = null;
	}

	board120[mT] = board120[mF];
	board120[mF] = empty;
	piece[board120[mT]].pos = mT;

	piece[board120[mT]].moved++;
	ply++;
}

void Board::unmovePiece() {
	unmovePiece(moveFrom, moveTo);
}

void Board::unmovePiece(int mF, int mT) {
	board120[mF] = pieceMoved;
	piece[board120[mF]].moved--;
	piece[board120[mF]].pos = mF;

	board120[mT] = prevOnMoveTo;
	if (board120[mT] != empty) {
		piece[board120[mT]].pos = mT;
		piece[board120[mT]].alive = true;
	}
	ply--;
}

void Board::moveInfo() const {
	using namespace std;
	
	!side ? cout << "White" : cout << "Black";
	cout << " moved " << piece[pieceMoved].name;
	cout << " from " << intToSquare(moveFrom);
	cout  << " to " << intToSquare(moveTo);
	if (prevOnMoveTo != empty) {
		cout << " and captured a ";
		cout << piece[prevOnMoveTo].name;
	}
	cout << "\n\n";
}	

void Board::changeTurn() {
	side = side ? 0 : 1;
}
