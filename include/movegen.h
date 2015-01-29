/*
----------------------------------
	~Moosey Chess Engine~
	      move.h
----------------------------------
*/

#ifndef MOVEGEN_H
#define MOVEGEN_H

#include <vector>

void generateCompleteMoveLists(std::vector<int>&, std::vector<int>&);
void generateMoveListFor(int);	
void generateHozMoves(int, int&);
void generateDiagMoves(int, int&);
void generateKnightMoves(int, int&);
void generateKingMoves(int, int&);
void generatePawnMoves(int, int&);

#endif
