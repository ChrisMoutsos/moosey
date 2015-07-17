/*
----------------------------------
	~Moosey Chess Engine~
	      board.cpp
----------------------------------
*/

#include <iostream>
#include <string>
#include <SDL2/SDL_mixer.h>
#include "common.h"
#include "board.h"

Board::Board() : display(this) { 
	initializeVars();
	emptyBoard();
	initializePieces();
	placePiecesDefault();	
	initializeZobrist();
	setSquarePositions();
	initializeBitboards();
	display.setSpriteClips();	
	display.setButtons();
}

Board::Board(std::string FEN) : display(this) {
	std::cout << "Loading FEN: " << FEN << '\n';
	initializeVars();
	emptyBoard();
	initializePieces();
	setSquarePositions();
	display.setSpriteClips();	
	display.setButtons();
	placePieces(FEN);
	initializeZobrist();
	moveInfo[0].zobrist = zobrist.key;
}

void Board::updateDisplay(const int& mF, const int& mT) {
	display.displayBoard(mF, mT);
}

void Board::initializeZobrist() {
	bool color;
	int pos;
	//Get initial key
	zobrist.key = 0;
	//Piece positions
	for (int p = 0; p < 32; p++) {
		if (!piece[p].getAlive()) continue;
		color = piece[p].getColor();
		pos = to64(piece[p].getPos());
		switch (piece[p].getValue()) {
			case R_VAL:
				zobrist.key ^= zobrist.piece[0][color][pos-1];
			case N_VAL:
				zobrist.key ^= zobrist.piece[1][color][pos-1];
			case B_VAL:
				zobrist.key ^= zobrist.piece[2][color][pos-1];
			case Q_VAL:
				zobrist.key ^= zobrist.piece[3][color][pos-1];
			case K_VAL:
				zobrist.key ^= zobrist.piece[4][color][pos-1];
			case P_VAL:
				zobrist.key ^= zobrist.piece[5][color][pos-1];
		}
	}
	//Side
	if (!side)
		zobrist.key ^= zobrist.side;

	//Castling availability
	if (piece[wK].getMoved() == 0) {
		if (piece[wkR].getMoved() == 0 && piece[wqR].getMoved() == 0) {
			zobrist.key ^= zobrist.castling[1][2];
		}
		else if (piece[wkR].getMoved() == 0) {
			zobrist.key ^= zobrist.castling[1][0];
		}
		else if (piece[wqR].getMoved() == 0) {
			zobrist.key ^= zobrist.castling[1][1];
		}
	}
	else 
		zobrist.key ^= zobrist.castling[1][3];

	if (piece[bK].getMoved() == 0) {
		if (piece[bkR].getMoved() == 0 && piece[bqR].getMoved() == 0) {
			zobrist.key ^= zobrist.castling[0][2];
		}
		else if (piece[bkR].getMoved() == 0) {
			zobrist.key ^= zobrist.castling[0][0];
		}
		else if (piece[bqR].getMoved() == 0) {
			zobrist.key ^= zobrist.castling[0][1];
		}
	}
	else 
		zobrist.key ^= zobrist.castling[0][3];
	
	//En passant square
	if (moveInfo.size() && moveInfo.back().epSq != 0)
		zobrist.key ^= zobrist.enPassant[to64(moveInfo.back().epSq)%10-1];
}

void Board::setSquarePositions() {
	//Set positions of the squares in the display
	if (!flipped) {
		for (int i = 0; i < 64; i++) {
			display.squares[i].setPos(display.getBoardXStart()+(display.getSqSize()*(i%8)),
					  display.getBoardYStart()+display.getBoardSize()-
					  (display.getSqSize()*(i/8+1)));
			display.squares[i].setSq(i+1);
		}
	}
	else {
		for (int i = 0; i < 64; i++) {
			display.squares[i].setPos(display.getBoardXStart()+display.getBoardSize()
					  -(display.getSqSize()*(i%8+1)),
					  display.getBoardYStart()+(display.getSqSize()*(i/8)));
			display.squares[i].setSq(i+1);
		}
	}
}

void Board::setPiecesOnSquares() {
	//Update all of the pieces the display squares are holding
	for (int i = 0; i < 64; i++)
		display.squares[i].setPiece(board120[from64(i+1)]);
}

void Board::initializeVars() {
	moveFrom = moveTo = ply = 0;
	side = WHITE;
	whiteIsBot = false;
	blackIsBot = true;
	whiteCastled = blackCastled = false;
	castling = sideInCheck = sideInCheckmate = 0;
	whiteMaterial = 8*P_VAL + 2*(R_VAL+B_VAL+N_VAL) + Q_VAL + K_VAL;
	blackMaterial = whiteMaterial;
	flipped = false;
/*
	numWhiteMoves = numBlackMoves = 0;
	for (int i = 0; i < 256; i++) {
		whiteMoveList[i] = 0;
		blackMoveList[i] = 0;
	}
*/
}

void Board::initializeBitboards() {
	int pos, t;
	for (int s = BLACK; s <= WHITE; s++) {
		for (int p = bqR-(s*16); p<= bPh-(s*16); p++) {
			if (piece[p].getAlive()) {
				pos = to64(piece[p].getPos())-1;
				t = piece[p].getType();
				bb.pieces[s][t] |= bb.sq[pos];
				bb.allPieces[s] |= bb.sq[pos];
			}
		}
	}
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

void Board::placePiece(unsigned int p, unsigned int sq) {
	piece[p].setPos(sq);
	board120[sq] = p;
}

void Board::placePiecesDefault() {
	for (int i = A1; i <= H1; i++)
		placePiece(i-A1, i);
	for (int i = A2; i <= H2; i++) 
		placePiece(i-A2+8, i);
	for (int i = A8; i <= H8; i++) 
		placePiece(i-A8+16, i);
	for (int i = A7; i <= H7; i++) 
		placePiece(i-A7+24, i);
}

void Board::initializePieces() {
	int v;
	
	//Set names, abbreviations, and values
	for (int i = 0; i <= 16; i += 16) {
		piece[wqR+i].setName("Rook");
		piece[wqR+i].setAbbr('R' + 2*i);
		piece[wqR+i].setValue(R_VAL);
		piece[wqR+i].setType(ROOK);

		piece[wqN+i].setName("Knight");
		piece[wqN+i].setAbbr('N' + 2*i);
		piece[wqN+i].setValue(N_VAL);
		piece[wqN+i].setType(KNIGHT);

		piece[wqB+i].setName("Bishop");
		piece[wqB+i].setAbbr('B' + 2*i);
		piece[wqB+i].setValue(B_VAL);
		piece[wqB+i].setType(BISHOP);

		piece[wQ+i].setName("Queen");
		piece[wQ+i].setAbbr('Q' + 2*i);
		piece[wQ+i].setValue(Q_VAL);
		piece[wQ+i].setType(QUEEN);

		piece[wK+i].setName("King");
		piece[wK+i].setAbbr('K' + 2*i);
		piece[wK+i].setValue(K_VAL);
		piece[wK+i].setType(KING);

		piece[wkB+i].setName("Bishop");
		piece[wkB+i].setAbbr('B' + 2*i);
		piece[wkB+i].setValue(B_VAL);
		piece[wkB+i].setType(BISHOP);

		piece[wkN+i].setName("Knight");
		piece[wkN+i].setAbbr('N' + 2*i);
		piece[wkN+i].setValue(N_VAL);
		piece[wkN+i].setType(KNIGHT);

		piece[wkR+i].setName("Rook");
		piece[wkR+i].setAbbr('R' + 2*i);
		piece[wkR+i].setValue(R_VAL);
		piece[wkR+i].setType(ROOK);
	}
	for (int s = 0; s <= 16; s += 16) 
		for (int i = 0; i < 8; i++) { 
			piece[wPa+i+s].setName("Pawn");
			piece[wPa+i+s].setAbbr('P' + 2*s);
			piece[wPa+i+s].setValue(P_VAL);
			piece[wPa+i+s].setType(PAWN);
		}

	//Set colors
	for (int i = wqR; i <= wPh; i++)
		piece[i].setColor(WHITE);
	for (int i = bqR; i <= bPh; i++)
		piece[i].setColor(BLACK);

	//Create moveLists
	for (int i = wqR; i <= bPh; i++) {
		piece[i].freeMoveList();
	
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

void Board::placePieces(std::string FEN) {
	unsigned int index = 0, sqCounter = A8, p;
	int wPIndex = wPa, bPIndex = bPa;

	//Place the pieces
	while (FEN[index] != ' ') {
		if (FEN[index] == '/')
			sqCounter -= 18;
		else if (isalpha(FEN[index])) {
			if (FEN[index] == 'Q')
				p = wQ;
			else if (FEN[index] == 'K')
				p = wK;
			else if (FEN[index] == 'R')
				p = piece[wqR].getPos() ? wkR : wqR;
			else if (FEN[index] == 'N')
				p = piece[wqN].getPos() ? wkN : wqN;
			else if (FEN[index] == 'B')
				p = piece[wqB].getPos() ? wkB : wqB;
			else if (FEN[index] == 'P') {
				p = wPIndex;
				wPIndex++;
			}
			else if (FEN[index] == 'q')
				p = bQ;
			else if (FEN[index] == 'k')
				p = bK;
			else if (FEN[index] == 'r')
				p = piece[bqR].getPos() ? bkR : bqR;
			else if (FEN[index] == 'n')
				p = piece[bqN].getPos() ? bkN : bqN;
			else if (FEN[index] == 'b')
				p = piece[bqB].getPos() ? bkB : bqB;
			else if (FEN[index] == 'p') {
				p = bPIndex;
				bPIndex++;
			}

			placePiece(p, sqCounter);	
			sqCounter++;
		}
		else
			sqCounter += FEN[index] - '0';

		index++;
	}

	whiteMaterial = blackMaterial = 0;
	for (int i = wqR; i <= bPh; i++) {
		//Kill pieces that weren't placed
		if (piece[i].getPos() == 0)
			piece[i].kill();
		if (!piece[i].getAlive()) continue;
		//Update material
		if (i <= wPh) 
			whiteMaterial += piece[i].getValue();
		else 
			blackMaterial += piece[i].getValue();
		//Incr pawn's movecount if they have moved
		if (piece[i].getValue() == P_VAL) {
			if (i <= wPh) {
				if (piece[i].getPos()/10 != 3)
					piece[i].incrMoved();
			}
			else {
				if (piece[i].getPos()/10 != 8)
					piece[i].incrMoved();
			}
		}
	}

	//Set the side to move
	index++;
	side = (FEN[index] == 'w') ? 1 : 0;

	//Set castling permissions
	index += 2;
	piece[wqR].incrMoved();
	piece[wkR].incrMoved();
	piece[bqR].incrMoved();
	piece[bkR].incrMoved();
	while (FEN[index] != ' ') {
		if (FEN[index] == 'K')
			piece[wkR].decrMoved();
		else if (FEN[index] == 'Q')
			piece[wqR].decrMoved();
		else if (FEN[index] == 'k')
			piece[bkR].decrMoved();
		else if (FEN[index] == 'q')
			piece[bqR].decrMoved();
		index++;
	}

	//Set en passant square
	int epSq = 0;
	index++;
	if (FEN[index] != '-') {
		int row, file;
		file = FEN[index] - 'a' + 1;
		index++; 
		row = FEN[index] - '0' + 1;
		epSq = row*10 + file;
	}
	index++;	

	//Set half move clock
	index++;
	int halfMoveClock = FEN[index] - '0';
	index++;
	while (FEN[index] != ' ') {
		halfMoveClock *= 10;
		index++;
		halfMoveClock += FEN[index] - '0';
		index++;
	}

	//Set ply
	index++;
	int moves;
	moves = FEN[index] - '0';
	while (index+1 != FEN.size() && FEN[index+1] != ' ') {
		moves *= 10;
		index++;
		moves += FEN[index] - '0';
	}
	ply = (moves-1)*2;
	if (!side) ply++;

	//Update moveInfo
	moveInfo.push_back({0, epSq, -1, -1, halfMoveClock});
}

void Board::handleInput(int& mF, int& mT, SDL_Event* e) {
	display.handleButtons(e);

	int sound = 0;
	for (int i = 0; i < 64; i++) {
		display.squares[i].handleEvent(e, mF, mT, side, sound);
		if (sound == 1)
			Mix_PlayChannel(-1, display.mFSound, 0);
		else if (sound == 2)
			Mix_PlayChannel(-1, display.mTSound, 0);
	}
	
	if (mF != -1 && mT != -1) {
		mF = from64(mF);
		mT = from64(mT);
		if (legalMove(mF, mT, getSide(), 1)) {
			setMove(mF, mT);
			movePiece();
			changeTurn();
			genOrderedMoveList();
			checkCheck(getSide());
			std::cout << "Current FEN (ply " << ply << ", move " << (ply-1)/2+1 << "): " << getFEN() << '\n';
			std::cout << "Current Zobrist: " << zobrist.key << '\n';
			if (drawCheck() == 2)
				std::cout << "Threefold repetition.\n";
			std::cout << "-----------------------------------------------------------------------------\n\n";
			//eval(true);
/*
			std::cout << "bitboards\n--------------------------------\n";
			std::cout << "allWhite: " << bb.allPieces[WHITE] << " allBlack: " << bb.allPieces[BLACK] << '\n';
			for (int t = ROOK; t <= PAWN; t++) {
				std::cout << "t: " << t << '\n';
				std::cout << "black: " << bb.pieces[BLACK][t] << " white: " << bb.pieces[WHITE][t] << '\n';
			}
			std::cout << '\n';
*/
		}
		mF = mT = -1;
	}
}

void Board::botMove() {
	if (sideInCheckmate)
		return;
	std::cout << "Thinking for ";
	side ? std::cout << "White..." : std::cout << "Black...";
	std::cout << " (ply " << ply+1 << ", move " << ply/2 + 1 << ")\n";
	display.displayBotText();
	int move = 0;
	if (side)
		move = whiteBot.think(*this, whiteBot.getLevel());
	else
		move = blackBot.think(*this, blackBot.getLevel());

	if (move == 0) {
		std::cout << "Stalemate!\n";
		sideInCheckmate = 3;
		return;
	}

	setMove(move/100, move%100);
	movePiece();
	if (!muted)
		Mix_PlayChannel(-1, display.mFSound, 0);
	changeTurn();
	genOrderedMoveList();
	checkCheck(side);
	std::cout << "White material: " << whiteMaterial << " Black material: " << blackMaterial << '\n';
	std::cout << "Current FEN (ply " << ply << ", move " << (ply-1)/2+1 << "): " << getFEN() << '\n';
	std::cout << "Current Zobrist: " << zobrist.key << '\n';
	if (drawCheck() == 2) {
		std::cout << "Threefold repetition.\n";
		sideInCheckmate = 3;
	}
	std::cout << "-----------------------------------------------------------------------------\n\n";
	//eval(true);
}

void Board::changeTurn() {
	side = side ? BLACK : WHITE;
}

void Board::undoMove() {
	if (movesMade.size() == 0) {
		return;
	}

	//Stalemate
	if (movesMade.back() == 0) {
		movesMade.pop_back();
		moveInfo.pop_back();
		ply--;
		changeTurn();
		checkCheck(side);
		return;
	}
	
	changeTurn();
	unmovePiece();
	genOrderedMoveList();
	checkCheck(side);
	
	moveFrom = movesMade.back()/100;     
	moveTo = movesMade.back()%100;	   
}

void Board::restart() {
	if (!start) return;
	
	//If we didn't load from a FEN
	if (ply == (int)movesMade.size()) {
		while (movesMade.size())
			undoMove();
		start = false;
		std::cout << "\nRestarted game\n";
	}
	//If we loaded from a FEN
	else {
		while (movesMade.size())
			undoMove();	
		moveTo = moveFrom = 0;
		std::cout << "\nReloaded FEN\n";
	}

}

//ACCESSORS
std::string Board::getFEN() {
	using namespace std;

	string FEN;
	int emptyCount = 0;

	//Piece positions
	for (int j = 0; j < 8; j++) {
		emptyCount = 0;
		for (int i = A8 - j*10; i <= H8 - j*10; i++) {
			if (board120[i] == empty)
				emptyCount++;
			else {
				if (emptyCount > 0) {
					FEN += to_string(emptyCount);
					emptyCount = 0;
				}
				FEN += piece[board120[i]].getAbbr();
			}
		}	
		if (emptyCount > 0) {
			FEN += to_string(emptyCount);
			emptyCount = 0;
		}
		if (j != 7)
			FEN += '/';
	}

	//Side to move
	FEN += side ? " w " : " b ";

	//Castling availability
	if (piece[wK].getMoved() == 0) {
		if (piece[wkR].getMoved() == 0)
			FEN += 'K';
		if (piece[wqR].getMoved() == 0)
			FEN += 'Q';
	}
	if (piece[bK].getMoved() == 0) {
		if (piece[bkR].getMoved() == 0)
			FEN += 'k';
		if (piece[bqR].getMoved() == 0)
			FEN += 'q';
	}
	if (FEN.back() == ' ')
		FEN += '-';

	//En passant target square
	if (moveInfo.size() && moveInfo.back().epSq != 0)
		FEN += " " + intToSquare(moveInfo.back().epSq) + " ";
	else 
		FEN += " - ";
	
	//Halfmove clock
	FEN += (moveInfo.size() == 0) ? "0" : to_string(moveInfo.back().halfMoveClock);
	
	//Full move clock
	FEN += " " + to_string(ply/2 + 1);

	return FEN;
}

int Board::getFromMoveList(bool s, int i) const {
	if (s) {
		return whiteMoveList[i];
	}
	else {
		return blackMoveList[i];
	}
}

int Board::getMoveListSize(bool s) const {
	return s ? int(whiteMoveList.size()) : int(blackMoveList.size());
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

int Board::getNumMovesMade() const {
	return int(movesMade.size());
}

int Board::getLastMove() const {
	if (movesMade.size()) return movesMade.back();
	return 0;
}


//MUTATORS
void Board::addToMovelist(bool s, int v) {
	if (s) {
		whiteMoveList.push_back(v);
	}
	else {
		blackMoveList.push_back(v);
	}
}

void Board::clearMoveList(bool s) {
	if (s) {
		whiteMoveList.clear();
	}
	else {
		blackMoveList.clear();
	}
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
