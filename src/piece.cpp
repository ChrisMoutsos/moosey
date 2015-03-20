/*
----------------------------------
	~Moosey Chess Engine~
	      piece.cpp
----------------------------------
*/

#include "common.h"
#include "piece.h"

Piece::Piece() {
	moved = 0;
	alive = true;
	promoted = false;
	moveList = 0;
	moveListSize = 0;
}

Piece::~Piece() {
	delete [] moveList;
}

//ACCESSORS
int Piece::getFromMoveList(int i) const {
	assert (i > -1 && i < moveListSize);
	return moveList[i];
}

//MUTATORS
void Piece::setInMoveList(int i, int v) {
	assert (i > -1 && i < moveListSize);
	moveList[i] = v;
}
