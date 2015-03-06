/*
----------------------------------
	~Moosey Chess Engine~
	      eval.cpp
----------------------------------
*/

#include "board.h"
#include <iostream>

int Board::eval() {
	static int reverse[64] = { 56, 57, 58, 59, 60, 61, 62, 63,
				   48, 49, 50, 51, 52, 53, 54, 55,
				   40, 41, 42, 43, 44, 45, 46, 47,
				   32, 33, 34, 35, 36, 37, 38, 39,
				   24, 25, 26, 27, 28, 29, 30, 31,
				   16, 17, 18, 19, 20, 21, 22, 23,
				    8,  9, 10, 11, 12, 13, 14, 15,
				    0,  1,  2,  3,  4,  5,  6,  7 };

	static int pawnTable[64] = { 0,  0,  0,  0,  0,  0,  0,  0,
				     5,	10, 10,-20,-20, 10, 10,  5,
				     5, -5,-10,  0,  0,-10, -5,  5,
				     0,  0,  0, 20, 20,  0,  0,  0,
				     5,  5, 10, 25, 25, 10,  5,  5,
				    10, 10, 20, 30, 30, 20, 10, 10,
				    50, 50, 50, 50, 50, 50, 50, 50,
				     0,  0,  0,  0,  0,  0,  0,  0 };

	static int knightTable[64] = { -50,-40,-30,-30,-30,-30,-40,-50,
				       -40,-20,  0,  5,  5,  0,-20,-40,
				       -30,  5, 10, 15, 15, 15,  5,-30,
				       -30,  0, 15, 20, 20, 15,  0,-30,
				       -30,  5, 15, 20, 20, 15,  5,-30,
				       -30,  0, 10, 15, 15, 10,  0,-30,
				       -40,-20,  0,  0,  0,  0,-20,-40,
				       -50,-40,-30,-30,-30,-30,-40,-50 };

	static int bishopTable[64] = { -20,-10,-10,-10,-10,-10,-10,-20,
				       -10,  5,  0,  0,  0,  0,  5,-10,
				       -10, 10, 10, 10, 10, 10, 10,-10,
	                               -10,  0, 10, 10, 10, 10,  0,-10,
				       -10,  5,  5, 10, 10,  5,  5,-10,
				       -10,  0,  5, 10, 10,  5,  0,-10,
				       -10,  0,  0,  0,  0,  0,  0,-10,
				       -20,-10,-10,-10,-10,-10,-10,-20 };

	static int rookTable[64] = { 0,  0,  0,  5,  5,  0,  0,  0,
				    -5,  0,  0,  0,  0,  0,  0, -5,
				    -5,  0,  0,  0,  0,  0,  0, -5,
				    -5,  0,  0,  0,  0,  0,  0, -5,
				    -5,  0,  0,  0,  0,  0,  0, -5,
				    -5,  0,  0,  0,  0,  0,  0, -5,
				     5, 10, 10, 10, 10, 10, 10,  5,
				     0,  0,  0,  0,  0,  0,  0,  0 };
	
	static int queenTable[64] = { -20,-10,-10, -5, -5,-10,-10,-20,
				      -10,  0,  5,  0,  0,  0,  0,-10,
				      -10,  5,  5,  5,  5,  5,  0,-10,
				        0,  0,  5,  5,  5,  5,  0, -5,
				        0,  0,  5,  5,  5,  5,  0, -5,
				      -10,  0,  5,  5,  5,  5,  0,-10,
				      -10,  0,  5,  5,  5,  5,  0,-10,
				      -20,-10,-10, -5, -5,-10,-10,-20 };

	static int kingTable1[64] = { 20, 30, 10,  0,  0,  10, 30, 20,
			 	      20, 20,  0,  0,  0,  0,  20, 20,
				      -10,-20,-20,-20,-20,-20,-20,-10,
				      -20,-30,-30,-40,-40,-30,-30,-20,
				      -30,-40,-40,-50,-50,-40,-40,-30,
				      -30,-40,-40,-50,-50,-40,-40,-30,
				      -30,-40,-40,-50,-50,-40,-40,-30,
				      -30,-40,-40,-50,-50,-40,-40,-30 };

	static int kingTable2[64] = { -50,-30,-30,-30,-30,-30,-30,-50,
				      -30,-30,  0,  0,  0,  0,-30,-30,
				      -30,-10, 20, 30, 30, 20,-10,-30,
				      -30,-10, 30, 40, 40, 30,-10,-30,
				      -30,-10, 30, 40, 40, 30,-10,-30,
				      -30,-10, 20, 30, 30, 20,-10,-30,
				      -30,-30,  0,  0,  0,  0,-30,-30,
				      -50,-40,-30,-20,-20,-30,-40,-50 };

	int score = 0;

	//Piece-square table
	//---White pieces
	int wMat = 0, bMat = 0;
	for (int i = wqR; i <= wkR; i += wkR-wqR)
		if (piece[i].getAlive()) {
			score += rookTable[to64(piece[i].getPos())-1];
		}
	for (int i = wqN; i <= wkN; i += wkN-wqN)
		if (piece[i].getAlive()) {
			score += knightTable[to64(piece[i].getPos())-1];
		}
	for (int i = wqB; i <= wkB; i += wkB-wqB)
		if (piece[i].getAlive()) {
			score += bishopTable[to64(piece[i].getPos())-1];
		}
	if (piece[wQ].getAlive()) {
		score += queenTable[to64(piece[wQ].getPos())-1];
	}
	if (piece[wK].getAlive()) {
		for (int i = wqR; i <= wPh; i++)
			if (piece[i].getAlive())
				wMat += piece[i].getValue();
		for (int i = bqR; i <= bPh; i++)
			if (piece[i].getAlive())
				bMat += piece[i].getValue();
		
		if (!(wMat <= 1310 && bMat <= 1310))
			score += kingTable1[to64(piece[wK].getPos())-1];
		else
			score += kingTable2[to64(piece[wK].getPos())-1];
	}
	for (int i = wPa; i <= wPh; i++) {
		if (piece[i].getAlive()) {
			if (piece[i].getValue() == P_VAL)
				score += pawnTable[to64(piece[i].getPos())-1];
			else
				score += queenTable[to64(piece[i].getPos())-1];
		}
	}
	//---Black pieces
	for (int i = bqR; i <= bkR; i += bkR-bqR)
		if (piece[i].getAlive())
			score -= rookTable[reverse[to64(piece[i].getPos())-1]];
	for (int i = bqN; i <= bkN; i += bkN-bqN)
		if (piece[i].getAlive())
			score -= knightTable[reverse[to64(piece[i].getPos())-1]];
	for (int i = bqB; i <= bkB; i += bkB-bqB)
		if (piece[i].getAlive())
			score -= bishopTable[reverse[to64(piece[i].getPos())-1]];
	if (piece[bQ].getAlive())
		score -= queenTable[reverse[to64(piece[bQ].getPos())-1]];
	if (piece[bK].getAlive()) {
		if (!(wMat <= 1310 && bMat <= 1310))
			score -= kingTable1[reverse[to64(piece[bK].getPos())-1]];
		else
			score -= kingTable2[reverse[to64(piece[bK].getPos())-1]];
	}
	for (int i = bPa; i <= bPh; i++) {
		if (piece[i].getAlive()) {
			if (piece[i].getValue() == P_VAL)
				score -= pawnTable[reverse[to64(piece[i].getPos())-1]];
			else
				score -= queenTable[reverse[to64(piece[i].getPos())-1]];
		}
	}

	//Material
	for (int i = wqR; i <= wPh; i++) {
		if (piece[i].getAlive())
			score += piece[i].getValue();
	}
	for (int i = bqR; i <= bPh; i++) {
		if (piece[i].getAlive())
			score -= piece[i].getValue();
	}

	//Castled
	if (whiteCastled) score += 100;
	if (blackCastled) score -= 100;

	//Passed pawns
	int filesWithBlackPawns[8] = {0}, filesWithWhitePawns[8] = {0};
	for (int p = wPa; p <= wPh; p++) {
		if (!piece[i].getAlive()) continue;
		filesWithWhitePawns[piece[p].getPos()%10-1] = 1;
	}
	for (int p = bPa; p <= bPh; p++) {
		if (!piece[i].getAlive()) continue;
		filesWithBlackPawns[piece[p].getPos()%10-1] = 1;
	}
	for (int p = wPa; p <= wPh; p++) {
		if (!piece[i].getAlive()) continue;
		if (filesWithBlackPawns[piece[p].getPos()%10-1] == 0)
			score += 50;
	}
	for (int p = bPa; p <= bPh; p++) {
		if (!piece[i].getAlive()) continue;
		if (filesWithWhitePawns[piece[p].getPos()%10-1] == 0)
			score -= 50;
	}


	//Doubled pawns
	int doubled = 0, pos;
	for (int i = wPa; i <= wPh; i++) {
		if (!piece[i].getAlive()) continue;
		pos = piece[i].getPos();
		for (int j = wPa; j <= wPh; j++) {
			if (!piece[j].getAlive()) continue;
			if (j != i && piece[j].getPos()%10 == pos%10) 
					doubled++;
		}
	}
	score -= doubled*10;
	doubled = 0;
	for (int i = bPa; i <= bPh; i++) {
		if (!piece[i].getAlive()) continue;
		pos = piece[i].getPos();
		for (int j = bPa; j <= bPh; j++) {
			if (!piece[j].getAlive()) continue;
			if (j != i && piece[j].getPos()%10 == pos%10) 
					doubled++;
		}
	}
	score += doubled*10;

	//Open filed rooks
	for (int j = wqR; j <= bqR; j += (bqR-wqR)) {
		for (int i = j; i <= j+7; i += 7) {
			if (!piece[i].getAlive()) continue;
			bool open = true;
			int rank = piece[i].getPos()/10;
			int file = piece[i].getPos()%10;
			int checkPiece;
			for (int r = rank; r <= 9; r++) { //Up
				checkPiece = board120[r*10+file];
				if (checkPiece != empty) {
					if (piece[checkPiece].getColor() != piece[i].getColor())
						open = false;
					break;
				}
			}
			for (int r = rank; r >= 2; r--) { //Down
				checkPiece = board120[r*10+file];
				if (checkPiece != empty) {
					if (piece[checkPiece].getColor() != piece[i].getColor())
						open = false;
					break;
				}
			}
			if (open)
				score = (j == wqR) ? score+100 : score-100;
		}
	}

	return side ? score : -score;
}
