/*
----------------------------------
	~Moosey Chess Engine~
	      board.cpp
----------------------------------
*/

#include "macros.h"
#include "board.h"
#include "move.h"
#include "pieces.h"

int board120[120], board64[65];
	
void initializePieces(std::string FEN) {
        int v;
        for (int i = wqR; i <= bPh; i++) {
                v = piece[i].value;
                piece[i].moveListSize = v==K_VAL ? 8 : (v==R_VAL ? 14 : (v==N_VAL ? 8 : (v==P_VAL ? 4 : (v==B_VAL ? 13 : 27))));
                piece[i].moveList = new int[piece[i].moveListSize];
        }
        if (FEN=="0") placePiecesDefault();
}

void placePiecesDefault() {
	for (int i = wqR; i <= wkR; i++) //White
		placePiece(i, i+1); 
	for (int i = wPa; i <= wPh; i++) 
		placePiece(i, A2+(i-wPa));
	for (int i = bPa; i <= bPh; i++)  //Black
		placePiece(i, A7+(i-bPa));
	for (int i = bqR; i <= bkR; i++) 
		placePiece(i, A8+(i-bqR));	
}

void placePiece(int pieceNumber, int square) {
	piece[pieceNumber].pos = square;
	board64[square] = pieceNumber;
	board120[from64(square)] = pieceNumber;
}

void emptyBoard() {
	for (int i = A1; i <= H8; i++) 
		board64[i] = -1;
	for (int i = 0; i < 119; i++)
		board120[i] = -99;
	for (int i = 2; i <= 9; i++) 
		for (int j = 1; j <= 8; j++) 
			board120[i*10+j] = -1;
}

int to64(int x) {
	if (x < 98 && x > 21 && !(x%10 == 0 || x%10 == 9))
			return x-20-2*((x-x%10)/10-2);
	return 0;
}

int from64(int x) {
	return x+20+((int)((x-1)/8))*2;
}

std::string intToSquare(int square) {
	std::string squareName;
	int fileNum, rankNum;
	if (square==0) return "";
	fileNum = (square-1)%8 + 1; //a=1, ..., h=8
	rankNum = (square-1)/8+1; //rank 1=1, ..., 8=8 
	squareName = (char) ((int)'a' + (fileNum-1));
	squareName += (char)(rankNum + 48);
	return squareName;
}
