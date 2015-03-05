/*
----------------------------------
	~Moosey Chess Engine~
	      movegen.cpp
----------------------------------
*/

#include <iostream>
#include "board.h"

void Board::orderMoveList(bool s, std::vector<int>& moveList) {
	//int captures = 0;
	std::vector<int> captures;
	std::vector<int> castlings;
	int mF, mT, move;
	for (int i = 0; i < (int)moveList.size(); i++) {
		move = moveList[i];
		mF = move/100;
		mT = move%100;
		if (board120[mT] != empty && piece[board120[mT]].getColor() != s) { //Captures
			captures.push_back(move);
			moveList.erase(moveList.begin()+i);
		}
		else if (piece[board120[mF]].getValue() == K_VAL) {
			if (abs(mF-mT) == 2) {
				castlings.push_back(move);
				moveList.erase(moveList.begin()+i);
			}
		}
	}
	generateGoodCaptures(s, captures);
	moveList.reserve(captures.size() + moveList.size() + castlings.size());
	moveList.insert(moveList.begin(), captures.begin(), captures.end());
	moveList.insert(moveList.begin(), castlings.begin(), castlings.end());
	
}

void Board::generateGoodCaptures(bool s, std::vector<int>& moveList) {
	//Most valuable victim / least valuable attacker

	//Clean all but captures
	int mF, mT, move;
	for (int i = 0; i < (int)moveList.size(); i++) {
		move = moveList[i];
		mF = move/100;
		mT = move%100;
		if (board120[mT] != empty && piece[board120[mT]].getColor() != s) { //Captures
			
		}
		else {
			moveList.erase(moveList.begin()+i);
			i--;
		}
	}

	//Bubble sort the rest	
	int temp, j = 0, diff, diff2;
	bool changed = true;
	while (changed) {
		changed = false;
		j++;
		for (int i = 0; i < (int)moveList.size() - j; i++) {
			move = moveList[i];
			mF = move/100;
			mT = move%100;
			diff = -piece[board120[mT]].getValue() 
				+ piece[board120[mF]].getValue();
			diff2 = -piece[board120[moveList[i+1]%100]].getValue() 
				+ piece[board120[moveList[i+1]%100]].getValue();
			//Sort from highest disparity between attacker/victim to lowest
			if (diff > diff2) {
				temp = moveList[i+1];
				moveList[i+1] = move;
				moveList[i] = temp;
				changed = true;
			}
			
		}
	}
}

void Board::generateMoveLists() {	
	generateMoveListFor(WHITE, whiteMoveList);
	generateMoveListFor(BLACK, blackMoveList);
}

void Board::generateMoveListFor(bool s, std::vector<int>& moveList) {	
	/* Generates psuedo-legal moves for side s, stores it in moveList */

	int mF, mT;
	bool realSide = side;

	side = s;
	for (int p = bqR-(s*bqR); p <= bPh-(s*bqR); p++)
		generatePieceMoveListFor(p);

	moveList.clear();
	for (int p = bqR-(s*bqR); p <= bPh-(s*bqR); p++)
		for (int j = 0; j < piece[p].getMoveListSize(); j++) 
			if (piece[p].getFromMoveList(j) != null) {
				mF = piece[p].getPos();
				mT = piece[p].getFromMoveList(j);
				moveList.push_back(mF*100 + mT);
			}

	side = realSide;	
}

void Board::cleanMoveList(bool s) {
	if (s)
		cleanMoveList(s, whiteMoveList);
	else
		cleanMoveList(s, blackMoveList);
}

void Board::cleanMoveList(bool s, std::vector<int>& moveList) {
	/* Erases any illegal moves (for side s) in moveList*/

	int mF, mT, size;
	bool realSide = side;
	int realMoveFrom = moveFrom;
	int realMoveTo = moveTo;

	size = moveList.size();
	for (int i = 0; i < size; i++) {
		mF = moveList[i]/100;
		mT = moveList[i]%100;
		if (!legalMove(mF, mT, s)) {
			moveList.erase(moveList.begin()+i);
			size--;
			i--;
		} 
	}
	side = realSide;	
	moveFrom = realMoveFrom;
	moveTo = realMoveTo;
}

void Board::generatePieceMoveListFor(int p) {
	/* Generates moves for an individual piece, stores them in the pieces moveList*/

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
	}
	if (canCastle(QUEENSIDE, s)) { 
		if (s)
			piece[p].setInMoveList(counter, _C1);
		else
			piece[p].setInMoveList(counter, _C8);
		counter++;
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
