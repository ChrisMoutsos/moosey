/*
----------------------------------
	~Moosey Chess Engine~
	      legal.cpp
----------------------------------
*/

#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include "common.h"
#include "board.h"
#include "ltexture.h"

bool Board::legalMove(int mF, int mT, bool s, bool v) { 
	bool isInCheck;
	
	if (!validateMove(mF, mT, s)) { //Verify piece moves that way
		if (v) std::cout << "Invalid move..\n";
		return false;
	}
	
	//Move the piece and see if we put our king in check
	movePiece(mF, mT);
	isInCheck = inCheck(s);	
	unmovePiece(mF, mT);
	
	if (isInCheck) { 
		if (v) std::cout << "Illegal move.\n";
		return false;
	}
	return true;
}

bool Board::putSelfInCheck(int mF, int mT, bool s) {
	bool isInCheck;

	movePiece(mF, mT);
	isInCheck = inCheck(s);
	unmovePiece(mF, mT);

	return isInCheck;
}

bool Board::checkCheck(bool s) {
	//Cleans wML or bML of illegal moves

	if (s)
		return checkCheck(s, whiteMoveList);
	else
		return checkCheck(s, blackMoveList);
}

bool Board::checkCheck(bool s, std::vector<int>& moveList) {
	//Cleans moveList of illegal moves, sets sideInCheck(mate) appropriately

	sideInCheck = 0;
	sideInCheckmate = 0;
	
	if (inCheck(s)) {
		sideInCheck = s ? 1 : 2;
		cleanMoveList(s, moveList);
		if (moveList.size() == 0) {
			sideInCheckmate = s ? 1 : 2;
			return true;
		}
	}
	return false;	//Not checkmate
}

bool Board::inCheck(bool s) const {
	//Statically looks at a position and tells you if side s is in check

	int kPos, pIndex, i, v, d;
	
	//If the king is dead, you're most likely in check
	if (s && !piece[wK].getAlive()) return true;
	if (!s && !piece[bK].getAlive()) return true;

	kPos = s ? piece[wK].getPos() : piece[bK].getPos();

	// Search ranks/files/diagonals for appropriate piece.
	for (int c = 1; c <= 8; c++) {	//Checking eight directions
		//Pick the appropriate direction depending on which check we're on
		d = c==1 ? L : c==2 ? R : c==3 ? U : c==4 ? D : c==5 ? UL : c==6 ? UR : c==7 ? DL : DR;
		i = 1;
		pIndex = kPos+d*i;
		while (board120[pIndex] != invalid) {
			//If square is empty, continue in that direction
			if (board120[pIndex] == empty) {
				i++;
				pIndex = kPos+d*i;
				continue; 
			}
			//If we run into a piece of our own color, change direction
			if (piece[board120[pIndex]].getColor() == s) 
				break;
			//If we run into a piece of enemy color, check its type
			v = piece[board120[pIndex]].getValue();
			if (v == Q_VAL)		//Queen on hoz or diag
				return true;
			if (c >= 1 && c <= 4) {
				if (v == R_VAL) //Rook on hoz
					return true;
			} 
			else if (v == B_VAL)    //Bishop on diag
				return true;
			if (i == 1) {		//First square from king in direction d
				if (c >= 5 && v == P_VAL) { //Pawns
					if (!s != (d==UL || d==UR))
						return true;
				}
				if (v == K_VAL) //Can't be next to a king
					return true;
			}
			break;			 //Change direction
		}
	}
	
	// Search the knight squares around the king for a knight.
	for (int c = 1; c <= 8; c++) {
		d = c==1 ? K1 : c==2 ? K2 : c==3 ? K3 : c==4 ? K4 : c==5 ? K5 : c==6 ? K6 : c==7 ? K7 : K8;
		pIndex = kPos + d; 
		if (board120[pIndex] == empty || board120[pIndex] == invalid) continue;
		if (piece[board120[pIndex]].getColor() == s) continue;
		if (piece[board120[pIndex]].getValue() == N_VAL) return true;
	}

	return false; //Not in check
}

bool Board::validateMove(int mF, int mT, bool s) {
	//Pseudolegal verification that mF can move to mT (for side s)	

	int onMF = board120[mF], onMT = board120[mT], value = piece[onMF].getValue();

	//Moving empty square, to null square, or invalid square
	if (onMF == -1 || mT == 0 || board120[mT] == invalid) 
		return false;
	//Trying to capture your own piece
	if (onMT != empty && piece[onMF].getColor() == piece[onMT].getColor()) 
		return false;
	//Trying to move enemy piece
	if (piece[onMF].getColor() != s) 
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

	if (board120[mT] == invalid) return false;

	//Ensure correct direction
	if ((s && mF > mT) || (!s && mF < mT))
		return false;

	//Moving forward one square
	if (diff == 10 && onMT == empty)
		return true;
	//Moving forward two squares
	if (diff == 20 && onMT == empty) {
		if (board120[mF+extra] == empty && piece[onMF].getMoved() == 0) 
			return true;
		else return false;
	}
	//Attacking
	if (diff == 9 || diff == 11) {	
		//En passant
		if (onMT == empty) { 	
			if (moveInfo.size() > 1 && mT == moveInfo.back().epSq)
				if ((s && moveInfo.back().epSq > H5) 
				   || (!s && moveInfo.back().epSq < A4))
					return true;
			return false;	
		}
		if (s != piece[onMT].getColor()) return true;
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
			return true;
		}
	}
	if ((mF - mT) == 2) {   //Castling queenside
		if (canCastle(QUEENSIDE, s)) {
			return true;
		}
	}
	return false;
}

bool Board::canCastle(int dir, bool s) {
	//Psuedo-legalizes castling. Meaning you can castle into check

	int k = s ? wK : bK;
	int r = s ? dir == KINGSIDE ? wkR : wqR : dir == KINGSIDE ? bkR : bqR;
	int c = dir == KINGSIDE ? 1 : -1;

	//If your king or rook is dead, you can't castle
	if (!(piece[k].getAlive() && piece[r].getAlive())) return false;
	if (piece[k].getMoved() || piece[r].getMoved()) return false;

	for (int i = 1; i <= 2; i++)	//Verify it's empty between K and R
		if (board120[piece[k].getPos()+i*c] != empty) return false;
	if (dir == QUEENSIDE)
		if (board120[piece[k].getPos()-3] != empty) return false;

	if (inCheck(s)) return false; //Make sure not currently in check

	//Verify not going through check
	movePiece(piece[k].getPos(), piece[k].getPos()+c);
	if (inCheck(s)) {
		unmovePiece();
		return false;
	}
	unmovePiece();
	
	return true;
}

int Board::drawCheck() const {
	int lastPly = movesMade.size();

	if (lastPly < 3)
		return false;

	int count = 0;

	for (int j = lastPly - 3; j >= lastPly - moveInfo.back().halfMoveClock - 1; j -= 2) {
		if (j < 0) break;
		if (moveInfo[lastPly-1].zobrist == moveInfo[j].zobrist) {
			count++;
			if (count == 2)
				return 2;
		}
	}
	
	return count;
}
