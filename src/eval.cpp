/*
----------------------------------
	~Moosey Chess Engine~
	      eval.cpp
----------------------------------
*/

#include "board.h"
#include <iostream>

int Board::eval() {
	//FIX INDEXES
	static int pawnTable[64] = { 0,  0,  0,  0,  0,  0,  0,  0,
				    50, 50, 50, 50, 50, 50, 50, 50,
				    10, 10, 20, 30, 30, 20, 10, 10,
				     5,  5, 10, 25, 25, 10,  5,  5,
				     0,  0,  0, 20, 20,  0,  0,  0,
				     5, -5,-10,  0,  0,-10, -5,  5,
				     5, 10, 10,-20,-20, 10, 10,  5,
				     0,  0,  0,  0,  0,  0,  0,  0 };

	static int knightTable[64] = { -50,-40,-30,-30,-30,-30,-40,-50,
				       -40,-20,  0,  0,  0,  0,-20,-40,
				       -30,  0, 10, 15, 15, 10,  0,-30,
				       -30,  5, 15, 20, 20, 15,  5,-30,
				       -30,  0, 15, 20, 20, 15,  0,-30,
				       -30,  5, 10, 15, 15, 15,  5,-30,
				       -40,-20,  0,  5,  5,  0,-20,-40,
			               -50,-40,-30,-30,-30,-30,-40,-50 }; 

	static int bishopTable[64] = { -20,-10,-10,-10,-10,-10,-10,-20,
				       -10,  0,  0,  0,  0,  0,  0,-10,
				       -10,  0,  5, 10, 10,  5,  0,-10,
				       -10,  5,  5, 10, 10,  5,  5,-10,
	                               -10,  0, 10, 10, 10, 10,  0,-10,
				       -10, 10, 10, 10, 10, 10, 10,-10,
				       -10,  5,  0,  0,  0,  0,  5,-10,
				       -20,-10,-10,-10,-10,-10,-10,-20 };

	static int rookTable[64] = { 0,  0,  0,  0,  0,  0,  0,  0,
				     5, 10, 10, 10, 10, 10, 10,  5,
				    -5,  0,  0,  0,  0,  0,  0, -5,
				    -5,  0,  0,  0,  0,  0,  0, -5,
				    -5,  0,  0,  0,  0,  0,  0, -5,
				    -5,  0,  0,  0,  0,  0,  0, -5,
				    -5,  0,  0,  0,  0,  0,  0, -5,
	  			     0,  0,  0,  5,  5,  0,  0,  0 };

	static int queenTable[64] = { -20,-10,-10, -5, -5,-10,-10,-20,
				      -10,  0,  0,  0,  0,  0,  0,-10,
				      -10,  0,  5,  5,  5,  5,  0,-10,
				       -5,  0,  5,  5,  5,  5,  0, -5,
				        0,  0,  5,  5,  5,  5,  0, -5,
				      -10,  5,  5,  5,  5,  5,  0,-10,
				      -10,  0,  5,  0,  0,  0,  0,-10,
				      -20,-10,-10, -5, -5,-10,-10,-20 };

	static int kingTable1[64] = { -30,-40,-40,-50,-50,-40,-40,-30,
				      -30,-40,-40,-50,-50,-40,-40,-30,
				      -30,-40,-40,-50,-50,-40,-40,-30,
				      -30,-40,-40,-50,-50,-40,-40,-30,
				      -20,-30,-30,-40,-40,-30,-30,-20,
	 			      -10,-20,-20,-20,-20,-20,-20,-10,
				       20, 20,  0,  0,  0,  0, 20, 20,
				       20, 30, 10,  0,  0, 10, 30, 20 };

	static int kingTable2[64] = { -50,-40,-30,-20,-20,-30,-40,-50,
				      -30,-20,-10,  0,  0,-10,-20,-30,
				      -30,-10, 20, 30, 30, 20,-10,-30,
				      -30,-10, 30, 40, 40, 30,-10,-30,
				      -30,-10, 30, 40, 40, 30,-10,-30,
				      -30,-10, 20, 30, 30, 20,-10,-30,
				      -30,-30,  0,  0,  0,  0,-30,-30,
				      -50,-30,-30,-30,-30,-30,-30,-50 };

	int score = 0;

	//Piece-square table
	//---White pieces
	int index, posi, wMat = 0, bMat = 0;
	for (int i = wqR; i <= wkR; i += wkR-wqR)
		if (piece[i].getAlive()) {
			posi = to64(piece[i].getPos())-1;
			index = posi+56 - posi/8*16;
			score += rookTable[index];
		}
	for (int i = wqN; i <= wkN; i += wkN-wqN)
		if (piece[i].getAlive()) {
			posi = to64(piece[i].getPos())-1;
			index = posi+56 - posi/8*16;
			score += knightTable[index];
		}
	for (int i = wqB; i <= wkB; i += wkB-wqB)
		if (piece[i].getAlive()) {
			posi = to64(piece[i].getPos())-1;
			index = posi+56 - posi/8*16;
			score += bishopTable[index];
		}
	if (piece[wQ].getAlive()) {
		posi = to64(piece[wQ].getPos())-1;
		index = posi+56 - posi/8*16;
		score += queenTable[index];
	}
	if (piece[wK].getAlive()) {
		for (int i = wqR; i <= wkR; i++)
			if (piece[i].getAlive())
				wMat += piece[i].getValue();
		for (int i = bqR; i <= bkR; i++)
			if (piece[i].getAlive())
				bMat += piece[i].getValue();
		
		posi = to64(piece[wK].getPos())-1;
		index = posi+56 - posi/8*16;
		if (!(wMat <= 1310 && bMat <= 1310))
			score += kingTable1[index];
		else
			score += kingTable2[index];
	}
	for (int i = wPa; i <= wPh; i++) {
		if (piece[i].getAlive()) {
			posi = to64(piece[i].getPos())-1;
			index = posi+56 - posi/8*16;
			if (piece[i].getValue() == P_VAL)
				score += pawnTable[index];
			else
				score += queenTable[index];
		}
	}
	//---Black pieces
	for (int i = bqR; i <= bkR; i += bkR-bqR)
		if (piece[i].getAlive())
			score -= rookTable[to64(piece[i].getPos())-1];
	for (int i = bqN; i <= bkN; i += bkN-bqN)
		if (piece[i].getAlive())
			score -= knightTable[to64(piece[i].getPos())-1];
	for (int i = bqB; i <= bkB; i += bkB-bqB)
		if (piece[i].getAlive())
			score -= bishopTable[to64(piece[i].getPos())-1];
	if (piece[bQ].getAlive())
		score -= queenTable[to64(piece[bQ].getPos())-1];
	if (piece[bK].getAlive()) {
		if (!(wMat <= 1310 && bMat <= 1310))
			score -= kingTable1[to64(piece[bK].getPos())-1];
		else
			score -= kingTable2[to64(piece[bK].getPos())-1];
	}
	for (int i = bPa; i <= bPh; i++) {
		if (piece[i].getAlive()) {
			if (piece[i].getValue() == P_VAL)
				score -= pawnTable[to64(piece[i].getPos())-1];
			else
				score -= queenTable[to64(piece[i].getPos())-1];
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
	if (whiteCastled) score += 300;
	if (blackCastled) score -= 300;

	//Passed pawns
	int filesWithBlackPawns[8] = {0}, filesWithWhitePawns[8] = {0};
	for (int p = wPa; p <= wPh; p++)
		filesWithWhitePawns[piece[p].getPos()%10-1] = 1;
	for (int p = bPa; p <= bPh; p++)
		filesWithBlackPawns[piece[p].getPos()%10-1] = 1;
	for (int p = wPa; p <= wPh; p++)
		if (filesWithBlackPawns[piece[p].getPos()%10-1] == 0)
			score += 200;
	for (int p = bPa; p <= bPh; p++)
		if (filesWithWhitePawns[piece[p].getPos()%10-1] == 0)
			score -= 200;


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

	//Mobility
	//score += (int)whiteMoveList.size();
	//score -= (int)blackMoveList.size();

	return side ? score : -score;
}
