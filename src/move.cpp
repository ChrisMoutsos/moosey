/*
----------------------------------
	~Moosey Chess Engine~
	      move.cpp
----------------------------------
*/

#include <iostream>
#include "board.h"

using namespace std;

void Board::movePiece() {
	movePiece(moveFrom, moveTo);
}

void Board::movePiece(int mF, int mT) {
	prevOnMoveTo = board64[mT];
	pieceMovedFrom = mF;
	pieceMoved = board64[mF];
	if (board64[mT] != empty) {
		piece[board64[mT]].alive = false;
		piece[board64[mT]].pos = null;
	}

	board64[mT] = board64[mF];
	board64[mF] = empty;
	board120[from64((int)mT)] = board64[mF];
	board120[from64((int)mF)] = empty;
	piece[board64[mT]].pos = mT;

	piece[board64[mT]].moved++;
	ply++;
}

void Board::unmovePiece() {
	unmovePiece(moveFrom, moveTo);
}

void Board::unmovePiece(int mF, int mT) {
	board64[mF] = pieceMoved;
	board120[from64(mF)] = pieceMoved;
	piece[board64[mF]].moved--;
	piece[board64[mF]].pos = mF;

	board64[mT] = prevOnMoveTo;
	board120[from64(mT)] = prevOnMoveTo;
	if (board64[mT] != empty) {
		piece[board64[mT]].pos = mT;
		piece[board64[mT]].alive = true;
	}
	ply--;
}

void Board::moveInfo() const {
	!side ? std::cout << "White" : std::cout << "Black";
	std::cout << " moved " << piece[pieceMoved].name << " from " << intToSquare(moveFrom) << " to " << intToSquare(moveTo);
	prevOnMoveTo != -1 ? std::cout << " and captured a " << piece[prevOnMoveTo].name << "\n\n" : cout << "\n\n";
}	

void Board::changeTurn() {
	side = side ? 0 : 1;
}
