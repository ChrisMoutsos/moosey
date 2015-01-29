/*
----------------------------------
	~Moosey Chess Engine~
	      move.h
----------------------------------
*/

#ifndef MOVE_H
#define MOVE_H

void movePiece(int mF, int mT);	//Moves piece on mF to mT
void unmovePiece(int mF, int mT);	//Unmoves piece on mT to mF
void moveInfo(int moveFrom, int moveTo, bool side);					//Outputs in form of "White moved Pawn from a2 to b3( and captured a Knight)"
void changeTurns(int& ply, bool& side);				

extern int pieceMoved, pieceMovedFrom, prevOnMoveTo;

#endif
