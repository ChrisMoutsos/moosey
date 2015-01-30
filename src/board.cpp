/*
----------------------------------
	~Moosey Chess Engine~
	      board.cpp
----------------------------------
*/

#include <string>
#include "board.h"

Board::Board() {
	initializeVars();
	emptyBoard();
	initializePieces();
	placePiecesDefault();	
}

Board::Board(std::string FEN) {
	initializeVars();
	emptyBoard();
	initializePieces();
	//FEN stuff
}

void Board::initializeVars() {
	moveFrom = 0;
	moveTo = 0;
	ply = 0;
	side = WHITE;
	pieceMoved = -1;
	pieceMovedFrom = 0;
	prevOnMoveTo = -1;
}

void Board::emptyBoard() {
	for (int i = A1; i <= H8; i++) 
		board64[i] = -1;
	for (int i = 0; i < 120; i++)
		board120[i] = -99;
	for (int i = 2; i <= 9; i++)
		for (int j = 1; j <= 8; j++) 
			board120[i*10+j] = -1;
}

void Board::placePiece(int pieceNumber, int square) {
	piece[pieceNumber].pos = square;
	board64[square] = pieceNumber;
	board120[from64(square)] = pieceNumber;
}

void Board::placePiecesDefault() {
	for (int i = wqR; i <= wkR; i++) //White
		placePiece(i, i+1);
	for (int i = wPa; i <= wPh; i++) 
		placePiece(i, A2+(i-wPa));
	for (int i = bPa; i <= bPh; i++) //Black
		placePiece(i, A7 +(i-bPa));
	for (int i = bqR; i <= bkR; i++) 
		placePiece(i, A8+(i-bqR));
}

void Board::initializePieces() {
        int v;
	pieceEntry pieceTemp[32] = {
		{"Rook", 'R', R_VAL, 0, 0, 1, 0, 1},{"Knight", 'N', N_VAL, 0, 0, 1, 0, 1}, 
		{"Bishop", 'B', B_VAL, 0, 0, 1, 0, 1}, {"Queen", 'Q', Q_VAL, 0, 0, 1, 0, 1}, 
		{"King", 'K', K_VAL, 0, 0, 1, 0, 1}, {"Bishop", 'B', B_VAL, 0, 0, 1, 0, 1},
        	{"Knight", 'N', N_VAL, 0, 0, 1, 0, 1}, {"Rook", 'R', R_VAL, 0, 0, 1, 0, 1}, 
		{"Pawn", 'P', P_VAL, 0, 0, 1, 0, 1}, {"Pawn", 'P', P_VAL, 0, 0, 1, 0, 1}, 
		{"Pawn", 'P', P_VAL, 0, 0, 1, 0,1 }, {"Pawn", 'P', P_VAL, 0, 0, 1, 0, 1}, 
		{"Pawn", 'P', P_VAL, 0, 0, 1, 0, 1}, {"Pawn", 'P', P_VAL, 0, 0, 1, 0, 1}, 
		{"Pawn", 'P', P_VAL, 0, 0, 1, 0, 1}, {"Pawn", 'P', P_VAL, 0, 0, 1, 0, 1}, 
		{"Rook", 'r', R_VAL, 0, 0, 1, 0, 0}, {"Knight", 'n', N_VAL, 0, 0, 1, 0, 0}, 
		{"Bishop", 'b', B_VAL, 0, 0, 1, 0, 0}, {"Queen", 'q', Q_VAL, 0, 0, 1, 0, 0}, 
		{"King", 'k', K_VAL, 0, 0, 1, 0, 0}, {"Bishop", 'b', B_VAL, 0, 0, 1, 0, 0}, 
		{"Knight", 'n', N_VAL, 0, 0, 1, 0, 0}, {"Rook", 'r', R_VAL, 0, 0, 1, 0, 0}, 
		{"Pawn", 'p', P_VAL, 0, 0, 1, 0, 0}, {"Pawn", 'p', P_VAL, 0, 0, 1, 0, 0}, 
		{"Pawn", 'p', P_VAL, 0, 0, 1, 0, 0}, {"Pawn", 'p', P_VAL, 0, 0, 1, 0, 0},
		{"Pawn", 'p', P_VAL, 0, 0, 1, 0, 0}, {"Pawn", 'p', P_VAL, 0, 0, 1, 0, 0}, 
		{"Pawn", 'p', P_VAL, 0, 0, 1, 0, 0}, {"Pawn", 'p', P_VAL, 0, 0, 1, 0, 0}
        };
	for (int i = 0; i < 32; i++)
		piece[i] = pieceTemp[i];

	for (int i = wqR; i <= bPh; i++) {
                v = piece[i].value;
                piece[i].moveListSize = v==K_VAL ? 8 : v==R_VAL ? 14 : v==N_VAL ? 8 : v==P_VAL ? 4 : v==B_VAL ? 13:27;
                piece[i].moveList = new int[piece[i].moveListSize];
        }
}

void Board::setMove(int mF, int mT) {
	if (mF > 0 && mF < 64) moveFrom = mF;
	if (mT > 0 && mT < 64) moveTo = mT;
}
