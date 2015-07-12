/*
----------------------------------
	~Moosey Chess Engine~
	      eval.cpp
----------------------------------
*/

#include <iostream>
#include "common.h"
#include "board.h"

int Board::eval(bool verbose) {
	using std::cout;

	if (verbose) {
		cout << "Evaluation\n";
		cout << "Whitematerial <= ENDGAME_VAL? " << (whiteMaterial <= ENDGAME_VAL) << '\n';
		cout << "Blackmaterial <= ENDGAME_VAL? " << (blackMaterial <= ENDGAME_VAL) << "\n\n";
	}

	enum eval_t { BISHOPPAIR = 50, CASTLED = 50, CASTLINGRIGHTS = 50, 
		      PASSEDPAWN = 50, DOUBLEDPAWN = 50, OPENFILEROOK = 50,
		      PAWNCHAIN = 5, ISOLATEDPAWN = 20
	};

	int reverse[64] = { 56, 57, 58, 59, 60, 61, 62, 63,
		 	    48, 49, 50, 51, 52, 53, 54, 55,
		            40, 41, 42, 43, 44, 45, 46, 47,
		  	    32, 33, 34, 35, 36, 37, 38, 39,
			    24, 25, 26, 27, 28, 29, 30, 31,
			    16, 17, 18, 19, 20, 21, 22, 23,
			     8,  9, 10, 11, 12, 13, 14, 15,
			     0,  1,  2,  3,  4,  5,  6,  7 };

	static int pawnTable1[64] = { 0,  0,  0,  0,  0,  0,  0,  0,
			 	      5,10, 10,-20,-20, 10, 10,  5,
				      5, -5,-10,  0,  0,-10, -5,  5,
				      0,  0,  0, 20, 20,  0,  0,  0,
				      5,  5, 10, 25, 25, 10,  5,  5,
				     10, 10, 20, 30, 30, 20, 10, 10,
				     50, 50, 50, 50, 50, 50, 50, 50,
				      0,  0,  0,  0,  0,  0,  0,  0 };

	static int pawnTable2[64] = { 0,  0,  0,  0,  0,  0,  0,  0,
				      5,  5,  5,  5,  5,  5,  5,  5,
				     20, 20, 20, 20, 20, 20, 20, 20,
				     30, 30, 30, 30, 30, 30, 30, 30,
				     40, 40, 40, 40, 40, 40, 40, 40,
				     60, 60, 60, 60, 60, 60, 60, 60,
				     80, 80, 80, 80, 80, 80, 80, 80,
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
	if (verbose)
		cout << "Piece-square table\n----------\nWhite: ";
	for (int i = wqR; i <= wkR; i += wkR-wqR)
		if (piece[i].getAlive()) {
			score += rookTable[to64(piece[i].getPos())-1];
			if (verbose)
				cout << "R (" << rookTable[to64(piece[i].getPos())-1] << ") ";
		}
	for (int i = wqN; i <= wkN; i += wkN-wqN)
		if (piece[i].getAlive()) {
			score += knightTable[to64(piece[i].getPos())-1];
			if (verbose)
				cout << "N (" << knightTable[to64(piece[i].getPos())-1] << ") ";
		}
	for (int i = wqB; i <= wkB; i += wkB-wqB)
		if (piece[i].getAlive()) {
			score += bishopTable[to64(piece[i].getPos())-1];
			if (verbose)
				cout << "B (" << bishopTable[to64(piece[i].getPos())-1] << ") ";
		}
	if (piece[wQ].getAlive()) {
		score += queenTable[to64(piece[wQ].getPos())-1];
		if (verbose)
			cout << "Q (" << queenTable[to64(piece[wQ].getPos())-1] << ") ";
	}
	if (piece[wK].getAlive()) {
		if (whiteMaterial <= ENDGAME_VAL) {
			score += kingTable2[to64(piece[wK].getPos())-1];
			if (verbose)
				cout << "K (" << kingTable2[to64(piece[wK].getPos())-1] << ") ";
		}
		else {
			score += kingTable1[to64(piece[wK].getPos())-1];
			if (verbose)
				cout << "K (" << kingTable1[to64(piece[wK].getPos())-1] << ") ";
		}
	}
	for (int i = wPa; i <= wPh; i++) {
		if (piece[i].getAlive()) {
			if (piece[i].getValue() == P_VAL) {
				if (whiteMaterial <= ENDGAME_VAL) {
					score += pawnTable2[to64(piece[i].getPos())-1];
					if (verbose)
						cout << "P" << i << " (" << pawnTable2[to64(piece[i].getPos())-1] << ") ";
				}
				else {
					score += pawnTable1[to64(piece[i].getPos())-1];
					if (verbose)
						cout << "P" << i << " (" << pawnTable1[to64(piece[i].getPos())-1] << ") ";
				}
			}
			else {
				score += queenTable[to64(piece[i].getPos())-1];
				if (verbose)
					cout << "Q (" << queenTable[to64(piece[wQ].getPos())-1] << ") ";
			}
		}
	}
	if (verbose)
		cout << "\nScore: " << score << "\n";
	//---Black pieces
	if (verbose)
		cout << "\nBlack: ";
	for (int i = bqR; i <= bkR; i += bkR-bqR)
		if (piece[i].getAlive()) {
			score -= rookTable[reverse[to64(piece[i].getPos())-1]];
			if (verbose)
				cout << "R (" << rookTable[reverse[to64(piece[i].getPos())-1]] << ") ";
		}
	for (int i = bqN; i <= bkN; i += bkN-bqN)
		if (piece[i].getAlive()) {
			score -= knightTable[reverse[to64(piece[i].getPos())-1]];
			if (verbose)
				cout << "N (" << knightTable[reverse[to64(piece[i].getPos())-1]] << ") ";
		}
	for (int i = bqB; i <= bkB; i += bkB-bqB)
		if (piece[i].getAlive()) {
			score -= bishopTable[reverse[to64(piece[i].getPos())-1]];
			if (verbose)
				cout << "B (" << bishopTable[reverse[to64(piece[i].getPos())-1]] << ") ";
		}
	if (piece[bQ].getAlive()) {
		score -= queenTable[reverse[to64(piece[bQ].getPos())-1]];
		if (verbose)
			cout << "Q (" << queenTable[reverse[to64(piece[bQ].getPos())-1]] << ") ";
	}
	if (piece[bK].getAlive()) {
		if (blackMaterial <= ENDGAME_VAL) {
			score -= kingTable2[reverse[to64(piece[bK].getPos())-1]];
			if (verbose)
				cout << "K (" << kingTable2[reverse[to64(piece[bK].getPos())-1]] << ") ";
		}
		else {
			score -= kingTable1[reverse[to64(piece[bK].getPos())-1]];
			if (verbose)
				cout << "K (" << kingTable1[reverse[to64(piece[bK].getPos())-1]] << ") ";
		}
	}
	for (int i = bPa; i <= bPh; i++) {
		if (piece[i].getAlive()) {
			if (piece[i].getValue() == P_VAL) {
				if (blackMaterial <= ENDGAME_VAL) {
					score -= pawnTable2[reverse[to64(piece[i].getPos())-1]];
					if (verbose)
						cout << "P" << i << " (" << pawnTable2[reverse[to64(piece[i].getPos())-1]] << ") ";
				}
				else {
					score -= pawnTable1[reverse[to64(piece[i].getPos())-1]];
					if (verbose)
						cout << "P" << i << " (" << pawnTable1[reverse[to64(piece[i].getPos())-1]] << ") ";
				}
			}
			else {
				score -= queenTable[reverse[to64(piece[i].getPos())-1]];
				if (verbose)
					cout << "Q (" << queenTable[reverse[to64(piece[i].getPos())-1]] << ") ";
			}
		}
	}
	if (verbose)
		cout << "\nScore: " << score << '\n';

	if (verbose)
		cout << "\nMaterial\n----------\nWhite: " << whiteMaterial << " Black: " << blackMaterial << '\n';
	//Material
	score += whiteMaterial;
	score -= blackMaterial;
	if (verbose)
		cout << "Score: " << score << "\n\n";

	if (verbose)
		cout << "Bishop pairs\n----------\n";
	//Bishop pair
	if (verbose)
		cout << "White: ";
	if (piece[wqB].getAlive() && piece[wkB].getAlive()) {
		score += BISHOPPAIR;
		if (verbose)
			cout << "YES";
	}
	if (verbose)
		cout << "\nBlack: ";
	if (piece[bqB].getAlive() && piece[bkB].getAlive()) {
		score -= BISHOPPAIR;
		if (verbose)
			cout << "YES";
	}
	if (verbose)
		cout << "\nScore: " << score << "\n\n";

	if (verbose)
		cout << "Castled?\n----------\nWhite: ";
	//Castled
	if (whiteCastled) {
		score += CASTLED;
		if (verbose)
			cout << "YES";
	}
	if (verbose)
		cout << "\nBlack: ";
	if (blackCastled) {
		score -= CASTLED;
		if (verbose)
			cout << "YES";
	}
	if (verbose) {
		cout << "\nScore: " << score << "\n\n";
		cout << "\nAbility to castle\n----------\nWhite: ";
	}
	//Ability to castle
	if (!whiteCastled) {
		if (piece[wK].getMoved())
			score -= CASTLINGRIGHTS;
		else if (piece[wqR].getMoved() && piece[wkR].getMoved())
			score -= CASTLINGRIGHTS;
		if (verbose)
			cout << "penalized";
	}
	if (verbose)
		cout << "\nBlack: ";
	if (!blackCastled) {
		if (piece[bK].getMoved())
			score += CASTLINGRIGHTS;
		else if (piece[bqR].getMoved() && piece[bkR].getMoved())
			score += CASTLINGRIGHTS;
		if (verbose)
			cout << "penalized";
	}
	if (verbose)
		cout << "\nScore: " << score << "\n\n";


	if (verbose)
		cout << "Passed, isolated, chained pawns\n----------\nWhite: ";

	int blackPawnsOnFile[8] = {0}, whitePawnsOnFile[8] = {0};
	int file = 0, pos;
	//Count white pawns on each file
	for (int p = wPa; p <= wPh; p++) {
		if (!piece[p].getAlive()) continue;
		if (piece[p].getValue() != P_VAL) continue;
		whitePawnsOnFile[piece[p].getPos()%10-1]++;
	}
	//Count black pawns on each file
	for (int p = bPa; p <= bPh; p++) {
		if (!piece[p].getAlive()) continue;
		if (piece[p].getValue() != P_VAL) continue;
		blackPawnsOnFile[piece[p].getPos()%10-1]++;
	}
	//Passed and isolated pawns
	for (int p = wPa; p <= wPh; p++) {
		if (!piece[p].getAlive()) continue;
		if (piece[p].getValue() != P_VAL) continue;

		pos = piece[p].getPos();
		file = pos%10;
		//a-file special case
		if (file == 1) {
			//Passed
			if (blackPawnsOnFile[1] == 0 &&
			    blackPawnsOnFile[0] == 0 ) {
				score += PASSEDPAWN;
				if (verbose)
					cout << "Passed a-file ";
			}
			//Isolated
			if (whitePawnsOnFile[1] == 0) {
				score -= ISOLATEDPAWN;
				if (verbose)
					cout << "Isolated a-file ";
			}
			//Chain
			if (board120[pos-9] != empty &&
			    piece[board120[pos-9]].getValue() == P_VAL) {
				score += PAWNCHAIN;
				if (verbose)
					cout << "Chain a-file ";
			}
		}
		//h-file special case
		else if (file == 8) {
			//Passed
			if (blackPawnsOnFile[7] == 0 &&
			    blackPawnsOnFile[6] == 0) {
				score += PASSEDPAWN;
				if (verbose)
					cout << "Passed h-file ";
			}
			//Isolated
			if (whitePawnsOnFile[6] == 0) {
				score -= ISOLATEDPAWN;
				if (verbose)
					cout << "Isolated h-file ";
			}
			//Chain
			if (board120[pos-11] != empty &&
			    piece[board120[pos-11]].getValue() == P_VAL) {
				score += PAWNCHAIN;
				if (verbose)
					cout << "Chain h-file ";
			}
		}
		//files 2-7
		else {
			//Passed
			if (blackPawnsOnFile[file-2] == 0 &&
			    blackPawnsOnFile[file-1] == 0 &&
			    blackPawnsOnFile[file] == 0) {
				score += PASSEDPAWN;
				if (verbose)
					cout << "Passed " << file << "-file ";
			}
			//Isolated
			if (whitePawnsOnFile[file-2] == 0 &&
			    whitePawnsOnFile[file] == 0) {
				score -= ISOLATEDPAWN;
				if (verbose)
					cout << "Isloated " << file << " file ";
			}
			//Chain
			if (board120[pos-9] != empty &&
			    piece[board120[pos-9]].getValue() == P_VAL) {
				score += PAWNCHAIN;
				if (verbose)
					cout << "Chain " << file << " file ";
			}
			if (board120[pos-11] != empty &&
			    piece[board120[pos-11]].getValue() == P_VAL) {
				score += PAWNCHAIN;
				if (verbose)
					cout << "Chain " << file << " file ";
			}
		}
	}
	if (verbose)
		cout << "\nBlack: ";
	for (int p = bPa; p <= bPh; p++) {
		if (!piece[p].getAlive()) continue;
		if (piece[p].getValue() != P_VAL) continue;

		pos = piece[p].getPos();
		file = pos%10;

		//a-file special case
		if (file == 1) {
			//Passed
			if (whitePawnsOnFile[1] == 0 &&
			    whitePawnsOnFile[0] == 0) {
				score -= PASSEDPAWN;
				if (verbose)
					cout << "Passed a-file ";
			}
			//Isolated
			if (blackPawnsOnFile[1] == 0) {
				score += ISOLATEDPAWN;
				if (verbose)
					cout << "Isolated a-file ";
			}
			//Chain
			if (board120[pos+11] != empty &&
			    piece[board120[pos+11]].getValue() == P_VAL) {
				score -= PAWNCHAIN;
				if (verbose)
					cout << "Chain a-file ";
			}
		}
		//h-file special case
		else if (file == 8) {
			//Passed
			if (whitePawnsOnFile[7] == 0 &&
			    whitePawnsOnFile[6] == 0) {
				score -= PASSEDPAWN;
				if (verbose)
					cout << "Passed h-file ";
			}
			//Isolated
			if (blackPawnsOnFile[6] == 0) {
				score += ISOLATEDPAWN;
				if (verbose)
					cout << "Isolated h-file ";
			}
			//Chain
			if (board120[pos+9] != empty &&
			    piece[board120[pos+9]].getValue() == P_VAL) {
				score -= PAWNCHAIN;
				if (verbose)
					cout << "Chain h-file ";
			}
		}
		//files 2-7
		else {
			//Passed
			if (whitePawnsOnFile[file-2] == 0 &&
			    whitePawnsOnFile[file-1] == 0 &&
			    whitePawnsOnFile[file] == 0) {
				score -= PASSEDPAWN;
				if (verbose)
					cout << "Passed " << file << "-file ";
			}
			//Isolated
			if (blackPawnsOnFile[file-2] == 0 &&
			    blackPawnsOnFile[file] == 0) {
				score += ISOLATEDPAWN;
				if (verbose)
					cout << "Isloated " << file << " file ";
			}
			//Chain
			if (board120[pos+11] != empty &&
			    piece[board120[pos+11]].getValue() == P_VAL) {
				score -= PAWNCHAIN;
				if (verbose)
					cout << "Chain " << file << " File ";
			}
			if (board120[pos+9] != empty &&
			    piece[board120[pos+9]].getValue() == P_VAL) {
				score -= PAWNCHAIN;
				if (verbose)
					cout << "Chain " << file << " file ";
			}
		}
	}
	if (verbose)
		cout << "\nScore: " << score << "\n\n";	

	if (verbose)
		cout << "Doubled pawns\n----------\n";
	//Doubled pawns
	for (int f = 0; f < 8; f++) {
		if (whitePawnsOnFile[f] > 1) {
			score -= (whitePawnsOnFile[f]-1)*DOUBLEDPAWN;
			if (verbose)
				cout << "W penalty, file " << f+1 << '\n';
		}
		if (blackPawnsOnFile[f] > 1) {
			score += (blackPawnsOnFile[f]-1)*DOUBLEDPAWN;
			if (verbose)
				cout << "B penalty, file " << f+1 << '\n';
		}
	}
	if (verbose)
		cout << "Score: " << score << "\n\n";

	//Semi open filed rooks
	if (verbose)
		cout << "Open filed rooks\n----------\n";
	for (int j = wqR; j <= bqR; j += (bqR-wqR)) {
		for (int i = j; i <= j+7; i += 7) {
			if (!piece[i].getAlive()) continue;
			bool open = true;
			int rank = piece[i].getPos()/10;
			int file = piece[i].getPos()%10;
			int checkPiece;
			for (int r = rank+1; r <= 9; r++) { //Up
				checkPiece = board120[r*10+file];
				if (checkPiece != empty) {
					if (piece[checkPiece].getColor() != piece[i].getColor())
						if (piece[checkPiece].getValue() == P_VAL) {
							open = false;
							break;
						}
				}
			}
			for (int r = rank-1; r >= 2; r--) { //Down
				checkPiece = board120[r*10+file];
				if (checkPiece != empty) {
					if (piece[checkPiece].getColor() != piece[i].getColor())
						if (piece[checkPiece].getValue() == P_VAL) {
							open = false;
							break;
						}
				}
			}
			if (open) {
				if (j == wqR) {
					score += OPENFILEROOK;
					if (verbose)
						cout << "White on file " << file << '\n';
				}
				else {
					score -= OPENFILEROOK;
					if (verbose)
						cout << "Black on file " << file << '\n';
				}
			}
				//score = (j == wqR) ? score+OPENFILEROOK : score-OPENFILEROOK;
		}
	}
	if (verbose)
		cout << "Score: " << score << "\n\n";

	return side ? score : -score;
}
