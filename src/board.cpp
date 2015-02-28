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

Board::~Board() {
	for (int i = wqR; i <= bPh; i++) 
		delete [] piece[i].moveList;
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
//---Board
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
//---Pieces
char Board::getPieceAbbr(int p) const {
	return (p >= wqR && p <= bPh) ? piece[p].abbr : '0';
}

int Board::getTimesMoved(int p) const {
	return (p >= wqR && p <= bPh) ? piece[p].moved : -1;
}

std::string Board::getName(int p) const {
	return (p >= wqR && p <= bPh) ? piece[p].name : "0";
}

bool Board::getAlive(int p) const {
	return (p >= wqR && p <= bPh) ? piece[p].alive : -1;
}

int Board::getFromPieceMoveList(int p, int i) const {
	if (!(p >= wqR && p <= bPh)) return -1;
	if (!(i > -1 && i < piece[p].moveListSize)) return -1;
	return piece[p].moveList[i];
}

int Board::getPieceMoveListSize(int p) const {
	return (p >= wqR && p <= bPh) ? piece[p].moveListSize : -1;
}

int Board::getValue(int p) const {
	return (p >= wqR && p <= bPh) ? piece[p].value : -1;
}

int Board::getPos(int p) const {
	return (p >= wqR && p <= bPh) ? piece[p].pos : -1;
}

//MUTATORS
//---Board
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
//---Pieces
void Board::killPiece(int p) {
	if (p >= wqR && p <= bPh) piece[p].alive = false;
}

void Board::unkillPiece(int p) {
	if (p >= wqR && p <= bPh) piece[p].alive = true;
}

void Board::setPiecePos(int p, int newPos) {
	if (p >= wqR && p <= bPh) piece[p].pos = newPos;
}

void Board::incrMoved(int p) {
	if (p >= wqR && p <= bPh) piece[p].moved++;
}

void Board::decrMoved(int p) {
	if (p >= wqR && p <= bPh) piece[p].moved--;
}
