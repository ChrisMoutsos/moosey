/*
----------------------------------
	~Moosey Chess Engine~
	      legal.cpp
----------------------------------
*/

#include <iostream>
#include "cmath"
#include "board.h"

using namespace std;

bool Board::legalMove(int mF, int mT, bool v) { 
	bool isInCheck;
	if (!validateMove(mF, mT)) {
		if (v) cout << "Illegal move.\n";
		return false;
	}
	movePiece(mF, mT);
	isInCheck = inCheck();
	unmovePiece(mF, mT);
	return isInCheck ? false : true;
}

bool Board::checkDraw() const {
	if ((int)whiteMoveList.size() == 0 && (int)blackMoveList.size() == 0) {
		cout << "Stalemate!" << endl;
		return true;
	}	
	return false;
}

bool Board::checkCheck() const {
	if (inCheck()) {
		if (inCheckmate()) { 
			side ? cout << "White is in checkmate. Black wins!\n" : cout << "Black is in checkmate. White wins!\n";
			return true;
		}
		else side ? cout << "White is in check!\n" : "Black is in check!\n";
	}
	return false;
}

bool Board::inCheckmate() const { 
	if (side && (int)whiteMoveList.size() == 0) return true;
	else if (!side && (int)blackMoveList.size() == 0) return true;
	else return false;
}

bool Board::inCheck() const {
	

	return false;
}

bool Board::validateMove(int mF, int mT) const {
	int value = piece[board64[mF]].value, small, big, diff120 = abs(from64(mF) - from64(mT)); 
	small = mF>mT ? mT : mF;
	big = mF>mT ? mF : mT;
	diff120 = from64(big) - from64(small);
	
	if (board64[mF] == -1) 
		return false;
	if (board64[mT] != empty && (piece[board64[mF]].color == piece[board64[mT]].color)) 
		return false;
	if (piece[board64[mF]].color != side) 
		return false;

	if (value == R_VAL)
		return validateHozMove(small, big, mF, mT);
	else if (value == N_VAL)
		return validateKnightMove(diff120, mF, mT);
	else if (value == B_VAL)
		return validateDiagMove(small, big, diff120);
	else if (value == Q_VAL) 
		return (validateHozMove(small, big, mF, mT) | (validateDiagMove(small, big, diff120)));
	else if (value == K_VAL)
		return validateKingMove(diff120);
	else if (value == P_VAL)
		return validatePawnMove(diff120, mF, mT);

	return false;
}


bool Board::validatePawnMove(int diff120, int mF, int mT) const {
	int extra = piece[board64[mF]].color ? 10 : -10;
	bool side = piece[board64[mF]].color ? 1 : 0;	

	if ((side && mF > mT) || (!side && mF < mT)) return false; //Ensures correct direction

	if (diff120 == 10 && board64[mT] == empty) 
		return true;
	else if (diff120 == 20 && board64[mT] == empty) {
		if (board120[from64(mF)+extra] == empty && piece[board64[mF]].moved == 0) return true;
		else return false;
	}
	else if (diff120 == 9 || diff120 == 11) {
		if ((side && !piece[board64[mT]].color) || (!side && piece[board64[mT]].color))  return true;
		else return false;
	}
	else return false;
}

bool Board::validateHozMove(int small, int big, int mF, int mT) const {
	if ((mT - mF)%8 == 0) { //Moving up/down file
		for (int i = small+8; i < big; i+=8)
			if (board64[i] != empty) return false;
		return true;
	}
	else if ((mF-1)/8 == (mT-1)/8) { //Move left/right across rank
		for (int i = small+1; i < big; i++)
			if (board64[i] != empty) return false;
		return true;
	}
	else return false;	
}

bool Board::validateDiagMove(int small, int big, int diff120) const {
	int temp;
	temp = diff120%11 == 0 ? 11 : diff120%9 == 0 ? 9 : 0;
	if (!temp) return false;
	for (int j = from64(small)+temp; j < from64(big); j+=temp)
		if (board120[j] != empty) return false;
	return true;
}

bool Board::validateKnightMove(int diff120, int mF, int mT) const {
	int side = piece[board64[mF]].color ? 1 : 0;
	if (diff120 == 8 || diff120 == 12 || diff120 == 19 || diff120 == 21) {
		if (board64[mT] == empty || !side != !piece[board64[mT]].color)
			return true;
	}
	return false;
}

bool Board::validateKingMove(int diff120) const {
	return (diff120 == 1 || diff120 == 10 || diff120 == 9 || diff120 == 11) ? true : false;
}

