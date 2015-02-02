/*
----------------------------------
	~Moosey Chess Engine~
	      board.cpp
----------------------------------
*/

#include <iostream>
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
	for (int i = 0; i < 120; i++) {
		if (i < 21 || i > 98)
			board120[i] = invalid;
		else if (i%10 == 0 || i%10 == 9)
			board120[i] = invalid;
		else
			board120[i] = empty;
	}
}

void Board::placePiece(int pieceNumber, int square) {
	piece[pieceNumber].pos = square;
	board120[square] = pieceNumber;
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
                piece[i].moveListSize = v==K_VAL ? 8 : v==R_VAL ? 14 : v==N_VAL ? 8 : v==P_VAL ? 4 : v==B_VAL ? 13:27;
                piece[i].moveList = new int[piece[i].moveListSize];
        }
}

void Board::setMove(int mF, int mT) {
	if (mF > 20 && mF < 99) moveFrom = mF;
	if (mT > 20 && mT < 99) moveTo = mT;
}

void Board::setPly(int newPly) {
	if (ply >= 0)  {
		ply = newPly;
		return;
	}
	std::cout << "Invalid setPly\n";
}

void Board::setSide(bool newSide) {
	side = newSide;
}

void Board::setPieceMoved(int newPM) {
	if (newPM >= empty && newPM <= bPh) {
		pieceMoved = newPM;
		return;
	}
	std::cout << "Invalid setPieceMoved\n";
}

void Board::setPieceMovedFrom(int newPMF) {
	if (newPMF >= 1 && newPMF <= 64) {
		pieceMovedFrom = newPMF;
		return;
	}
	std::cout << "Invalid setPieceMovedFrom\n";
}

void Board::setPrevOnMoveTo(int newPOMT) {
	if (newPOMT >= empty && newPOMT <= bPh) {
		prevOnMoveTo = newPOMT;
		return;
	}
	std::cout << "Invalid setPrevOnMoveTo\n";
}

void Board::setBoard120(int i, int v) {
	if (i >= 0 && i < 120) 
		if (v >= invalid && v <= bPh) {
			board120[i] = v;
			return;
		}
	std::cout << "Invalid setBoard120\n"; 
}

void Board::addToMovelist(bool side, int v) {
	if (side) whiteMoveList.push_back(v);
	else blackMoveList.push_back(v);
}

void Board::clearMoveList(bool side) {
	if (side) whiteMoveList.clear();
	else blackMoveList.clear();
}

void Board::killPiece(int i) {
	if (i >= wqR && i <= bPh) {
		piece[i].alive = false;
		return;
	}
	std::cout << "Invalid killPiece\n";
}

void Board::unkillPiece(int i) {
	if (i >= wqR && i <= bPh) {
		piece[i].alive = true;
		return;
	}
	std::cout << "Invalid unkillPiece\n";
}

void Board::setPiecePos(int p, int position) {
	if (!(p >= wqR && p <= bPh)) {
		std::cout << "Bad piece in setPiecePos\n";
		return;
	}
	if (!(position >= 0 && position <= 64)) {
		std::cout << "Bad pos in setPiecePos\n";
		return;
	}
	piece[p].pos = position;
}

void Board::incrMoved(int p) {
	if (!(p >= wqR && p <= bPh)) {
		std::cout << "Bad piece in incrMoved\n";
		return;
	}
	piece[p].moved++;
}

void Board::decrMoved(int p) {
	if (!(p >= wqR && p <= bPh)) {
		std::cout << "Bad piece in decrMoved\n";
		return;
	}
	piece[p].moved--;
}
