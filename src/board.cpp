/*
----------------------------------
	~Moosey Chess Engine~
	      board.cpp
----------------------------------
*/

#include <iostream>
#include <string>
#include "board.h"
#include "display.h"

Board::Board() { 
	initializeVars();
	emptyBoard();
	initializePieces();
	placePiecesDefault();	
	setSquarePositions(*this);
	setSpriteClips();	
	generateMoveLists();
}

Board::Board(std::string FEN) {
	initializeVars();
	emptyBoard();
	initializePieces();
	setSquarePositions(*this);
	setSpriteClips();	
	generateMoveLists();
	//FEN stuff
}

Board::~Board() {
	for (int i = wqR; i <= bPh; i++) 
		delete [] piece[i].moveList;
}

void Board::initializeVars() {
	moveFrom = moveTo = ply =  null;
	side = WHITE;
	castling = 0;
	sideInCheck = 0; 
	sideInCheckmate = 0;
}

void Board::emptyBoard() {
	for (int i = 0; i < 120; i++) {
		if (i < 21 || i > 98)
			board120[i] = invalid;
		else if (i%10 == 0 || i%10 == 9)
			board120[i] = invalid;
		else
			board120[i] = empty;
	}
}

void Board::placePiece(int p, int sq) {
	piece[p].pos = sq;
	board120[sq] = p;
}

void Board::placePiecesDefault() {
	for (int i = _A1; i <= _H1; i++)
		placePiece(i-_A1, i);
	for (int i = _A2; i <= _H2; i++) 
		placePiece(i-_A2+8, i);
	for (int i = _A8; i <= _H8; i++) 
		placePiece(i-_A8+16, i);
	for (int i = _A7; i <= _H7; i++) 
		placePiece(i-_A7+24, i);
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
		switch (v) {
		case K_VAL:
			piece[i].moveListSize = 8;
			break;
		case Q_VAL:
			piece[i].moveListSize = 27;
			break;
		case R_VAL:
			piece[i].moveListSize = 14;
			break;
		case B_VAL:
			piece[i].moveListSize = 13;
			break;
		case N_VAL:
			piece[i].moveListSize =8;
			break;
		case P_VAL:
			piece[i].moveListSize = 4;
			break;
		}
		piece[i].moveList = new int[piece[i].moveListSize];
    }
}

void Board::handleInput(int& mF, int& mT, SDL_Event* e) {
	for (int i = 0; i < 64; i++)
		squares[i].handleEvent(e, mF, mT, side);
	if (mF != -1 && mT != -1) {
		mF = from64(mF);
		mT = from64(mT);
		if (legalMove(mF, mT, getSide(), 1)) {
			setMove(mF, mT);
			movePiece();
			changeTurn();
			generateMoveLists();
			checkCheck(getSide());
		}
		mF = -1;
		mT = -1;
	}
}

void Board::addToMovelist(bool s, int v) {
	if (s) whiteMoveList.push_back(v);
	else blackMoveList.push_back(v);
}

void Board::clearMoveList(bool s) {
	if (s) whiteMoveList.clear();
	else blackMoveList.clear();
}

int Board::getMoveListSize(bool s) const {
	if (s) 
		return (int)whiteMoveList.size();
	else 
		return (int)blackMoveList.size();
}
int Board::getFromMoveList(bool s, int i) const {
	if (s) 
		return whiteMoveList[i];
	else 
		return blackMoveList[i];
}
