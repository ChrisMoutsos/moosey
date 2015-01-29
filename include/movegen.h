/*
----------------------------------
	~Moosey Chess Engine~
	      move.h
----------------------------------
*/

#ifndef MOVEGEN_H
#define MOVEGEN_H

#include <vector>

void generateCompleteMoveLists(std::vector<int>& whiteMoveList, std::vector<int>& blackMoveList);
void generateMoveListFor(int p);	
void generateHozMoves(int p, int& counter);
void generateDiagMoves(int p, int& counter);
void generateKnightMoves(int p, int& counter);
void generateKingMoves(int p, int& counter);
void generatePawnMoves(int p, int& counter);

#endif
