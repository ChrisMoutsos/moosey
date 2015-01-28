#include <iostream>
#include "board.h"
#include "legal.h"
#include "pieces.h"
#include "move.h"
#include "cmath"

using namespace std;

bool legalMove(int mF, int mT, bool side) { //Checks moveFrom to moveTo by default
	bool isInCheck;

	if (!validateMove(mF, mT)) return false;
	movePiece(mF, mT);
	isInCheck = inCheck(side);
	unmovePiece(mF, mT);
	return isInCheck ? false : true;
}

void checkDraw() {
	if ((int)whiteMoveList.size() == 0 && (int)blackMoveList.size() == 0) {
		cout << "Stalemate!" << endl;
		exit = true;
	}	
}

void checkCheck(bool side) {
	if (inCheck()) {
		if (inCheckmate()) { 
			side ? cout << "White is in checkmate. Black wins!\n" : cout << "Black is in checkmate. White wins!\n";
			exit = true;
		}
		else side ? cout << "White is in check!\n" : "Black is in check!\n";
	}
}

bool inCheckmate(bool side) { //Default side = global side
	if (side && (int)whiteMoveList.size() == 0) return true;
	else if (!side && (int)blackMoveList.size() == 0) return true;
	else return false;
}

bool inCheck(bool side) { //Default side = global side
	int d, i, kPos, posIndex, iPiece;
	int startPiece, endPiece, enemyStartPiece, enemyEndPiece;
	startPiece = side ? 0 : 16;
	endPiece = side ? 15 : 31;
	enemyStartPiece = side ? 16 : 0;
	enemyEndPiece = side ? 31 : 15;
	kPos = side ? piece[wK].pos : piece[bK].pos;

	for (int c = 1; c <= 4; c++) { //Check left/right/up/down
		d = (c==1) ? -1 : (c==2) ? 1 : (c==3) ? -8 : 8;
		i = 1;
		posIndex = kPos + d*i;

		while (((posIndex-1)/8 == (kPos-1)/8) || ((posIndex)%8 == (kPos)%8)) {
                        if (posIndex < 1 || posIndex > 64) break;
			else iPiece = board64[posIndex];

                        if (iPiece >= enemyStartPiece && iPiece <= enemyEndPiece) {
				if (i==1 && piece[iPiece].value == K_VAL) return true;
				if (piece[iPiece].value == R_VAL || piece[iPiece].value == Q_VAL) return true;	
			}
			else if (iPiece >= startPiece && iPiece <= endPiece) break;

			i++;
                        posIndex = kPos + d*i;
                }
	}
	for (int c = 1; c <= 4; c++) { //Check diagonals
		d = (c==1) ? -9 : (c==2) ? -7 : (c==3) ? 9 : 7;
		i = 1;
		posIndex = kPos + d*i;
		
		while (((from64(posIndex) - from64(kPos))%11 == 0 || (from64(posIndex) - from64(kPos))%9 == 0)) { 
                        if (posIndex < 1 || posIndex > 64) break;
			else iPiece = board64[posIndex];
			
			if (iPiece >= enemyStartPiece && iPiece <= enemyEndPiece) {
				if (i==1 && piece[iPiece].value == P_VAL) return true;
				if (piece[iPiece].value == B_VAL || piece[iPiece].value == Q_VAL) return true;
			}
			else if (iPiece >= startPiece && iPiece <= endPiece) break;
			
			i++;
			posIndex = kPos + d*i;
		}
	}
	kPos = from64(kPos);
	for (int c = 1; c <= 8; c++) { //Check knights
		d = (c==1) ? -19 : (c==2) ? -21 : (c==3) ? -8 : (c==4) ? -12 : (c==5) ? 12 : (c==6) ? 8 : (c==7) ? 21 : 19;
		
		if (piece[board120[kPos+d]].value == N_VAL && (board120[kPos+d] >= enemyStartPiece && board120[kPos+d] <= enemyEndPiece))
			return true;
	}	
	return false;
}


bool validateMove(int moveFrom, int moveTo) {
	int value = piece[board64[moveFrom]].value, small, big, diff120 = abs(from64(moveFrom) - from64(moveTo)); 
	small = moveFrom>moveTo ? moveTo : moveFrom;
	big = moveFrom>moveTo ? moveFrom : moveTo;
	
	if (moveFrom == moveTo) return false;
	if (board64[moveFrom] <= wPh && board64[moveFrom] >= wqR && board64[moveTo] <= wPh && board64[moveTo] >= wqR) return false;
	if (board64[moveFrom] >= bqR && board64[moveTo] >= bqR) return false;

	if (value == R_VAL)		return validateHozMove(small, big, diff120, moveFrom, moveTo);
	else if (value == N_VAL)	return validateKnightMove(diff120, moveFrom, moveTo);
	else if (value == B_VAL)	return validateDiagMove(small, big, diff120, moveFrom, moveTo);
	else if (value == Q_VAL) {
		if (validateHozMove(small, big, diff120, moveFrom, moveTo)) return true;
		else if (validateDiagMove(small, big, diff120, moveFrom, moveTo)) return true;
		else return false;
	}
	else if (value == K_VAL) 	return validateKingMove(diff120, moveFrom, moveTo);
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

bool validateHozMove(int small, int big, int diff120, int moveFrom, int moveTo) {
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

bool validateDiagMove(int small, int big, int diff120, int moveFrom, int moveTo) {
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

bool validateKingMove(int diff120, int moveFrom, int moveTo) {
	return (diff120 == 1 || diff120 == 10 || diff120 == 9 || diff120 == 11) ? true : false;
}

