/*
----------------------------------
	~Moosey Chess Engine~
	      move.cpp
----------------------------------
*/

#include "board.h"
#include "pieces.h"
#include "legal.h"
#include "move.h"

using namespace std;

int pieceMoved = -1, pieceMovedFrom = 0, prevOnMoveTo = -1;

void movePiece(int mF, int mT) { //Moves moveFrom to moveTo by default
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
}

void unmovePiece(int mF, int mT) {
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
}

void moveInfo(int moveFrom, int moveTo, bool side) {
	!side ? cout << "White" : cout << "Black";
	cout << " moved " << piece[pieceMoved].name << " from " << intToSquare(moveFrom) << " to " << intToSquare(moveTo);
	prevOnMoveTo != -1 ? cout << " and captured a " << piece[prevOnMoveTo].name << endl << endl : cout << endl << endl;
}	

void changeTurns(int& ply, bool& side) {
	side = side ? 0 : 1;
	ply++;
}
