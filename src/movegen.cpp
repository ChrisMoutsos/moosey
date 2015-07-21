/*
----------------------------------
	~Moosey Chess Engine~
	      movegen.cpp
----------------------------------
*/

#include <iostream>
#include <algorithm>
#include "common.h"
#include "display.h"
#include "board.h"

int Board::perft(int depth) {
	int n_moves, i;
	int nodes = 0;
	std::vector<int> moveList;

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

int Board::getNonOrderedAllLegalMoves(bool s, std::vector<int>& moveList) {
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
			if (putSelfInCheck(mF, mT, s)) continue;
			moveList.push_back(mF*100+mT);
		}
	}

	return int(moveList.size());
}

void Board::genOrderedMoveList() {
	if (side == WHITE)
		genOrderedMoveList(side, whiteMoveList);
	else
		genOrderedMoveList(side, blackMoveList);
}

void Board::genOrderedMoveList(bool s, std::vector<int>& moveList) {
	//Generates psuedo-legal moves for side s, stores it in moveList
	//Order: Captures, castling, non-captures

	moveList.clear();
	std::vector<int> captures, nonCaptures;

	generatePieceMoveLists(s);
		
	getCaptures(s, captures);
	getNonCaptures(s, nonCaptures);

	sortCaptures(captures);
	sortNonCaptures(nonCaptures);

	if ((!whiteCastled && s) || (!blackCastled && s)) {
		for (size_t i = 0; i < nonCaptures.size(); i++) {
			if (piece[board120[nonCaptures[i]/100]].getValue() == K_VAL) {
				if (abs(nonCaptures[i]/100 - nonCaptures[i]%100) == 2) {
					int move = nonCaptures[i];
					nonCaptures.erase(nonCaptures.begin()+i);
					nonCaptures.insert(nonCaptures.begin(), move);
				}
			}
		}
	}

	moveList.reserve(captures.size() + nonCaptures.size());
	moveList.insert(moveList.begin(), captures.begin(), captures.end());
	moveList.insert(moveList.end(), nonCaptures.begin(), nonCaptures.end());
}

void Board::getCaptures(bool s, std::vector<int>& moveList) {
	//Put every capture for side s in moveList

	moveList.clear();

	int mF, mT, startP, endP;
	startP = s ? wqR : bqR;
	endP = s ? wPh : bPh;

	for (int i = startP; i <= endP; i++) {
		if (!piece[i].getAlive()) continue;
		//generatePieceMoveListFor(i);
		mF = piece[i].getPos();
		for (int j = 0; j < piece[i].getMoveListSize(); j++) {
			mT = piece[i].getFromMoveList(j);
			if (mT == 0) break;
			if (board120[mT] == empty) continue;
			moveList.push_back(mF * 100 + mT);
		}
	}
}

void Board::generateCaptures(bool s, std::vector<int>& moveList) {
	generateCapturePieceMoveLists(s);

	int mF, mT, startP, endP;
	startP = s ? wqR : bqR;
	endP = s ? wPh : bPh;

	for (int i = startP; i <= endP; i++) {
		if (!piece[i].getAlive()) continue;
		mF = piece[i].getPos();
		for (int j = 0; j < piece[i].getMoveListSize(); j++) {
			mT = piece[i].getFromMoveList(j);
			if (mT == 0) break;
			if (board120[mT] == empty) continue;
			moveList.push_back(mF * 100 + mT);
		}
	}
}

void Board::sortCaptures(std::vector<int>& moveList) {
	struct MVVLVA {
		MVVLVA(Board* b) : b(b) {}
		bool operator()(int i, int j) {
			return (b->piece[b->board120[i % 100]].getValue() - b->piece[b->board120[i / 100]].getValue() / 10
				> b->piece[b->board120[j % 100]].getValue() - b->piece[b->board120[j / 100]].getValue() / 10);
		};
	private:
		Board* b;
	} MVVLVAsorter(this);


	std::sort(moveList.begin(), moveList.end(), MVVLVAsorter);
}

void Board::addPromotions(bool s, std::vector<int>& moveList) {
	int startP, endP, extra;
	startP = s ? wqR : bqR;
	endP = s ? wPh : bPh;
	extra = s ? 10 : -10;
	
	for (int i = startP; i <= endP; i++) {
		if (!piece[i].getAlive()) continue;
		if (s && piece[i].getPos() < 80) continue;
		if (!s && piece[i].getPos() > 38) continue;
		for (int j = -1; j <= 1; j++)
			if (validateMove(piece[i].getPos(), piece[i].getPos()+extra+j, s))
				moveList.insert(moveList.begin()+0,
						piece[i].getPos()*100+piece[i].getPos()+extra+j);
	}
}

void Board::getNonCaptures(bool s, std::vector<int>& moveList) {
	//Put every noncapture for side s in moveList

	moveList.clear();

	int mF, mT, startP, endP;
	startP = s ? wqR : bqR;
	endP = s ? wPh : bPh;

	for (int i = startP; i <= endP; i++) {
		if (!piece[i].getAlive()) continue;
		mF = piece[i].getPos();
		for (int j = 0; j < piece[i].getMoveListSize(); j++) {
			mT = piece[i].getFromMoveList(j);
			if (mT == 0) break;
			if (board120[mT] != empty) continue;
			moveList.push_back(mF * 100 + mT);
		}
	}
}

void Board::sortNonCaptures(std::vector<int>& moveList) {
	struct hhSort {
		hhSort(Board* b) : b(b) {}
		bool operator() (int i, int j) {
			//std::cout << "i: " << i << " j: " << j << '\n';
			if (b->getSide())
				return (b->whiteBot.getFromHH(to64(i / 100) - 1, to64(i % 100) - 1)
				> b->whiteBot.getFromHH(to64(j / 100) - 1, to64(j % 100) - 1));
			else
				return (b->blackBot.getFromHH(to64(i / 100) - 1, to64(i % 100) - 1)
				> b->blackBot.getFromHH(to64(j / 100) - 1, to64(j % 100) - 1));
		};
	private:
		Board* b;
	} hhSorter(this);


	std::sort(moveList.begin(), moveList.end(), hhSorter);
}

void Board::removeNonCaptures(bool s, std::vector<int>& moveList) {
	//Remove any move in moveList that is a noncapture
	int mT, move;
	for (size_t i = 0; i < moveList.size(); i++) {
		move = moveList[i];
		mT = move%100;
		if (board120[mT] == empty) {
			moveList.erase(moveList.begin()+i);
			i--;
		}
	}
}

void Board::cleanMoveList(bool s) {
	if (s)
		cleanMoveList(s, whiteMoveList);
	else
		cleanMoveList(s, blackMoveList);
}

void Board::cleanMoveList(bool s, std::vector<int>& moveList) {
	//Erases any illegal moves (for side s) in moveList

	int mF, mT, size;

	size = int(moveList.size());
	for (int i = 0; i < size; i++) {
		mF = moveList[i]/100;
		mT = moveList[i]%100;
		if (!legalMove(mF, mT, s)) {
			moveList.erase(moveList.begin()+i);
			size--;
			i--;
		} 
	}
}

void Board::generateCapturePieceMoveLists(bool s) {
	int startP, endP;
	startP = s ? wqR : bqR;
	endP = s ? wPh : bPh;
	for (int i = startP; i <= endP; i++)
		generateCapturePieceMoveListFor(i);
}

void Board::generateCapturePieceMoveListFor(int p) {
	//Generates moves for an individual piece, stores them in the pieces moveList

	int counter = 0;
	if (piece[p].getAlive()) {
		if (piece[p].getValue() == R_VAL)
			generateHozCaptures(p, counter);
		else if (piece[p].getValue() == B_VAL)
			generateDiagCaptures(p, counter);
		else if (piece[p].getValue() == N_VAL)
			generateKnightCaptures(p, counter);
		else if (piece[p].getValue() == Q_VAL) {
			generateHozCaptures(p, counter);
			generateDiagCaptures(p, counter);
		}
		else if (piece[p].getValue() == K_VAL)
			generateKingCaptures(p, counter);
		else if (piece[p].getValue() == P_VAL)
			generatePawnCaptures(p, counter);
	}
	for (int i = counter; i < piece[p].getMoveListSize(); i++) //Empty the rest of the moveList
		piece[p].setInMoveList(i, 0);
}

void Board::generateHozCaptures(int p, int& counter) {
	bool s = piece[p].getColor();
	int pos63 = to64(piece[p].getPos()) - 1;

	U64 attackBB, blockBB;
	int blockSq;

	for (int dir = NORTH; dir <= WEST; dir += 2) {
		attackBB = bb.rayAttacks[dir][pos63];
		blockBB = attackBB & (bb.allPieces[WHITE] | bb.allPieces[BLACK]);

		//If no blocker, change direction
		if (!blockBB) continue;

		//If there was a blocker, find out who it was
		if (dir == WEST || dir == SOUTH) {
			blockSq = bb.bitScanReverse(blockBB);
		}
		else {
			blockSq = bb.bitScanForward(blockBB);
		}

		//If the blocker is your own piece, change direction
		if (bb.allPieces[s] & bb.sq[blockSq]) {
			continue;
		}
		//Else, it's a catpure, add to the movelist
		piece[p].setInMoveList(counter, from64(blockSq + 1));
		counter++;
	}
}

void Board::generateDiagCaptures(int p, int& counter) {
	bool s = piece[p].getColor();
	int pos63 = to64(piece[p].getPos()) - 1;

	U64 attackBB, blockBB;
	int blockSq;

	for (int dir = NORTHEAST; dir <= NORTHWEST; dir += 2) {
		attackBB = bb.rayAttacks[dir][pos63];
		blockBB = attackBB & (bb.allPieces[WHITE] | bb.allPieces[BLACK]);

		//If there's no blocker, change direction
		if (!blockBB) continue;

		//If there was a blocker, find out who it was
		if (dir == SOUTHWEST || dir == SOUTHEAST) {
			blockSq = bb.bitScanReverse(blockBB);
		}
		else {
			blockSq = bb.bitScanForward(blockBB);
		}

		//If the blocker is your own piece, change direction
		if (bb.allPieces[s] & bb.sq[blockSq]) {
			continue;
		}
		//Else, it's a catpure, add to the movelist
		piece[p].setInMoveList(counter, from64(blockSq + 1));
		counter++;
	}
}

void Board::generateKnightCaptures(int p, int& counter) {
	bool s = piece[p].getColor();
	int pos63 = to64(piece[p].getPos()) - 1;

	U64 attackBB;
	int extra;
	attackBB = bb.knightAttacks[pos63] & bb.allPieces[!s];
	for (int i = 1; i <= 8; i++) {
		if (!attackBB) break;
		extra = i == 1 ? 17 : i == 2 ? 10 : i == 3 ? -6 : i == 4 ? -15 : i == 5 ? -17 : i == 6 ? -10 : i == 7 ? 6 : 15;
		if (!(pos63 + extra < 64 && pos63 + extra >= 0))
			continue;
		if (bb.queryBit(pos63 + extra, attackBB)) {
			piece[p].setInMoveList(counter, from64(pos63 + extra + 1));
			counter++;
			bb.unsetBit(pos63 + extra, attackBB);
		}
	}
}

void Board::generateKingCaptures(int p, int& counter) {
	bool s = piece[p].getColor();
	int pos63 = to64(piece[p].getPos()) - 1;

	int extra;
	U64 attackBB;
	attackBB = bb.kingAttacks[pos63] & bb.allPieces[!s];
	for (int i = 1; i <= 8; i++) {
		if (!attackBB) break;
		extra = i == 1 ? 1 : i == 2 ? -1 : i == 3 ? 8 : i == 4 ? -8 : i == 5 ? 9 : i == 6 ? -9 : i == 7 ? 7 : -7;
		if (!(pos63 + extra < 64 && pos63 + extra >= 0))
			continue;
		if (bb.queryBit(pos63 + extra, attackBB)) {
			piece[p].setInMoveList(counter, from64(pos63 + extra + 1));
			counter++;
			bb.unsetBit(pos63 + extra, attackBB);
		}
	}
}

void Board::generatePawnCaptures(int p, int& counter) {
	bool s = piece[p].getColor();
	int pos63 = to64(piece[p].getPos()) - 1;

	//Get attacking moves
	U64 attackBB = bb.pawnAttacks[s][pos63];
	U64 enemyBB = bb.allPieces[!s];

	if (moveInfo.size() && moveInfo.back().epSq != 0) {
		enemyBB |= bb.sq[to64(moveInfo.back().epSq) - 1];
	}
	attackBB &= enemyBB;

	int attSq;

	while (attackBB) {
		attSq = bb.bitScanForward(attackBB);
		piece[p].setInMoveList(counter, from64(attSq + 1));
		counter++;
		bb.unsetBit(attSq, attackBB);
	}
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
	int pos63 = to64(piece[p].getPos()) - 1;

	U64 attackBB, blockBB;
	int blockSq, sq, dirIncr;
	for (int dir = NORTH; dir <= WEST; dir += 2) {
		switch (dir) {
		case NORTH:
			dirIncr = 8;
			break;
		case EAST:
			dirIncr = 1;
			break;
		case SOUTH:
			dirIncr = -8;
			break;
		case WEST:
			dirIncr = -1;
			break;
		}
		attackBB = bb.rayAttacks[dir][pos63];
		blockBB = attackBB & (bb.allPieces[WHITE] | bb.allPieces[BLACK]);
		//If there was a blocker, cut off the rest
		if (blockBB) {
			if (dir == WEST || dir == SOUTH) {
				blockSq = bb.bitScanReverse(blockBB);
			}
			else {
				blockSq = bb.bitScanForward(blockBB);
			}
			attackBB ^= bb.rayAttacks[dir][blockSq];
			//If the blocker is your own piece, unset it
			if (bb.allPieces[s] & bb.sq[blockSq]) {
				bb.unsetBit(blockSq, attackBB);
			}
		}
		//Add each move to the movelist
		sq = pos63;
		while (attackBB) {
			//if (bb.queryBit(sq, attackBB)) {
			if (bb.queryBit(sq, attackBB)) {
				bb.unsetBit(sq, attackBB);
				piece[p].setInMoveList(counter, from64(sq+1));
				counter++;
			}
			sq += dirIncr;
		}
	}
}

void Board::generateDiagMoves(int p, int& counter) {
	bool s = piece[p].getColor();
	int pos63 = to64(piece[p].getPos()) - 1;

	U64 attackBB, blockBB;
	int blockSq, sq, dirIncr;
	for (int dir = NORTHEAST; dir <= NORTHWEST; dir += 2) {
		switch (dir) {
		case NORTHEAST:
			dirIncr = 9;
			break;
		case NORTHWEST:
			dirIncr = 7;
			break;
		case SOUTHEAST:
			dirIncr = -7;
			break;
		case SOUTHWEST:
			dirIncr = -9;
			break;
		}
		attackBB = bb.rayAttacks[dir][pos63];
		blockBB = attackBB & (bb.allPieces[WHITE] | bb.allPieces[BLACK]);
		//If there was a blocker, cut off the rest
		if (blockBB) {
			if (dir == SOUTHWEST || dir == SOUTHEAST) {
				blockSq = bb.bitScanReverse(blockBB);
			}
			else {
				blockSq = bb.bitScanForward(blockBB);
			}
			attackBB ^= bb.rayAttacks[dir][blockSq];
			//If the blocker is your own piece, unset it
			if (bb.allPieces[s] & bb.sq[blockSq]) {
				bb.unsetBit(blockSq, attackBB);
			}
		}
		//Add each move to the movelist
		sq = pos63;
		while (attackBB) {
			//if (bb.queryBit(sq, attackBB)) {
			if (bb.queryBit(sq, attackBB)) {
				bb.unsetBit(sq, attackBB);
				piece[p].setInMoveList(counter, from64(sq + 1));
				counter++;
			}
					sq += dirIncr;
		}
	}
	
}

void Board::generateKnightMoves(int p, int& counter) {
	bool s = piece[p].getColor();
	int pos63 = to64(piece[p].getPos()) - 1;
	
	U64 attackBB;
	int extra;
	attackBB = bb.knightAttacks[pos63] & ~bb.allPieces[s];
	for (int i = 1; i <= 8; i++) {
		if (!attackBB) break;
		extra = i == 1 ? 17 : i == 2 ? 10 : i == 3 ? -6 : i == 4 ? -15 : i == 5 ? -17 : i == 6 ? -10 : i == 7 ? 6 : 15;
		if (!(pos63 + extra < 64 && pos63 + extra >= 0)) 
			continue;
		if (bb.queryBit(pos63 + extra, attackBB)) {
			piece[p].setInMoveList(counter, from64(pos63 + extra + 1));
			counter++;
			bb.unsetBit(pos63 + extra, attackBB);
		}
	}
}

void Board::generateKingMoves(int p, int& counter) {
	bool s = piece[p].getColor();
	int pos63 = to64(piece[p].getPos()) - 1;

	int extra;
	U64 attackBB;
	attackBB = bb.kingAttacks[pos63] & ~bb.allPieces[s];
	for (int i = 1; i <= 8; i++) {
		if (!attackBB) break;
		extra = i == 1 ? 1 : i == 2 ? -1 : i == 3 ? 8 : i == 4 ? -8 : i == 5 ? 9 : i == 6 ? -9 : i == 7 ? 7 : -7;
		if (!(pos63 + extra < 64 && pos63 + extra >= 0)) 
			continue;
		if (bb.queryBit(pos63 + extra, attackBB)) {
			piece[p].setInMoveList(counter, from64(pos63 + extra + 1));
			counter++;
			bb.unsetBit(pos63 + extra, attackBB);
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
	int pos63 = to64(piece[p].getPos()) - 1;
	
	//Get attacking moves
	U64 attackBB = bb.pawnAttacks[s][pos63];
	U64 enemyBB = bb.allPieces[!s];

	if (moveInfo.size() && moveInfo.back().epSq != 0) {
		enemyBB |= bb.sq[to64(moveInfo.back().epSq) - 1];
	}
	attackBB &= enemyBB;

	int attSq;
	
	while (attackBB) {
		attSq = bb.bitScanForward(attackBB);
		piece[p].setInMoveList(counter, from64(attSq + 1));
		counter++;
		bb.unsetBit(attSq, attackBB);
	}


	//Get forward moves
	U64 allPieces = bb.allPieces[s] | bb.allPieces[!s];
	int step = s ? 8 : -8;
	if (pos63 + step < 64 && pos63 + step >= 0) {
		if (bb.sq[pos63 + step] & ~(allPieces)) {
			piece[p].setInMoveList(counter, from64(pos63 + step + 1));
			counter++;
			if (piece[p].getMoved() == 0 && 
				pos63 + step * 2 < 64 && pos63 + step * 2 >= 0) {
				if (bb.sq[pos63 + step * 2] & ~(allPieces)) {
					piece[p].setInMoveList(counter, from64(pos63 + step * 2 + 1));
					counter++;
				}
			}
		}
	}
}
