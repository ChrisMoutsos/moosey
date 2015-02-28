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
#include "button.h"

Board::Board() { 
	initializeVars();
	emptyBoard();
	initializePieces();
	placePiecesDefault();	
	setSquarePositions();
	setButtonPositions();
	setSpriteClips();	
	generateMoveLists();
}

Board::Board(std::string FEN) {
	initializeVars();
	emptyBoard();
	initializePieces();
	setSquarePositions();
	setButtonPositions();
	setSpriteClips();	
	generateMoveLists();
	//FEN stuff
}

void Board::setSquarePositions() {
	for (int i = 0; i < 64; i++) {
		squares[i].setPos(BXSTART+(SQ_SIZE*(i%8)),
				  BYSTART+B_SIZE-(SQ_SIZE*(i/8+1)));
		squares[i].setSq(i+1);
	}
}

void Board::setPiecesOnSquares() {
	for (int i = 0; i < 64; i++)
		squares[i].setPiece(board120[from64(i+1)]);
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
	piece[p].setPos(sq);
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

	for (int i = 0; i <= 16; i += 16) {
		piece[wqR+i].setName("Rook");
		piece[wqR+i].setAbbr(char(int('R') + 2*i));
		piece[wqR+i].setValue(R_VAL);

		piece[wqN+i].setName("Knight");
		piece[wqN+i].setAbbr(char(int('N') + 2*i));
		piece[wqN+i].setValue(N_VAL);

		piece[wqB+i].setName("Bishop");
		piece[wqB+i].setAbbr(char(int('B') + 2*i));
		piece[wqB+i].setValue(B_VAL);

		piece[wQ+i].setName("Queen");
		piece[wQ+i].setAbbr(char(int('Q') + 2*i));
		piece[wQ+i].setValue(Q_VAL);

		piece[wK+i].setName("King");
		piece[wK+i].setAbbr(char(int('K') + 2*i));
		piece[wK+i].setValue(K_VAL);

		piece[wkB+i].setName("Bishop");
		piece[wkB+i].setAbbr(char(int('B') + 2*i));
		piece[wkB+i].setValue(B_VAL);

		piece[wkN+i].setName("Knight");
		piece[wkN+i].setAbbr(char(int('N') + 2*i));
		piece[wkN+i].setValue(N_VAL);

		piece[wkR+i].setName("Rook");
		piece[wkR+i].setAbbr(char(int('R') + 2*i));
		piece[wkR+i].setValue(R_VAL);
	}
	for (int s = 0; s <= 16; s += 16) 
		for (int i = 0; i < 8; i++) { 
			piece[wPa+i+s].setName("Pawn");
			piece[wPa+i+s].setAbbr(char(int('P') + 2*s));
			piece[wPa+i+s].setValue(P_VAL);
		}

	for (int i = wqR; i <= wPh; i++)
		piece[i].setColor(WHITE);
	for (int i = bqR; i <= bPh; i++)
		piece[i].setColor(BLACK);
	

	//Create moveLists
	for (int i = wqR; i <= bPh; i++) {
		v = piece[i].getValue();
		if (v == K_VAL)
			piece[i].setMoveListSize(8);
		else if (v == Q_VAL)
			piece[i].setMoveListSize(27);
		else if (v == R_VAL)
			piece[i].setMoveListSize(14);
		else if (v == B_VAL)
			piece[i].setMoveListSize(13);
		else if (v == N_VAL)
			piece[i].setMoveListSize(8);
		else if (v == P_VAL)
			piece[i].setMoveListSize(4);
		piece[i].setMoveList(new int[piece[i].getMoveListSize()]);
    }
}

void Board::handleInput(int& mF, int& mT, SDL_Event* e) {
	bool clearMFMT = false;
	for (int i = 0; i < 2; i++) 
		if (buttons[i].handleEvent(e, *this))
			clearMFMT = true;
	if (clearMFMT) {
		mF = -1;
		mT = -1;
	}
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

//ACCESSORS
int Board::getBoard120(int sq) const {
	return (sq > -1 && sq < 99) ? board120[sq] : invalid;
}

int Board::getFromMoveList(bool s, int i) const {
	if (s && i > -1 && i < (int)whiteMoveList.size()) 
		return whiteMoveList[i];
	else if (!s && i > -1 && i < (int)blackMoveList.size())
		return blackMoveList[i];
	else
		return -1;
}

int Board::getMoveListSize(bool s) const {
	return s ? (int)whiteMoveList.size() : (int)blackMoveList.size();
}

int Board::getEpSq(int i) const {
	return (i > -1 && i < (int)epSq.size()) ? epSq[i] : -1;
}

int Board::getPmSq(int i) const {
	return (i > -1 && i < (int)pmSq.size()) ? pmSq[i] : -1;
}

int Board::getMoveMade(int i) const {
	return (i > -1 && i < (int)movesMade.size()) ? movesMade[i] : -1;
}

int Board::getPrevOnMoveTo(int i) const {
	return (i > -1 && i < (int)prevOnMoveTo.size()) ? prevOnMoveTo[i] : invalid;
}

int Board::getPieceMoved(int i) const {
	return (i > -1 && i < (int)pieceMoved.size()) ? pieceMoved[i] : -1;
}

//MUTATORS
void Board::setBoard120(int i, int v) {
	if (i > -1 && i < 120) board120[i] = v;
}

void Board::addToMovelist(bool s, int v) {
	if (s) whiteMoveList.push_back(v);
	else blackMoveList.push_back(v);
}

void Board::clearMoveList(bool s) {
	if (s) whiteMoveList.clear();
	else blackMoveList.clear();
}
