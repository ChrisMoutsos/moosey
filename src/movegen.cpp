/*
----------------------------------
	~Moosey Chess Engine~
	      movegen.cpp
----------------------------------
*/

#include <iostream>
#include "board.h"

void Board::generateMoveLists() {	
	int mF, mT;
	bool realSide = side;
	int realPieceMoved = pieceMoved;
	int realPrevOnMoveTo = prevOnMoveTo;
	int realEpSq = epSq;

	side = WHITE;
	for (int i = wqR; i <= wPh; i++)
		generateMoveListFor(i);
	side = BLACK;
	for (int i = bqR; i <= bPh; i++)
		generateMoveListFor(i);

	whiteMoveList.clear();
	blackMoveList.clear();
	for (int i = wqR; i <= bPh; i++) 
		for (int j = 0; j < piece[i].moveListSize; j++) 
			if (piece[i].moveList[j] != null) {
				mF = piece[i].pos;
				mT = piece[i].moveList[j];
				if (piece[i].color) 
					whiteMoveList.push_back(mF*100 + mT);
				else
					blackMoveList.push_back(mF*100 + mT);
			}

	side = realSide;	
	pieceMoved = realPieceMoved;
	prevOnMoveTo = realPrevOnMoveTo;
	epSq = realEpSq;
}

void Board::cleanMoveList(bool s) {
	int mF, mT, size;
	bool realSide = side;
	int realPieceMoved = pieceMoved;
	int realPrevOnMoveTo = prevOnMoveTo;
	int realEpSq = epSq;
	size = s ? (int)whiteMoveList.size() : (int)blackMoveList.size();
	for (int i = 0; i < size; i++) {
		mT = s ? whiteMoveList[i]%100 : blackMoveList[i]%100;
		mF = s ? whiteMoveList[i]/100 : blackMoveList[i]/100;
		if (!legalMove(mF, mT, s)) {
			if (s) 
				whiteMoveList.erase(whiteMoveList.begin()+i);
			else
				blackMoveList.erase(blackMoveList.begin()+i);
			size--;
			i--;
		}
	}
	side = realSide;	
	pieceMoved = realPieceMoved;
	prevOnMoveTo = realPrevOnMoveTo;
	epSq = realEpSq;
}

void Board::generateMoveListFor(int p) {
	int counter = 0;
	if (piece[p].alive) {	
		if (piece[p].value == R_VAL) 
			generateHozMoves(p, counter);
		else if (piece[p].value == B_VAL)
			generateDiagMoves(p, counter);
		else if (piece[p].value == N_VAL)
			generateKnightMoves(p, counter);
		else if (piece[p].value == Q_VAL) { 
			generateHozMoves(p, counter);
			generateDiagMoves(p, counter);
		}
		else if (piece[p].value == K_VAL)
			generateKingMoves(p, counter);
		else if (piece[p].value == P_VAL)
			generatePawnMoves(p, counter);
	}
	for (int i = counter; i < piece[p].moveListSize; i++) //Empty the rest of the moveList
		piece[p].moveList[i] = 0;
}

void Board::generateHozMoves(int p, int& counter) {
	bool s = piece[p].color;
	int i, d, posIndex;

	for (int c = 1; c <= 4; c++) {
		d = c==1 ? -1 : c==2 ? 1 : c==3 ? -10 : 10;
		i = 1;
		posIndex = piece[p].pos+d*i;
		while (board120[posIndex] != invalid) {	
			if (board120[posIndex] != empty && piece[board120[posIndex]].color == s) break;
                       
			if (validateMove(piece[p].pos, posIndex, s)) {
				piece[p].moveList[counter] = posIndex;
                        	counter++;
			}
			i++;
			if (board120[posIndex] != empty && piece[board120[posIndex]].color != s) break;
                        posIndex = piece[p].pos+d*i;
                }
	}
}


void Board::generateDiagMoves(int p, int& counter) {
	bool s = piece[p].color;
	int d, i, posIndex;

	for (int c = 1; c <= 4; c++) {
		d = c==1 ? -11 : c==2 ? -9 : c==3 ? 11 : 9;
		i = 1;
		posIndex = piece[p].pos+d*i;
		
		while (board120[posIndex] != invalid) { 
			if (board120[posIndex] != empty && piece[board120[posIndex]].color == s) break;
                        
			if (validateMove(piece[p].pos, posIndex, s)) {
				piece[p].moveList[counter] = posIndex;
                        	counter++;
			}
			i++;
			if (board120[posIndex] != empty && piece[board120[posIndex]].color != s) break;
                        posIndex = piece[p].pos+d*i;
                }
	}
}

void Board::generateKnightMoves(int p, int& counter) {
	bool s = piece[p].color;
	int extra;
	for (int i = 1; i <= 8; i++) {
		extra = i==1 ? 8 : i==2 ? -8 : i==3 ? 12 : i==4 ? -12 : i==5 ? 19 : i==6 ? -19 : i==7 ? 21 : -21;
		if (validateMove(piece[p].pos, piece[p].pos + extra, s)) {
			piece[p].moveList[counter] = piece[p].pos + extra;
			counter++;
		}
	}
}

void Board::generateKingMoves(int p, int& counter) {
	bool s = piece[p].color;
	int extra;
        for (int i = 1; i <= 8; i++) {
                extra = i==1 ? 1 : i==2 ? -1 : i==3 ? 10 : i==4 ? -10 : i==5 ? 11 : i==6 ? -11 : i==7 ? 9 : -9;
                if (validateMove(piece[p].pos, piece[p].pos+extra, s)) {
                        piece[p].moveList[counter] = piece[p].pos + extra;
                        counter++;
                }
        }
	if (canCastle(KINGSIDE, s)) { 
		piece[p].moveList[counter] = s ? _G1 : _G8;
		counter++;
	}
	if (canCastle(QUEENSIDE, s)) { 
		piece[p].moveList[counter] = s ? _B1 : _B8;
		counter++;
	}

}

void Board::generatePawnMoves(int p, int& counter) {
	bool s = piece[p].color;
	int extra;
        for (int i = 1; i <= 4; i++) {
                extra = i==1 ? 10 : i==2 ? 20 : i==3 ? 11 : 9;
		extra = side ? extra : -extra;
                if (validateMove(piece[p].pos, piece[p].pos+extra, s)) {
                        piece[p].moveList[counter] = piece[p].pos + extra;
                        counter++;
                }
        }
}
