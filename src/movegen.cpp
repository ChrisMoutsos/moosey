/*
----------------------------------
	~Moosey Chess Engine~
	      movegen.cpp
----------------------------------
*/

#include <iostream>
#include "board.h"

void Board::generateMoveLists() {	
	bool realSide = side;
	side = WHITE;
	for (int i = wqR; i <= wPh; i++)
		generateMoveListFor(i);
	side = BLACK;
	for (int i = bqR; i <= bPh; i++)
		generateMoveListFor(i);
	side = realSide;

	whiteMoveList.clear();
	blackMoveList.clear();
	for (int i = wqR; i <= wPh; i++) {
		for (int j = 0; j < piece[i].moveListSize; j++) 
			if (piece[i].moveList[j] != null)
				whiteMoveList.push_back(piece[i].moveList[j]);
	}
	for (int i = bqR; i <= bPh; i++) {
		for (int j = 0; j < piece[i].moveListSize; j++) 
			if (piece[i].moveList[j] != null)
				blackMoveList.push_back(piece[i].moveList[j]);
	}
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
	bool side = piece[p].color;
	int d, i, posIndex;

	for (int c = 1; c <= 4; c++) {
		d = (c==1) ? -1 : (c==2) ? 1 : (c==3) ? -8 : 8;
		i = 1;
		posIndex = piece[p].pos+d*i;
		
		while (((posIndex-1)/8 == (piece[p].pos-1)/8) || ((posIndex)%8 == (piece[p].pos)%8)) { 
			if (posIndex < 1 || posIndex > 64) break;
			if (board64[posIndex] != empty && piece[board64[posIndex]].color == side) break;
                        
			if (legalMove(piece[p].pos, posIndex)) {
				piece[p].moveList[counter] = posIndex;
                        	counter++;
			}
			i++;
			if (board64[posIndex] != empty && piece[board64[posIndex]].color != side) break;
                        posIndex = piece[p].pos+d*i;
                }
	}

}


void Board::generateDiagMoves(int p, int& counter) {
	bool side = piece[p].color;
	int d, i, posIndex;

	for (int c = 1; c <= 4; c++) {
		d = (c==1) ? -9 : (c==2) ? -7 : (c==3) ? 9 : 7;
		i = 1;
		posIndex = piece[p].pos+d*i;
		
		while (((from64(posIndex) - from64(piece[p].pos))%11 == 0 || (from64(posIndex) - from64(piece[p].pos))%9 == 0)) { 
                        if (posIndex < 1 || posIndex > 64) break;
			if (board64[posIndex] != empty && piece[board64[posIndex]].color == side) break;
                        
			if (legalMove(piece[p].pos, posIndex)) {
				piece[p].moveList[counter] = posIndex;
                        	counter++;
			}
			i++;
			if (board64[posIndex] != empty && piece[board64[posIndex]].color != side) break;
                        posIndex = piece[p].pos+d*i;
                }
	}
}

void Board::generateKnightMoves(int p, int& counter) {
	int extra;
	for (int i = 1; i <= 8; i++) {
		extra = i==1 ? 8 : (i==2 ? -8 : (i==3 ? 12 : (i==4 ? -12 : (i==5 ? 19 : (i==6 ? -19 : (i==7 ? 21 : -21))))));
		if (legalMove(piece[p].pos, to64(from64(piece[p].pos) + extra))) {
			piece[p].moveList[counter] = to64(from64(piece[p].pos) + extra);
			counter++;
		}
	}
}

void Board::generateKingMoves(int p, int& counter) {
	int extra;
        for (int i = 1; i <= 8; i++) {
                extra = i==1 ? 1 : (i==2 ? -1 : (i==3 ? 10 : (i==4 ? -10 : (i==5 ? 11 : (i==6 ? -11 : (i==7 ? 9 : -9))))));
                if (legalMove(piece[p].pos, to64(from64(piece[p].pos)+extra))) {
                        piece[p].moveList[counter] = to64(from64(piece[p].pos) + extra);
                        counter++;
                }
        }

}

void Board::generatePawnMoves(int p, int& counter) {
	int extra;
        for (int i = 1; i <= 4; i++) {
                extra = i==1 ? 8 : (i==2 ? 16 : (i==3 ? 7 : 9));
		extra = side ? extra : -extra;
                if (legalMove(piece[p].pos, piece[p].pos+extra)) {
                        piece[p].moveList[counter] = piece[p].pos + extra;
                        counter++;
                }
        }
}
