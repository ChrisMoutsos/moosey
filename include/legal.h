/*
----------------------------------
	~Moosey Chess Engine~
	      legal.h
----------------------------------
*/

#ifndef LEGAL_H
#define LEGAL_H

#include <vector>

bool legalMove(int, int, bool); //Verifies move is valid and legal

bool checkDraw(std::vector<int>, std::vector<int>);		   //If draw, outputs correct message and ends game
bool checkCheck(std::vector<int>, std::vector<int>, bool); 
bool inCheck(bool);
bool inCheckmate(std::vector<int>, std::vector<int>, bool);

bool validateMove(int, int);	//Uses the below function to validate moves
bool validatePawnMove(int, int, int);
bool validateHozMove(int, int, int, int);
bool validateDiagMove(int, int, int);
bool validateKnightMove(int, int, int);
bool validateKingMove(int);

#endif
