/*
----------------------------------
	~Moosey Chess Engine~
	      move.h
----------------------------------
*/

#ifndef MOVE_H
#define MOVE_H

void movePiece(int, int);	//Moves piece on mF to mT
void unmovePiece(int, int);	//Unmoves piece on mT to mF
void moveInfo(int, int, bool);					//Outputs in form of "White moved Pawn from a2 to b3( and captured a Knight)"
void changeTurns(int&, bool&);				

extern int pieceMoved, pieceMovedFrom, prevOnMoveTo;

#endif
