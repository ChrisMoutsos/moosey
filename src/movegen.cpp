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

	side = WHITE;
	for (int i = wqR; i <= wPh; i++)
		generateMoveListFor(i);
	side = BLACK;
	for (int i = bqR; i <= bPh; i++)
		generateMoveListFor(i);

	whiteMoveList.clear();
	blackMoveList.clear();
	for (int i = wqR; i <= bPh; i++)
		for (int j = 0; j < piece[i].getMoveListSize(); j++) 
			if (piece[i].getFromMoveList(j) != null) {
				mF = piece[i].getPos();
				mT = piece[i].getFromMoveList(j);
				if (piece[i].getColor()) 
					whiteMoveList.push_back(mF*100 + mT);
				else
					blackMoveList.push_back(mF*100 + mT);
			}

	side = realSide;	
}

void Board::cleanMoveList(bool s) {
	int mF, mT, size;
	bool realSide = side;
	int realMoveFrom = moveFrom;
	int realMoveTo = moveTo;

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
	moveFrom = realMoveFrom;
	moveTo = realMoveTo;
}

void Board::generateMoveListFor(int p) {
	int counter = 0;
	if (piece[p].getAlive()) {	
		if (piece[p].getValue() == R_VAL) 
			generateHozMoves(p, counter);
		else if (piece[p].getValue() == B_VAL)
			generateDiagMoves(p, counter);
		else if (piece[p].getValue() == N_VAL)
			generateKnightMoves(p, counter);
		else if (piece[p].getValue() == Q_VAL) { 
			generateHozMoves(p, counter);
			generateDiagMoves(p, counter);
		}
		else if (piece[p].getValue() == K_VAL)
			generateKingMoves(p, counter);
		else if (piece[p].getValue() == P_VAL)
			generatePawnMoves(p, counter);
	}
	for (int i = counter; i < piece[p].getMoveListSize(); i++) //Empty the rest of the moveList
		piece[p].setInMoveList(i, 0);
}

void Board::generateHozMoves(int p, int& counter) {
	bool s = piece[p].getColor();
	int i, d, posIndex;

	for (int c = 1; c <= 4; c++) {
		d = c==1 ? -1 : c==2 ? 1 : c==3 ? -10 : 10;
		i = 1;
		posIndex = piece[p].getPos()+d*i;
		while (board120[posIndex] != invalid) {
			if (validateMove(piece[p].getPos(), posIndex, s)) {
				piece[p].setInMoveList(counter, posIndex);
                	       	counter++;
			}
			i++;
			posIndex = piece[p].getPos()+d*i;
		}
	}
}


void Board::generateDiagMoves(int p, int& counter) {
	bool s = piece[p].getColor();
	int d, i, posIndex, pOMT;

	for (int c = 1; c <= 4; c++) {
		d = c==1 ? -11 : c==2 ? -9 : c==3 ? 11 : 9;
		i = 1;
		posIndex = piece[p].getPos()+d*i;
		pOMT = board120[posIndex];
		
		while (pOMT != invalid) { 
			if (validateMove(piece[p].getPos(), posIndex, s)) {
				piece[p].setInMoveList(counter, posIndex);
                        	counter++;
			}
			i++;
                        posIndex = piece[p].getPos()+d*i;
			pOMT = board120[posIndex];
                }
	}
}

void Board::generateKnightMoves(int p, int& counter) {
	bool s = piece[p].getColor();
	int extra, mT;
	for (int i = 1; i <= 8; i++) {
		extra = i==1 ? 8 : i==2 ? -8 : i==3 ? 12 : i==4 ? -12 : i==5 ? 19 : i==6 ? -19 : i==7 ? 21 : -21;
		mT = piece[p].getPos() + extra;
		if (validateMove(piece[p].getPos(), mT, s)) {
			piece[p].setInMoveList(counter, mT);
			counter++;	
		}
	}
}

void Board::generateKingMoves(int p, int& counter) {
	bool s = piece[p].getColor();
	int extra, mT;
        for (int i = 1; i <= 8; i++) {
                extra = i==1 ? 1 : i==2 ? -1 : i==3 ? 10 : i==4 ? -10 : i==5 ? 11 : i==6 ? -11 : i==7 ? 9 : -9;
		mT = piece[p].getPos() + extra;
                if (validateMove(piece[p].getPos(), mT, s)) {
                        piece[p].setInMoveList(counter, mT);
                        counter++;
                }
        }
	if (canCastle(KINGSIDE, s)) { 
		if (s)
			piece[p].setInMoveList(counter, _G1);
		else
			piece[p].setInMoveList(counter, _G8);
		counter++;
		setCastling(0);
	}
	if (canCastle(QUEENSIDE, s)) { 
		if (s)
			piece[p].setInMoveList(counter, _B1);
		else
			piece[p].setInMoveList(counter, _B8);
		counter++;
		setCastling(0);
	}

}

void Board::generatePawnMoves(int p, int& counter) {
	bool s = piece[p].getColor();
	int extra, mT;
        for (int i = 1; i <= 4; i++) {
                extra = i==1 ? 10 : i==2 ? 20 : i==3 ? 11 : 9;
		extra = side ? extra : -extra;
		mT = piece[p].getPos() + extra;
                if (validateMove(piece[p].getPos(), mT, s)) {
                        piece[p].setInMoveList(counter, mT);
                        counter++;
                }
        }
}
