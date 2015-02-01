/*
----------------------------------
	~Moosey Chess Engine~
	      legal.cpp
----------------------------------
*/

#include <iostream>
#include "cmath"
#include "board.h"

bool Board::legalMove(int mF, int mT, bool verbose) { 
	bool isInCheck;
	if (!validateMove(mF, mT)) {
		if (verbose) std::cout << "Illegal move.\n";
		return false;
	}
	movePiece(mF, mT);
	isInCheck = inCheck();
	unmovePiece(mF, mT);
	return isInCheck ? false : true;
}

bool Board::checkDraw() const {
	if ((int)whiteMoveList.size() == 0 && (int)blackMoveList.size() == 0) {
		std::cout << "Stalemate!\n";
		return true;
	}	
	return false;
}

bool Board::checkCheck() const {
	if (inCheck()) {
		if (inCheckmate()) { 
			side ? std::cout << "White" : std::cout << "Black";
			std::cout << " is in checkmate. ";
			side ? std::cout << "Black wins!\n" : std::cout << "White wins!\n";
			return true;
		}
		else side ? std::cout << "White is in check!\n" : "Black is in check!\n";
	}
	return false;
}

bool Board::inCheckmate() const { 
	if (side && (int)whiteMoveList.size() == 0) 
		return true;
	else if (!side && (int)blackMoveList.size() == 0) 
		return true;
	return false;
}

bool Board::inCheck() const {
	

	return false;
}

bool Board::validateMove(int mF, int mT) const {
	int value = piece[board64[mF]].value, onMF = board64[mF], onMT = board64[mT];

	if (onMF == -1 || mT == 0) 
		return false;
	if (onMT != empty && piece[onMF].color == piece[onMT].color) 
		return false;
	if (piece[onMF].color != side) 
		return false;

	if (value == R_VAL)
		return validateHozMove(mF, mT);
	else if (value == N_VAL)
		return validateKnightMove(mF, mT);
	else if (value == B_VAL)
		return validateDiagMove(mF, mT);
	else if (value == Q_VAL) 
		return (validateHozMove(mF, mT) | (validateDiagMove(mF, mT)));
	else if (value == K_VAL)
		return validateKingMove(mF, mT);
	else if (value == P_VAL)
		return validatePawnMove(mF, mT);

	return false;
}


bool Board::validatePawnMove(int mF, int mT) const {
	int extra = piece[board64[mF]].color ? 10 : -10;
	int onMF = board64[mF], onMT = board64[mT];
	int diff120 = abs(from64(mF) - from64(mT));

	if ((side && mF > mT) || (!side && mF < mT)) //Ensures correct direction
		return false;

	if (diff120 == 10 && onMT == empty) 
		return true;
	else if (diff120 == 20 && onMT == empty) {
		if (board120[from64(mF)+extra] == empty && piece[onMF].moved == 0) return true;
		else return false;
	}
	else if (diff120 == 9 || diff120 == 11) {
		if (onMT == empty) return false;
		if (!side != !piece[onMT].color) return true;
		else return false;
	}
	else 
		return false;
}

bool Board::validateHozMove(int mF, int mT) const {
	int small = mF>mT ? mT : mF, big = mF>mT ? mF : mT;	

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

bool Board::validateDiagMove(int mF, int mT) const {
	int temp, small = mF>mT ? mT : mF, big = mF>mT ? mF : mT;	
	int diff120 = from64(big) - from64(small);

	temp = diff120%11 == 0 ? 11 : diff120%9 == 0 ? 9 : 0;
	if (!temp) return false;
	for (int j = from64(small)+temp; j < from64(big); j+=temp)
		if (board120[j] != empty) return false;
	return true;
}

bool Board::validateKnightMove(int mF, int mT) const {
	int onMT = board64[mT], diff120 = abs(from64(mF) - from64(mT));
	
	if (diff120 == 8 || diff120 == 12 || diff120 == 19 || diff120 == 21) {
		if (onMT == empty || !side != !piece[onMT].color)
			return true;
	}
	return false;
}

bool Board::validateKingMove(int mF, int mT) const {
	int diff120 = abs(from64(mF) - from64(mT));
	return (diff120 == 1 || diff120 == 10 || diff120 == 9 || diff120 == 11) ? true : false;
}

