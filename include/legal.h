#ifndef LEGAL_H
#define LEGAL_H

bool legalMove(int mF = moveFrom, int mT = moveTo, bool side = side); //Verifies move is valid and legal

void checkDraw();		   //If draw, outputs correct message and ends game
void checkCheck(bool side = side); //Calls inCheck and inCheckmate, outputs correct message and ends game if appropriate
bool inCheck(bool side = side);
bool inCheckmate(bool side = side);

bool validateMove(int moveFrom = moveFrom, int moveTo = moveTo);	//Uses the below function to validate moves
bool validatePawnMove(int diff120, int moveFrom, int moveTo);
bool validateHozMove(int small, int big, int diff120, int moveFrom, int moveTo);
bool validateDiagMove(int small, int big, int diff120, int moveFrom, int moveTo);
bool validateKnightMove(int diff120, int moveFrom, int moveTo);
bool validateKingMove(int diff120, int moveFrom, int moveTo);

#endif
