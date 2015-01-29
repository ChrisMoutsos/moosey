/*
----------------------------------
	~Moosey Chess Engine~
	      legal.cpp
----------------------------------
*/

#include <iostream>
#include "cmath"
#include "board.h"
#include "legal.h"
#include "pieces.h"
#include "move.h"

using namespace std;

bool legalMove(int mF, int mT, bool side) { 
	bool isInCheck;
//	cout << "Checking if it's legal to moveFrom " << mF << " to " << mT << " as " << side << endl;

	if (!validateMove(mF, mT)) return false;
	movePiece(mF, mT);
	isInCheck = inCheck(side);
	unmovePiece(mF, mT);
	return isInCheck ? false : true;
}

bool checkDraw(vector<int> whiteMoveList, vector<int> blackMoveList) {
	if ((int)whiteMoveList.size() == 0 && (int)blackMoveList.size() == 0) {
		cout << "Stalemate!" << endl;
		return true;
	}	
	return false;
}

bool checkCheck(vector<int> whiteMoveList, vector<int> blackMoveList, bool side) {
	if (inCheck(side)) {
		if (inCheckmate(whiteMoveList, blackMoveList, side)) { 
			side ? cout << "White is in checkmate. Black wins!\n" : cout << "Black is in checkmate. White wins!\n";
			return true;
		}
		else side ? cout << "White is in check!\n" : "Black is in check!\n";
	}
	return false;
}

bool inCheckmate(vector<int> whiteMoveList, vector<int> blackMoveList, bool side) { 
	if (side && (int)whiteMoveList.size() == 0) return true;
	else if (!side && (int)blackMoveList.size() == 0) return true;
	else return false;
}

bool inCheck(bool side) {
	

	return false;
}
	

bool validateMove(int moveFrom, int moveTo) {
	int value = piece[board64[moveFrom]].value, small, big, diff120 = abs(from64(moveFrom) - from64(moveTo)); 
	small = moveFrom>moveTo ? moveTo : moveFrom;
	big = moveFrom>moveTo ? moveFrom : moveTo;
	diff120 = from64(big) - from64(small);
	
	if (moveFrom == moveTo) return false;
	if (board64[moveFrom] <= wPh && board64[moveFrom] >= wqR && board64[moveTo] <= wPh && board64[moveTo] >= wqR) return false;
	if (board64[moveFrom] >= bqR && board64[moveTo] >= bqR) return false;

	if (value == R_VAL)		return validateHozMove(small, big, moveFrom, moveTo);
	else if (value == N_VAL)	return validateKnightMove(diff120, moveFrom, moveTo);
	else if (value == B_VAL)	return validateDiagMove(small, big, diff120);
	else if (value == Q_VAL) {
		if (validateHozMove(small, big, moveFrom, moveTo)) return true;
		else if (validateDiagMove(small, big, diff120)) return true;
		else return false;
	}
	else if (value == K_VAL) 	return validateKingMove(diff120);
	else if (value == P_VAL) 	return validatePawnMove(diff120, moveFrom, moveTo);

	return false;
}


bool validatePawnMove(int diff120, int moveFrom, int moveTo) {
	int extra = (board64[moveFrom] <= wPh) ? 10 : -10;
	bool side = board64[moveFrom] <= wPh ? 1 : 0;	

	if ((side && moveFrom > moveTo) || (!side && moveFrom < moveTo)) return false; //Ensures correct direction

	if (diff120 == 10 && board64[moveTo] == empty)  {
		return true;
	}
	else if (diff120 == 20 && board64[moveTo] == empty) {
		if (board120[from64(moveFrom)+extra] == empty && piece[board64[moveFrom]].moved == 0) return true;
		else return false;
	}
	else if (diff120 == 9 || diff120 == 11) {
		if ((side && board64[moveTo] >= bqR) || (!side && board64[moveTo] <= wPh && board64[moveTo] >= wqR))  return true;
		else return false;
	}
	else return false;
}

bool validateHozMove(int small, int big, int moveFrom, int moveTo) {
	if ((moveTo - moveFrom)%8 == 0) { //Moving up/down file
		for (int i = small+8; i < big; i+=8)
			if (board64[i] != -1) return false;
		return true;
	}
	else if ((moveFrom-1)/8 == (moveTo-1)/8) { //Move left/right across rank
		for (int i = small+1; i < big; i++)
			if (board64[i] != -1) return false;
		return true;
	}
	else return false;	
}

bool validateDiagMove(int small, int big, int diff120) {
	int temp;
	temp = diff120%11 == 0 ? 11 : diff120%9 == 0 ? 9 : 0;
	if (!temp) return false;
	for (int j = from64(small)+temp; j < from64(big); j+=temp)
		if (board120[j] != -1) return false;
	return true;
}

bool validateKnightMove(int diff120, int moveFrom, int moveTo) {
	int side = board64[moveFrom] <= wkN ? 1 : 0;
	if (diff120 == 8 || diff120 == 12 || diff120 == 19 || diff120 == 21) {
		if ((side && (board64[moveTo] == empty || board64[moveTo] >= bqR)) || (!side && (board64[moveTo] <= wPh))) 
			return true;
	}
	return false;
}

bool validateKingMove(int diff120) {
	return (diff120 == 1 || diff120 == 10 || diff120 == 9 || diff120 == 11) ? true : false;
}

