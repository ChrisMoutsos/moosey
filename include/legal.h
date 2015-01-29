/*
----------------------------------
	~Moosey Chess Engine~
	      legal.h
----------------------------------
*/

#ifndef LEGAL_H
#define LEGAL_H

#include <vector>

bool legalMove(int mF, int mT, bool side); //Verifies move is valid and legal

bool checkDraw(std::vector<int> whiteMoveList, std::vector<int> blackMoveList);		   //If draw, outputs correct message and ends game
bool checkCheck(std::vector<int> whiteMoveList, std::vector<int> blackMoveList, bool side); 
bool inCheck(bool side);
bool inCheckmate(std::vector<int> whiteMoveList, std::vector<int> blackMoveList, bool side);

bool validateMove(int moveFrom, int moveTo);	//Uses the below function to validate moves
bool validatePawnMove(int diff120, int moveFrom, int moveTo);
bool validateHozMove(int small, int big, int moveFrom, int moveTo);
bool validateDiagMove(int small, int big, int diff120);
bool validateKnightMove(int diff120, int moveFrom, int moveTo);
bool validateKingMove(int diff120);

#endif
