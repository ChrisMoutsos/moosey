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
	int killSquare = mT, epExtra = 0, mFVal = piece[board120[mF]].value;
	bool s = piece[board120[mF]].color, passanting = false;
	int * temp = NULL;

	movesMade.push_back(mF*100+mT);
	
	if (!castling) {	
		if (piece[board120[mF]].value == P_VAL && ply > 0 && mT == epSq.back()) { //Enpassanting
			passanting = true;
			killSquare = s ? mT-10 : mT+10;
			epExtra = s ? -10 : 10;
		}

		//Set potential en passant square
		if (piece[board120[mF]].value == P_VAL && abs(mF-mT) == 20)
			if (s) epSq.push_back(mF+10);
			else epSq.push_back(mF-10);
		else epSq.push_back(null);

		if (mFVal == P_VAL && ((s && mT/10 == 9) || (!s && mT/10 == 2))) { //Promoting
			pmSq.push_back(mT);
			piece[board120[mF]].value = Q_VAL;
			piece[board120[mF]].name = "Queen";
			piece[board120[mF]].abbr = s ? 'Q' : 'q';
			piece[board120[mF]].promoted = true;
			temp = new int[27]; //Make bigger movelist
			for (int i = 0; i < 4; i++) //Copy any old values over
				temp[i] = piece[board120[mF]].moveList[i];
			delete [] piece[board120[mF]].moveList;	//Free old moveList
			piece[board120[mF]].moveList = temp;	//Point at new array
			for (int i = 4; i < 27; i++ )	//Fill rest of array with zeroes
				piece[board120[mF]].moveList[i] = 0;
		}
		else pmSq.push_back(null);

		prevOnMoveTo.push_back(board120[mT]);
		pieceMoved.push_back(board120[mF]);
		pieceKilled.push_back(board120[killSquare]);

		if (board120[mT+epExtra] != empty) {
			piece[board120[mT+epExtra]].alive = false;
			piece[board120[mT+epExtra]].pos = null;
			if (passanting)
				board120[mT+epExtra] = empty;
		}
	
		board120[mT] = board120[mF];
		board120[mF] = empty;
		piece[board120[mT]].pos = mT;
	
		piece[board120[mT]].moved++;
	}
	else { //Castling
		epSq.push_back(null);
		pmSq.push_back(null);		

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

		piece[board120[mF+cExtras[0]]].pos = mF+cExtras[0];
		piece[board120[mF+cExtras[1]]].pos = mF+cExtras[1];
	
		piece[board120[mF+cExtras[0]]].moved++;
		piece[board120[mF+cExtras[1]]].moved++;
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
	int diffMTMF = abs(mT-mF), epExtra = 0, fakeCounter = 0;
	int * temp;
	bool s = piece[board120[mT]].color, unpassanting = false;

	if (!castling) {
		//Unpassanting
		if (piece[pieceMoved.back()-1].value == P_VAL && prevOnMoveTo[prevOnMoveTo.back()] == empty) {
			if (diffMTMF == 11 || diffMTMF == 9) {
				unpassanting = true;
				epExtra = s ? -10 : 10;
			}
		}
		if (mT == pmSq.back()) { //Promoting
			piece[board120[mT]].value = P_VAL;
			piece[board120[mT]].name = "Pawn";
			piece[board120[mT]].abbr = s ? 'P' : 'p';
			piece[board120[mT]].promoted = false;
			temp = new int[4]; //Create smaller movelist
			delete [] piece[board120[mT]].moveList; //Free old moveList
			piece[board120[mT]].moveList = temp;	//Point at new moveList
			generatePawnMoves(board120[mT], fakeCounter); //Regen moves
		}	
		board120[mF] = pieceMoved.back();
		piece[board120[mF]].pos = mF;
		piece[board120[mF]].moved--;
	
		board120[mT] = prevOnMoveTo.back();
		if (unpassanting)
			board120[mT+epExtra] = pieceKilled.back();

		if (unpassanting || board120[mT] != empty) {
			piece[board120[mT+epExtra]].pos = mT+epExtra;
			piece[board120[mT+epExtra]].alive = true;
		}
	}
	else { //Castling
		cExtras = {-2, -1, -4};		//Queenside
		if (mT == _G1 || mT == _G8) 	
			cExtras = {2, 1, 3};	//Kingside

		board120[mF] = board120[mF+cExtras[0]];
		board120[mT] = empty;
		board120[mF+cExtras[2]] = board120[mF+cExtras[1]];
		board120[mF+cExtras[0]] = empty;
		board120[mF+cExtras[1]] = empty;

		piece[board120[mF]].pos = mF;
		piece[board120[mF+cExtras[2]]].pos = mF+cExtras[2];
		
		piece[board120[mF]].moved--;
		piece[board120[mF+cExtras[2]]].moved--;	

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
	side = side ? 0 : 1;
	setCastling(0);
}

void Board::undoMove() {
	unmovePiece();
	changeTurn();
	moveFrom = movesMade.back() / 100;
	moveTo = movesMade.back() % 100;
}
