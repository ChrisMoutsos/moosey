/*
----------------------------------
	~Moosey Chess Engine~
	      legal.cpp
----------------------------------
*/

#include <iostream>
#include <string>
#include "cmath"
#include "board.h"
#include "ltexture.h"

bool Board::legalMove(int mF, int mT, bool s, bool v) { 
	int realEpSq = epSq;
	
	if (!validateMove(mF, mT, s)) { //Make sure the piece moves in that way
		if (v) std::cout << "Invalid move..\n";
		return false;
	}
	if (castling)	      //Legalization of castling occurs in validateMove
		return true;
	
	movePiece(mF, mT);    //Move the piece,
	inCheck(s);		      //see if we put our king in check
	unmovePiece(mF, mT);  //unmove the piece
	
	epSq = realEpSq;
	if (sideInCheck) {    //If either side is in check
		if (v) std::cout << "Illegal move.\n";
		return false;
	}
	return true;
}

bool Board::checkStalemate() const {
	if ((int)whiteMoveList.size() == 0 && (int)blackMoveList.size() == 0) {
		std::cout << "Stalemate!\n";
		return true;
	}	
	return false;
}

bool Board::checkCheck(bool s) {
	std::string checkStr = " ";

	sideInCheck = 0;
	sideInCheckmate = 0;
	
	if (inCheck(s)) {
		sideInCheck = s ? 1 : 2;
		cleanMoveList(s);
		if (inCheckmate(s)) { //Checkmate
			sideInCheckmate = s ? 1 : 2;
			checkStr = s ? "Black wins!" : "White wins!";
			checkText.loadFromRenderedText(checkStr, textColor, Garamond26);
			return true;
		}
		else {				  //Check
			checkStr = s ? "White is in check" : "Black is in check";
			checkText.loadFromRenderedText(checkStr, textColor, Garamond26);
			return false;
		}
	}
	checkText.loadFromRenderedText(" ", textColor, Garamond26);
	return false;	//Neither check nor checkmate
}

bool Board::inCheckmate(bool s) const { 
	if (s && (int)whiteMoveList.size() == 0) 
		return true;
	else if (!s && (int)blackMoveList.size() == 0) 
		return true;
	return false;
}

bool Board::inCheck(bool s) const {
	int kPos, pIndex, i, v, d;
	kPos = s ? piece[wK].pos : piece[bK].pos;

	/* Search ranks/files/diagonals for appropriate piece. */
	for (int c = 1; c <= 8; c++) {
		d = c==1 ? L : c==2 ? R : c==3 ? U : c==4 ? D : c==5 ? UL : c==6 ? UR : c==7 ? DL : DR;
		i = 1;
		pIndex = kPos+d*i;
		while (board120[pIndex] != invalid) { 
			if (board120[pIndex] != empty) { 
				if (piece[board120[pIndex]].color != s) { 
					v = piece[board120[pIndex]].value;
					if (v == Q_VAL)
						return true;
					if (c >= 1 && c <= 4) {
						if (v == R_VAL)
							return true;
					} 
					else 
						if (v == B_VAL)
							return true;
					if (i == 1 && c >= 5 && v == P_VAL) 
						if (!s != (d==UL || d==UR))
							return true;
				}
				break;
			}
			i++;
			pIndex = kPos+d*i;
		}
	}
	
	/* Search the knight squares around the king for a knight. */
	for (int c = 1; c <= 8; c++) {
		d = c==1 ? K1 : c==2 ? K2 : c==3 ? K3 : c==4 ? K4 : c==5 ? K5 : c==6 ? K6 : c==7 ? K7 : K8;
		pIndex = kPos + d;
		if (board120[pIndex] != empty && board120[pIndex] != invalid)
			if (piece[board120[pIndex]].color == !s)
				if (piece[board120[pIndex]].value == N_VAL)
					return true;
		
	}

	return false;
}

bool Board::validateMove(int mF, int mT, bool s) {
	int onMF = board120[mF], onMT = board120[mT], value = piece[onMF].value;

	//Moving empty square, to null square, or invalid square
	if (onMF == -1 || mT == 0 || board120[mT] == invalid) 
		return false;
	//Trying to capture your own piece
	if (onMT != empty && piece[onMF].color == piece[onMT].color && !castling) 
		return false;
	//Trying to move enemy piece
	if (piece[onMF].color != s) 
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
		return validateKingMove(mF, mT, s);
	else if (value == P_VAL)
		return validatePawnMove(mF, mT, s);

	return false;
}

bool Board::validatePawnMove(int mF, int mT, bool s) const {
	int extra = s ? 10 : -10;
	int onMF = board120[mF], onMT = board120[mT];
	int diff = abs(mF - mT);

	if ((s && mF > mT) || (!s && mF < mT))  //Ensures correct direction
		return false;

	if (diff == 10 && onMT == empty)	//Moving forward one square
		return true;
	if (diff == 20 && onMT == empty) {	//Moving forward two squares
		if (board120[mF+extra] == empty && piece[onMF].moved == 0) 
			return true;
		else return false;
	}
	if (diff == 9 || diff == 11) {		//Attacking
		if (onMT == empty) { 	
			if (mT == epSq) 	//En passanting
				return true;
			return false;	
		}
		if (s != piece[onMT].color) return true;
		else return false;
	}
	return false;
}

bool Board::validateHozMove(int mF, int mT) const {
	int small = mF>mT ? mT : mF, big = mF>mT ? mF : mT;	

	if ((mT - mF)%10 == 0) { //Moving up/down file
		for (int i = small+10; i < big; i+=10)
			if (board120[i] != empty) return false;
		return true;
	}
	else if (mF/10 == mT/10) { //Move left/right across rank
		for (int i = small+1; i < big; i++)
			if (board120[i] != empty) return false;
		return true;
	}
	else return false;	
}

bool Board::validateDiagMove(int mF, int mT) const {
	int temp, small = mF>mT ? mT : mF, big = mF>mT ? mF : mT;	
	int diff = big - small;

	temp = diff%11 == 0 ? 11 : diff%9 == 0 ? 9 : 0;
	if (!temp) return false; //Not moving diagonally
	for (int j = small+temp; j < big; j+=temp)
		if (board120[j] != empty) return false;
	return true;
}

bool Board::validateKnightMove(int mF, int mT) const {
	int diff = abs(mF - mT);
	
	if (diff == 8 || diff == 12 || diff == 19 || diff == 21)
		return true;
	return false;
}

bool Board::validateKingMove(int mF, int mT, bool s) {
	int diff = abs(mF - mT);
	
	if (diff == 1 || diff == 10 || diff == 9 || diff == 11)
		return true;
	if ((mF - mT) == -2) {	//Castling kingside
		if (canCastle(KINGSIDE, s)) {
			setCastling(KINGSIDE);
			return true;
		}
	}
	if ((mF - mT) == 3) {   //Castling queenside
		if (canCastle(QUEENSIDE, s)) {
			setCastling(QUEENSIDE);
			return true;
		}
	}
	return false;
}

bool Board::canCastle(int dir, bool s) {
	int k = s ? wK : bK;
	int r = s ? dir == KINGSIDE ? wkR : wqR : dir == KINGSIDE ? bkR : bqR;
	int kSq, c = dir == KINGSIDE ? 1 : -1;

	if (piece[k].moved || piece[r].moved) return false;
	for (int i = 1; i <= 2; i++)	//Verify it's empty between K and R
		if (board120[piece[k].pos+i*c] != empty) return false;
	if (inCheck(s)) return false;	//Verify not in check
	
	for (int j = 1; j <= 2; j++) {	//Verify not going through/to check
		kSq = piece[k].pos;
		movePiece(kSq, kSq+j*c);
		if (inCheck(s)) {
			unmovePiece(kSq, kSq+j*c);
			return false;
		}
		unmovePiece(kSq, kSq+j*c);
	}
	return true;
}
