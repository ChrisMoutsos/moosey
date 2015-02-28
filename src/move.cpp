/*
----------------------------------
	~Moosey Chess Engine~
	      move.cpp
----------------------------------
*/

#include <iostream>
#include "board.h"
#include <array>

void Board::movePiece() {
	movePiece(moveFrom, moveTo);
}

void Board::movePiece(int mF, int mT) {
	std::array<int, 3> cExtras; //For castling, it holds how far away
			            //mF is from final pos of K, R, and corner
	int killSquare = mT, epExtra = 0, mFVal = piece[board120[mF]].getValue();
	bool s = piece[board120[mF]].getColor(), passanting = false;
	int * temp = NULL;

	movesMade.push_back(mF*100+mT);
	
	if (!castling) {	
		if (piece[board120[mF]].getValue() == P_VAL && ply > 0 && mT == epSq.back()) { //Enpassanting
			passanting = true;
			killSquare = s ? mT-10 : mT+10;
			epExtra = s ? -10 : 10;
		}

		//Set potential en passant square
		if (piece[board120[mF]].getValue() == P_VAL && abs(mF-mT) == 20)
			if (s) epSq.push_back(mF+10);
			else epSq.push_back(mF-10);
		else epSq.push_back(null);

		if (mFVal == P_VAL && ((s && mT/10 == 9) || (!s && mT/10 == 2))) { //Promoting
			pmSq.push_back(mT);
			piece[board120[mF]].setValue(Q_VAL);
			piece[board120[mF]].setName("Queen");
			if (s)
				piece[board120[mF]].setAbbr('Q');
			else
				piece[board120[mF]].setAbbr('q');
			piece[board120[mF]].setPromoted(true);
			temp = new int[27]; //Make bigger movelist
			for (int i = 0; i < 4; i++) //Copy any old values over
				temp[i] = piece[board120[mF]].getFromMoveList(i);
			piece[board120[mF]].freeMoveList();	//Free old moveList
			piece[board120[mF]].setMoveList(temp);	//Point at new array
			for (int i = 4; i < 27; i++ )	//Fill rest of array with zeroes
				piece[board120[mF]].setInMoveList(i, 0);
			piece[board120[mF]].setMoveListSize(27); //Update moveListSize
		}
		else pmSq.push_back(null);

		prevOnMoveTo.push_back(board120[mT]);
		pieceMoved.push_back(board120[mF]);
		pieceKilled.push_back(board120[killSquare]);

		if (board120[mT+epExtra] != empty) {
			piece[board120[mT+epExtra]].kill();
			piece[board120[mT+epExtra]].setPos(null);
			if (passanting)
				board120[mT+epExtra] = empty;
		}
	
		board120[mT] = board120[mF];
		board120[mF] = empty;
		piece[board120[mT]].setPos(mT);
	
		piece[board120[mT]].incrMoved();
	}
	else { //Castling
		epSq.push_back(null);
		pmSq.push_back(null);		

		//cExtras = {kingmT-kingmF, rookmT-kingmF, emptymT-kingmF};
		cExtras = {-2, -1, -4};		//Queenside
		if (mT == _G1 || mT == _G8) 
			cExtras = {2, 1, 3};	//Kingside
	
		prevOnMoveTo.push_back(empty);
		pieceMoved.push_back(board120[mF]);
		pieceKilled.push_back(empty);
		
		board120[mF+cExtras[0]] = board120[mF];
		board120[mF] = empty;
		board120[mF+cExtras[1]] = board120[mF+cExtras[2]];
		board120[mF+cExtras[2]] = empty;

		piece[board120[mF+cExtras[0]]].setPos(mF+cExtras[0]);
		piece[board120[mF+cExtras[1]]].setPos(mF+cExtras[1]);
	
		piece[board120[mF+cExtras[0]]].incrMoved();
		piece[board120[mF+cExtras[1]]].incrMoved();
	}
	ply++;
}

void Board::unmovePiece() {
	int moveFrom2 = movesMade.back() / 100;
	int moveTo2 = movesMade.back() % 100;
	unmovePiece(moveFrom2, moveTo2);
}

void Board::unmovePiece(int mF, int mT) {
	std::array<int, 3> cExtras; //For castling, it holds how far away
			            //mF is from final pos of K, R, and corner
	int diffMTMF = abs(mT-mF), epExtra = 0;
	int * temp;
	bool s = piece[board120[mT]].getColor(), unpassanting = false;

	if (!castling) {
		//Unpassanting
		if (piece[pieceMoved.back()].getValue() == P_VAL && prevOnMoveTo.back() == empty) {
			if (diffMTMF == 11 || diffMTMF == 9) {
				unpassanting = true;
				epExtra = s ? -10 : 10;
			}
		}
		if (mT == pmSq.back()) { //Unpromoting
			piece[board120[mT]].setValue(P_VAL);
			piece[board120[mT]].setName("Pawn");
			if (s)
				piece[board120[mT]].setAbbr('P');
			else
				piece[board120[mT]].setAbbr('p');
			piece[board120[mT]].setPromoted(false);
			temp = new int[4]; //Create smaller movelist
			piece[board120[mT]].freeMoveList(); //Free old moveList
			piece[board120[mT]].setMoveList(temp);	//Point at new moveList
			piece[board120[mT]].setMoveListSize(4); //Update moveListSize
		}	
		board120[mF] = pieceMoved.back();
		piece[board120[mF]].setPos(mF);
		piece[board120[mF]].decrMoved();
	
		board120[mT] = prevOnMoveTo.back();
		if (unpassanting)
			board120[mT+epExtra] = pieceKilled.back();

		if (unpassanting || board120[mT] != empty) {
			piece[board120[mT+epExtra]].setPos(mT+epExtra);
			piece[board120[mT+epExtra]].kill();
		}
	}
	else { //Castling
		//cExtras = {kingmT-kingmF, rookmT-kingmF, emptymT-kingmF};
		cExtras = {-2, -1, -4};		//Queenside
		if (mT == _G1 || mT == _G8) 	
			cExtras = {2, 1, 3};	//Kingside

		board120[mF] = board120[mF+cExtras[0]];
		board120[mT] = empty;
		board120[mF+cExtras[2]] = board120[mF+cExtras[1]];
		board120[mF+cExtras[0]] = empty;
		board120[mF+cExtras[1]] = empty;

		piece[board120[mF]].setPos(mF);
		piece[board120[mF+cExtras[2]]].setPos(mF+cExtras[2]);
		
		piece[board120[mF]].decrMoved();
		piece[board120[mF+cExtras[2]]].decrMoved();	

	}

	movesMade.pop_back();
	prevOnMoveTo.pop_back();
	pieceMoved.pop_back();
	pieceKilled.pop_back();
	pmSq.pop_back();
	epSq.pop_back();

	ply--;
}

void Board::changeTurn() {
	side = side ? BLACK : WHITE;
	setCastling(0);
}

void Board::undoMove() {
	int mF2 = movesMade.back()/100;
	int mT2 = movesMade.back()%100;

	changeTurn();
	if (piece[pieceMoved.back()].getValue() == K_VAL) {
		if (mF2 == _E1) {
			if (mT2 == _G1) castling = KINGSIDE;
			else if (mT2 == _B1) castling = QUEENSIDE;
		}
		else if (mF2 == _E8) {
			if (mT2 == _G8) castling = KINGSIDE;
			else if (mT2 == _B8) castling = QUEENSIDE;
		}
	}
	unmovePiece();
	castling = 0;
	generateMoveLists();
	checkCheck(side);
	moveFrom = mF2;
	moveTo = mF2;
}

void Board::restart() {
	while (ply > 0)
		undoMove();	
	moveTo = null;
	moveFrom = null;
}
