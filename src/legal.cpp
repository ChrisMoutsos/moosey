/*
----------------------------------
	~Moosey Chess Engine~
	      legal.cpp
----------------------------------
*/

#include <iostream>
#include "cmath"
#include "board.h"

bool Board::legalMove(int mF, int mT, bool s, bool verbose) { 
	bool isInCheck;
	if (!validateMove(mF, mT)) {
		if (verbose) std::cout << "Illegal move.\n";
		return false;
	}
	movePiece(mF, mT);
	isInCheck = inCheck(s);
	unmovePiece(mF, mT);
	return isInCheck ? false : true;
}

bool Board::checkStalemate() const {
	if ((int)whiteMoveList.size() == 0 && (int)blackMoveList.size() == 0) {
		std::cout << "Stalemate!\n";
		return true;
	}	
	return false;
}

void Board::cleanMovelists() {
	
}

bool Board::checkCheck(bool side) {
	if (inCheck(side)) {
		if (inCheckmate(side)) { 
			side ? std::cout << "White" : std::cout << "Black";
			std::cout << " is in checkmate. ";
			side ? std::cout << "Black wins!\n" : std::cout << "White wins!\n";
			return true;
		}
		else {
			side ? std::cout << "White is in check!\n\n" : std::cout << "Black is in check!\n\n";
			return false;
		}
	}
	return false;
}

bool Board::inCheckmate(bool side) const { 
	if (side && (int)whiteMoveList.size() == 0) 
		return true;
	else if (!side && (int)blackMoveList.size() == 0) 
		return true;
	return false;
}

bool Board::inCheck(bool side) {
	int kingPos, small, big, diff120, diff2, dir;
	kingPos = side ? piece[wK].pos : piece[bK].pos;
	
	for (int d = -1; d <= 1; d+=2) {

	}
	for (int d = -10; d <= 10; d+=20) {

	}
	for (int d = -11; d <= 11; d+=22) {

	}
	for (int d = -9; d <= 9; d+=18) {

	}
	for (int c = 1; c <= 8; c++) {

	}

	return false;
}

bool Board::validateMove(int mF, int mT) const {
	int value = piece[board64[mF]].value, onMF = board64[mF], onMT = board64[mT];
	//std::cout << "validating " << mF << " to " << mT << std::endl;
	if (onMF == -1 || mT == 0) 
		return false;
	if (onMT != empty && piece[onMF].color == piece[onMT].color) 
		return false;
	if (piece[onMF].color != side) {
		//std::cout << "not your piece" << std::endl;
		return false;
	}

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
	if (diff120 == 20 && onMT == empty) {
		if (board120[from64(mF)+extra] == empty && piece[onMF].moved == 0) return true;
		else return false;
	}
	if (diff120 == 9 || diff120 == 11) {
		if (onMT == empty) return false;
		if (!side != !piece[onMT].color) return true;
		else return false;
	}
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
	int onMT = board120[from64(mT)], onMF = board120[from64(mF)];
	int diff120 = abs(from64(mF) - from64(mT));
	
	if (diff120 == 8 || diff120 == 12 || diff120 == 19 || diff120 == 21) {
		if (onMT == empty) 
			return true;
		if (!piece[onMF].color != !piece[onMT].color)
			return true;
	}
	return false;
}

bool Board::validateKingMove(int mF, int mT) const {
	int diff120 = abs(from64(mF) - from64(mT));
	return (diff120 == 1 || diff120 == 10 || diff120 == 9 || diff120 == 11) ? true : false;
}

