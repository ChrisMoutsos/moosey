/*
----------------------------------
	~Moosey Chess Engine~
	      board.cpp
----------------------------------
*/

#include <iostream>
#include <string>
#include "common.h"
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
	genOrderedMoveList();
}

Board::Board(std::string FEN) {
	initializeVars();
	emptyBoard();
	initializePieces();
	setSquarePositions();
	setButtonPositions();
	setSpriteClips();	
	genOrderedMoveList();
	//FEN stuff
}

void Board::setSquarePositions() {
	//Set positions of the squares in the display
	for (int i = 0; i < 64; i++) {
		squares[i].setPos(BXSTART+(SQ_SIZE*(i%8)),
				  BYSTART+B_SIZE-(SQ_SIZE*(i/8+1)));
		squares[i].setSq(i+1);
	}
}

void Board::setPiecesOnSquares() {
	//Update all of the pieces the display squares are holding
	for (int i = 0; i < 64; i++)
		squares[i].setPiece(board120[from64(i+1)]);
}

void Board::initializeVars() {
	moveFrom = moveTo = ply =  null;
	side = WHITE;
	whiteCastled = blackCastled = false;
	castling = sideInCheck = sideInCheckmate = 0;
	whiteMaterial = 8*P_VAL + 2*(R_VAL+B_VAL+N_VAL) + Q_VAL + K_VAL;
	blackMaterial = whiteMaterial;
	//Clear history heuristic tables
	for (int i = 0; i < 2; i++)
		for (int f = 0; f < 64; f++)
			for (int t = 0; t < 64; t++)
				hh[i][f][t] = 0;
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

	//Set names, abbreviations, and values
	for (int i = 0; i <= 16; i += 16) {
		piece[wqR+i].setName("Rook");
		piece[wqR+i].setAbbr('R' + 2*i);
		piece[wqR+i].setValue(R_VAL);

		piece[wqN+i].setName("Knight");
		piece[wqN+i].setAbbr('N' + 2*i);
		piece[wqN+i].setValue(N_VAL);

		piece[wqB+i].setName("Bishop");
		piece[wqB+i].setAbbr('B' + 2*i);
		piece[wqB+i].setValue(B_VAL);

		piece[wQ+i].setName("Queen");
		piece[wQ+i].setAbbr('Q' + 2*i);
		piece[wQ+i].setValue(Q_VAL);

		piece[wK+i].setName("King");
		piece[wK+i].setAbbr('K' + 2*i);
		piece[wK+i].setValue(K_VAL);

		piece[wkB+i].setName("Bishop");
		piece[wkB+i].setAbbr('B' + 2*i);
		piece[wkB+i].setValue(B_VAL);

		piece[wkN+i].setName("Knight");
		piece[wkN+i].setAbbr('N' + 2*i);
		piece[wkN+i].setValue(N_VAL);

		piece[wkR+i].setName("Rook");
		piece[wkR+i].setAbbr('R' + 2*i);
		piece[wkR+i].setValue(R_VAL);
	}
	for (int s = 0; s <= 16; s += 16) 
		for (int i = 0; i < 8; i++) { 
			piece[wPa+i+s].setName("Pawn");
			piece[wPa+i+s].setAbbr('P' + 2*s);
			piece[wPa+i+s].setValue(P_VAL);
		}

	//Set colors
	for (int i = wqR; i <= wPh; i++)
		piece[i].setColor(WHITE);
	for (int i = bqR; i <= bPh; i++)
		piece[i].setColor(BLACK);
	

	//Create moveLists
	for (int i = wqR; i <= bPh; i++) {
		v = piece[i].getValue();
		if (v == K_VAL)
			piece[i].setMoveListSize(10);
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
			genOrderedMoveList();
			checkCheck(getSide());
		}
		mF = -1;
		mT = -1;
	}
}

void Board::botMove() {
	std::cout << "Thinking for ";
	side ? std::cout << "White..." : std::cout << "Black...";
	std::cout << " (ply " << ply+1 << ")\n";
	displayBotText(*this);
	int move = 0;
	move = think(*this, 1);
	setMove(move/100, move%100);
	movePiece();
	changeTurn();
	genOrderedMoveList();
	checkCheck(side);
	std::cout << "White Material: " << whiteMaterial << " Black Material: " << blackMaterial << '\n';
}

//ACCESSORS
int Board::getFromMoveList(bool s, int i) const {
	if (s) {
		assert (i > -1 && i < (int)whiteMoveList.size());
		return whiteMoveList[i];
	}
	else {
		assert (i > -1 && i < (int)blackMoveList.size());
		return blackMoveList[i];
	}
}

int Board::getMoveListSize(bool s) const {
	return s ? (int)whiteMoveList.size() : (int)blackMoveList.size();
}

int Board::getEpSq(int i) const {
	assert(i > -1 && i < (int)moveInfo.size());
	return moveInfo[i].epSq;
}

int Board::getPmSq(int i) const {
	assert(i > -1 && i < (int)moveInfo.size());
	return moveInfo[i].pmSq;
}

int Board::getPrevOnMoveTo(int i) const {
	assert(i > -1 && i < (int)moveInfo.size());
	return moveInfo[i].prevOnMoveTo;
}

int Board::getPieceMoved(int i) const {
	assert(i > -1 && i < (int)moveInfo.size());
	return moveInfo[i].pieceMoved;
}

int Board::getMoveMade(int i) const {
	assert(i > -1 && i < (int)movesMade.size());
	return movesMade[i];
}


//MUTATORS
void Board::addToMovelist(bool s, int v) {
	if (s) whiteMoveList.push_back(v);
	else blackMoveList.push_back(v);
}

void Board::clearMoveList(bool s) {
	if (s) whiteMoveList.clear();
	else blackMoveList.clear();
}

//OPERATOR OVERLOADS
const int& Board::operator [](const int index) const {
	assert(index > -1 && index < 120);
	return board120[index];
}

int& Board::operator [](const int index) {
	assert(index > -1 && index < 120);
	return board120[index];
}
