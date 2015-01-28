/*
----------------------------------
	~Moosey Chess Engine~
	      move.h
----------------------------------
*/

#ifndef MOVEGEN_H
#define MOVEGEN_H

void generateCompleteMoveLists();		//Calls below function. Puts all of the piece's moves into global moveLists
void generateMoveListFor(int p);		//Generates moveList for p
void generateHozMoves(int p, int& counter);
void generateDiagMoves(int p, int& counter);
void generateKnightMoves(int p, int& counter);
void generateKingMoves(int p, int& counter);
void generatePawnMoves(int p, int& counter);

#endif
