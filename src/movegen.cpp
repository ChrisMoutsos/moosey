/*
----------------------------------
	~Moosey Chess Engine~
	      movegen.cpp
----------------------------------
*/

#include <iostream>
#include "common.h"
#include "board.h"

void Board::genOrderedMoveList() {
	if (side)
		genOrderedMoveList(side, whiteMoveList);
	else
		genOrderedMoveList(side, blackMoveList);
}

void Board::genOrderedMoveList(bool s, std::vector<int>& moveList) {
	/* Generates psuedo-legal moves for side s, stores it in moveList */
	/* Captures, castling, non-captures */

	moveList.clear();
	std::vector<int> captures, nonCaptures;
	int mF, mT, move;
		
	generatePieceMoveLists(s);
	getGoodCaptures(s, captures);
	getNonCaptures(s, nonCaptures);

	for (int i = 0; i < (int)nonCaptures.size(); i++) {
		move = nonCaptures[i];
		mF = move/100;
		mT = move%100;
		if (piece[board120[mF]].getValue() == K_VAL) {
			if (abs(mF-mT) == 2) {
				nonCaptures.erase(nonCaptures.begin()+i);
				nonCaptures.insert(nonCaptures.begin(), move);
			}
		}
	}

	moveList.reserve(captures.size() + nonCaptures.size());
	moveList.insert(moveList.begin(), captures.begin(), captures.end());
	moveList.insert(moveList.end(), nonCaptures.begin(), nonCaptures.end());
}


void Board::removeNonCaptures(bool s, std::vector<int>& moveList) {
	/* Remove any move in moveList that is a noncapture */
	int mT, move;
	for (int i = 0; i < (int)moveList.size(); i++) {
		move = moveList[i];
		mT = move%100;
		if (board120[mT] == empty) {
			moveList.erase(moveList.begin()+i);
			i--;
		}
	}
}

void Board::getNonCaptures(bool s, std::vector<int>& moveList) {
	/* Put every noncapture for side s in moveList */

	moveList.clear();
	int startP, endP, mF, mT;
	startP = s ? wqR : bqR;
	endP = s ? wPh : bPh;

	for (int i = startP; i <= endP; i++) {
		if (!piece[i].getAlive()) continue;
		mF = piece[i].getPos();
		for (int j = 0; j < piece[i].getMoveListSize(); j++) {
			mT = piece[i].getFromMoveList(j);
			if (mT == 0) break;
			if (board120[mT] != empty) continue;
			moveList.push_back(mF*100+mT);
		}
	}
}

void Board::getGoodCaptures(bool s, std::vector<int>& moveList) {
	/* Put every capture for side s in moveList, ordered */

	moveList.clear();
	int startP, endP, mF, mT, mF2, mT2, move;
	startP = s ? wqR : bqR;
	endP = s ? wPh : bPh;

	for (int i = startP; i <= endP; i++) {
		if (!piece[i].getAlive()) continue;
		mF = piece[i].getPos();
		for (int j = 0; j < piece[i].getMoveListSize(); j++) {
			mT = piece[i].getFromMoveList(j);
			if (mT == 0) break;
			if (board120[mT] == empty) continue;
			moveList.push_back(mF*100+mT);
		}
	}

	//Bubble sort captures	
	int temp, j = 0, diff, diff2;
	bool changed = true;
	while (changed) {
		changed = false;
		j++;
		for (int i = 0; i < (int)moveList.size() - j; i++) {
			move = moveList[i];
			mF = move/100;
			mT = move%100;
			mF2 = moveList[i+1]/100;
			mT2 = moveList[i+1]%100;
			diff = piece[board120[mT]].getValue() - piece[board120[mF]].getValue()/100;
			diff2 = piece[board120[mT2]].getValue() - piece[board120[mF2]].getValue()/100;
			
			//Sort from highest disparity between attacker/victim to lowest
			if (diff < diff2) {
				temp = moveList[i+1];
				moveList[i+1] = move;
				moveList[i] = temp;
				changed = true;
			}
			
		}
	}
}

/*
void Board::generateMoveLists() {	
	generateMoveListFor(WHITE, whiteMoveList);
	generateMoveListFor(BLACK, blackMoveList);
}

void Board::generateMoveListFor(bool s, std::vector<int>& moveList) {	
//	 Generates psuedo-legal moves for side s, stores it in moveList 

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
*/
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

void Board::generatePieceMoveLists(bool s) {
	int startP, endP;
	startP = s ? wqR : bqR;
	endP = s ? wPh : bPh;
	for (int i = startP; i <= endP; i++)
		generatePieceMoveListFor(i);
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
