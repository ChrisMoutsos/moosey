/*
----------------------------------
	~Moosey Chess Engine~
	      eval.cpp
----------------------------------
*/

#include "board.h"

int Board::eval() {
	int score = 0;
	
	//Material
	for (int i = wqR; i <= wPh; i++) {
		if (piece[i].getAlive())
			score += piece[i].getValue();
	}
	for (int i = bqR; i <= bPh; i++) {
		if (piece[i].getAlive())
			score -= piece[i].getValue();
	}

	//Center control

	//Castled
	if (whiteCastled) score += 300;
	if (blackCastled) score -= 300;

	//Pawns

	//Open filed rooks
	for (int j = wqR; j <= bqR; j += (bqR-wqR)) {
		for (int i = j; i <= j+7; i += 7) {
			if (!piece[i].getAlive()) continue;
			bool open = true;
			int rank = piece[i].getPos()/10;
			int file = piece[i].getPos()%10;
			int checkPiece;
			for (int r = rank; r <= 9; i++) { //Up
				checkPiece = board120[r*10+file];
				if (checkPiece != empty)
					if (piece[checkPiece].getColor() != piece[i].getColor())
						open = false;
			}
			for (int r = rank; r >= 2; i--) { //Down
				checkPiece = board120[r*10+file];
				if (checkPiece != empty)
					if (piece[checkPiece].getColor() != piece[i].getColor())
						open = false;
			}
			if (open)
				score = (j == wqR) ? score+100 : score-100;
		}
	}

	//Mobility
	

	return side ? score : -score;
}
