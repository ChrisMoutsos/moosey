/*
----------------------------------
	~Moosey Chess Engine~
	      movegen.cpp
----------------------------------
*/

#include <iostream>
#include <algorithm>
#include <SDL2/SDL.h>
#include "common.h"
#include "display.h"
#include "board.h"

int Board::perft(int depth) {
	int n_moves, i;
	int nodes = 0;
	int moveList[256] = { 0 };

	n_moves = getNonOrderedAllLegalMoves(side, moveList);

	if (depth == 1) return n_moves;
	
	for (i = 0; i < n_moves; i++) {
		movePiece(moveList[i]/100, moveList[i]%100);
		changeTurn();
		nodes += perft(depth-1);
		unmovePiece(moveList[i]/100, moveList[i]%100);
		changeTurn();
	}

	return nodes;
}

int Board::getNonOrderedAllLegalMoves(bool s, int* moveList) {
	int startP, endP, mF, mT, index = 0;
	startP = s ? wqR : bqR;
	endP = s ? wPh : bPh;

	for (int i = startP; i <= endP; i++) {
		if (!piece[i].getAlive()) continue;
		generatePieceMoveListFor(i);
		mF = piece[i].getPos();
		for (int j = 0; j < piece[i].getMoveListSize(); j++) {
			mT = piece[i].getFromMoveList(j);
			if (mT == 0) break;
			if (putSelfInCheck(mF, mT, s)) continue;
			moveList[index] = mF*100+mT;
			index++;
		}
	}

	moveList[index] = 0;

	return index;
}

int Board::genOrderedMoveList() {
	int* moveList = side ? whiteMoveList : blackMoveList;
	int size = genOrderedMoveList(side, moveList);
	if (side) numWhiteMoves = size;
	else numBlackMoves = size;

	return size;
}

int Board::genOrderedMoveList(bool s, int* moveList) {
	//Generates psuedo-legal moves for side s, stores it in moveList
	//Order: Captures, castling, non-captures

	int captures[256] = { 0 };
	int nonCaptures[256] = { 0 };
		
	getCaptures(s, captures);
	int nonCapturesSize = getNonCaptures(s, nonCaptures);

	sortCaptures(captures);
	sortNonCaptures(nonCaptures);

	if ((!whiteCastled && s) || (!blackCastled && s)) {
		for (int i = 0; i < nonCapturesSize; i++) {
			if (piece[board120[nonCaptures[i]/100]].getValue() == K_VAL) {
				if (abs(nonCaptures[i]/100 - nonCaptures[i]%100) == 2) {
					int move = nonCaptures[i];
					for (int j = 0; j < i; j++) {
						nonCaptures[j+1] = nonCaptures[j];
					}
					nonCaptures[0] = move;
				}
			}
		}
	}

	int i = 0;
	while (captures[i] != 0) {
		moveList[i] = captures[i];
		i++;
	}
	int j = 0;
	while (nonCaptures[j] != 0) {
		moveList[i+j] = nonCaptures[j];
		j++;
	}

	moveList[i+j] = 0;

	return i+j;
}

int Board::genNonOrderedMoveList(bool s, int* moveList) {
	int captures[256] = { 0 };
	int nonCaptures[256] = { 0 };
		
	getCaptures(s, captures);
	getNonCaptures(s, nonCaptures);

	int i = 0;
	while (captures[i] != 0) {
		moveList[i] = captures[i];
		i++;
	}
	int j = 0;
	while (nonCaptures[j] != 0) {
		moveList[i+j] = nonCaptures[j];
		j++;
	}

	moveList[i+j] = 0;

	return i+j;
}

int Board::getCaptures(bool s, int* moveList) {
	//Put every capture for side s in moveList

	int index = 0;
	int startP, endP, enemyStartP, enemyEndP;
	startP = s ? wqR : bqR;
	endP = s ? wPh : bPh;
	enemyStartP = s ? bqR : wqR;
	enemyEndP = s ? bPh : wPh;

	for (int i = startP; i <= endP; i++) {
		if (!piece[i].getAlive()) continue;
		for (int j = enemyStartP; j <= enemyEndP; j++) {
			if (!piece[j].getAlive()) continue;
			if (validateMove(piece[i].getPos(), piece[j].getPos(), s)) {
				moveList[index] = piece[i].getPos()*100 + piece[j].getPos();
				index++;
			}
		}
	}	

	moveList[index] = 0;

	return index;
}

void Board::sortCaptures(int* moveList) {
	bool sorted = false;
	int i = 0, temp;
	while (!sorted) {
		sorted = true;
		while (moveList[i+1] != 0) {
			if (!MVVLVA(moveList[i], moveList[i+1])) {
				temp = moveList[i];
				moveList[i] = moveList[i+1];
				moveList[i+1] = temp;
				sorted = false;
			}
			i++;
		}
	}
}

bool Board::MVVLVA(int i, int j) {
	return (piece[board120[i%100]].getValue() - piece[board120[i/100]].getValue()/10
		 >= piece[board120[j%100]].getValue() - piece[board120[j/100]].getValue()/10);
}

int Board::addPromotions(bool s, int* moveList) {
	int index = 0;
	while (moveList[index] != 0) {
		index++;
	}

	int startP, endP, extra;
	startP = s ? wqR : bqR;
	endP = s ? wPh : bPh;
	extra = s ? 10 : -10;
	
	for (int i = startP; i <= endP; i++) {
		if (!piece[i].getAlive()) continue;
		if (s && piece[i].getPos() < 80) continue;
		if (!s && piece[i].getPos() > 38) continue;
		for (int j = -1; j <= 1; j++) {
			if (validateMove(piece[i].getPos(), piece[i].getPos()+extra+j, s)) {
				moveList[index] = piece[i].getPos()*100+piece[i].getPos()+extra+j;
				index++;
			}
		}
	}

	moveList[index] = 0;

	return index;
}

int Board::getNonCaptures(bool s, int* moveList) {
	//Put every noncapture for side s in moveList

	int index = 0;
	int startP, endP, mF, mT;
	startP = s ? wqR : bqR;
	endP = s ? wPh : bPh;

	for (int i = startP; i <= endP; i++) {
		if (!piece[i].getAlive()) continue;
		generatePieceMoveListFor(i);
		mF = piece[i].getPos();
		for (int j = 0; j < piece[i].getMoveListSize(); j++) {
			mT = piece[i].getFromMoveList(j);
			if (mT == 0) break;
			if (board120[mT] != empty) continue;
			moveList[index] = mF*100+mT;
			index++;
		}
	}

	moveList[index] = 0;

	return index;
}

void Board::sortNonCaptures(int* moveList) {
	bool sorted = false, s;
	int i = 0, temp;
	while (!sorted) {
		sorted = true;
		if (moveList[0] != 0)
			s = piece[board120[moveList[0]/100]].getColor();
		while (moveList[i+1] != 0) {
			if (!hhSort(s, moveList[i], moveList[i+1])) {
				temp = moveList[i];
				moveList[i] = moveList[i+1];
				moveList[i+1] = temp;
				sorted = false;
			}
			i++;
		}
	}
}

bool Board::hhSort(bool s, int i, int j) {
	if (s)
		return (whiteBot.getFromHH(to64((i/100)-1), to64((i%100)-1)) 
			>= whiteBot.getFromHH(to64((j/100)-1), to64((j%100)-1)));
	else
		return (blackBot.getFromHH(to64((i/100)-1), to64((i%100)-1)) 
			>= blackBot.getFromHH(to64((j/100)-1), to64((j%100)-1)));
}

void Board::removeNonCaptures(bool s, int* moveList) {
	//Remove any move in moveList that is a noncapture
	int mT, move;
	int i = 0;
	while (moveList[i] != 0) {
		move = moveList[i];
		mT = move%100;
		if (board120[mT] == empty) {
			int j = i;
			while (moveList[j+1] != 0) {
				moveList[j] = moveList[j+1];
				j++;
			}
			moveList[j] = 0;
		}
		i++;
	}
}

int Board::cleanMoveList(bool s) {
	int* moveList = side ? whiteMoveList : blackMoveList;
	int size = cleanMoveList(s, moveList);
	return size;
}

int Board::cleanMoveList(bool s, int* moveList) {
	//Erases any illegal moves (for side s) in moveList

	int i = 0;
	int mF, mT;

	while (moveList[i] != 0) {
		mF = moveList[i]/100;
		mT = moveList[i]%100;
		if (putSelfInCheck(mF, mT, s)) {
			int j = i;
			while (moveList[j+1] != 0) {
				moveList[j] = moveList[j+1];
				j++;
			}
			moveList[j] = 0;
			i--;
		} 
		i++;
	}

	return i;
}

void Board::generatePieceMoveLists(bool s) {
	int startP, endP;
	startP = s ? wqR : bqR;
	endP = s ? wPh : bPh;
	for (int i = startP; i <= endP; i++)
		generatePieceMoveListFor(i);
}

void Board::generatePieceMoveListFor(int p) {
	//Generates moves for an individual piece, stores them in the pieces moveList

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
	
	if ((s && whiteCastled) || (!s && blackCastled)) return;

	if (canCastle(KINGSIDE, s)) { 
		if (s)
			piece[p].setInMoveList(counter, G1);
		else
			piece[p].setInMoveList(counter, G8);
		counter++;
	}
	if (canCastle(QUEENSIDE, s)) { 
		if (s)
			piece[p].setInMoveList(counter, C1);
		else
			piece[p].setInMoveList(counter, C8);
		counter++;
	}

}

void Board::generatePawnMoves(int p, int& counter) {
	bool s = piece[p].getColor();
	int extra, mT;
        for (int i = 1; i <= 4; i++) {
                extra = i==1 ? 10 : i==2 ? 20 : i==3 ? 11 : 9;
		extra = s ? extra : -extra;
		mT = piece[p].getPos() + extra;
                if (validateMove(piece[p].getPos(), mT, s)) {
                        piece[p].setInMoveList(counter, mT);
                        counter++;
                }
        }
}
