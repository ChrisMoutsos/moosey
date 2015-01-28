/*
----------------------------------
	~Moosey Chess Engine~
	      legal.h
----------------------------------
*/

#ifndef LEGAL_H
#define LEGAL_H

bool legalMove(int mF, int mT, bool side); //Verifies move is valid and legal

bool checkDraw();		   //If draw, outputs correct message and ends game
bool checkCheck(bool side); //Calls inCheck and inCheckmate, outputs correct message and ends game if appropriate
bool inCheck(bool side);
bool inCheckmate(bool side);

bool validateMove(int moveFrom, int moveTo);	//Uses the below function to validate moves
bool validatePawnMove(int diff120, int moveFrom, int moveTo);
bool validateHozMove(int small, int big, int diff120, int moveFrom, int moveTo);
bool validateDiagMove(int small, int big, int diff120, int moveFrom, int moveTo);
bool validateKnightMove(int diff120, int moveFrom, int moveTo);
bool validateKingMove(int diff120, int moveFrom, int moveTo);

#endif
