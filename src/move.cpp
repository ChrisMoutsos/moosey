/*
----------------------------------
	~Moosey Chess Engine~
	      move.cpp
----------------------------------
*/

#include <iostream>
#include "board.h"

void Board::movePiece() {
	bool c = false;
	if (side) 
		c = (moveFrom == _E1 && (moveTo == _G1 || moveTo == _B1)); 
	else 
		c = (moveFrom == _E8 && (moveTo == _G8 || moveTo == _B8)); 
	movePiece(moveFrom, moveTo, c);
}

void Board::movePiece(int mF, int mT, bool castling) {
	int rExtra, kExtra, eExtra, killSquare = mT, epExtra = 0;
	bool s = piece[board120[mF]].color, passanting = false;

	if (!castling) {	
		if (piece[board120[mF]].value == P_VAL && mT == epSq) {
			passanting = true;
			killSquare = s ? mT-10 : mT+10;
			epExtra = s ? -10 : 10;
		}

		//Set potential en passant square
		if (piece[board120[mF]].value == P_VAL && abs(mF-mT) == 20)
			epSq = s ? mF+10 : mF-10;
		else epSq = null;


		prevOnMoveTo = board120[mT];
		movedFrom = mF;
		movedTo = mT;
		pieceMoved = board120[mF];
		
		pieceKilled = board120[killSquare];
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
	else {
		if (mT == _G1 || mT == _G8) {
			eExtra = 3;
			kExtra = 2;
			rExtra = 1;
		}
		else {
			eExtra = -4;
			kExtra = -2;
			rExtra = -1;
		}
	
		prevOnMoveTo = empty;
		movedFrom = mF;
		pieceMoved = board120[mF];
		
		board120[mF+kExtra] = board120[mF];
		board120[mF] = empty;
		board120[mF+rExtra] = board120[mF+eExtra];
		board120[mF+eExtra] = empty;

		piece[board120[mF+kExtra]].pos = mF+kExtra;
		piece[board120[mF+rExtra]].pos = mF+rExtra;
	
		piece[board120[mF+kExtra]].moved++;
		piece[board120[mF+rExtra]].moved++;
	}
	ply++;
}

void Board::unmovePiece() {
	bool c = false;
	if (side)
		c = (moveFrom == _E1 && (moveTo == _G1 || moveTo == _B1));
	else
		c = (moveFrom == _E8 && (moveTo == _G8 || moveTo == _B8)); 
	unmovePiece(moveFrom, moveTo, c);
}

void Board::unmovePiece(int mF, int mT, bool castling) {
	int rExtra, kExtra, eExtra;
	int diffMTMF = abs(mT-mF);
	bool s = piece[board120[mT]].color, unpassanting = false;

	if (!castling) {
		if (piece[pieceMoved].value == P_VAL && prevOnMoveTo == empty) {
			if (diffMTMF == 11 || diffMTMF == 9) {
				unpassanting = true;
				epSq = mT;
			}
		}
		board120[mF] = pieceMoved;
		piece[board120[mF]].pos = mF;
		piece[board120[mF]].moved--;
	
		board120[mT] = prevOnMoveTo;
		
		if (!unpassanting) {
			if (board120[mT] != empty) {
				piece[board120[mT]].pos = mT;
				piece[board120[mT]].alive = true;
			}
		}
		else {
			if (s) {
				board120[mT-10] = pieceKilled;
				piece[board120[mT-10]].pos = mT-10;
				piece[board120[mT-10]].alive = true;
			}
			else {
				board120[mT+10] = pieceKilled;
				piece[board120[mT+10]].pos = mT+10;
				piece[board120[mT+10]].alive = true;
			}
		}
	}
	else {
		if (mT == _G1 || mT == _G8) {
			eExtra = 3;
			kExtra = 2;
			rExtra = 1;
		}
		else {
			eExtra = -4;
			kExtra = -2;
			rExtra = -1;
		}
		board120[mF] = board120[mF+kExtra];
		board120[mT] = empty;
		board120[mF+eExtra] = board120[mF+rExtra];
		board120[mF+kExtra] = empty;
		board120[mF+rExtra] = empty;

		piece[board120[mF]].pos = mF;
		piece[board120[mF+eExtra]].pos = mF + eExtra;
		
		piece[board120[mF]].moved--;
		piece[board120[mF+eExtra]].moved--;	

	}
	ply--;
}

void Board::moveInfo() const {
	using namespace std;
	bool castling = false;

	!side ? cout << "White" : cout << "Black";

	if (moveFrom==_E1 && (moveTo == _B1 || moveTo == _G1))
		castling = true;
	else if (moveFrom==_E8 && (moveTo == _B8 || moveTo == _G8))
		castling = true;
	if (!castling) {
		cout << " moved " << piece[pieceMoved].name;
		cout << " from " << intToSquare(moveFrom);
		cout  << " to " << intToSquare(moveTo);
		if (prevOnMoveTo != empty) {
			cout << " and captured a ";
			cout << piece[prevOnMoveTo].name;
		}
	}
	else {
		cout << " castled ";
		if (moveTo == _B1 || moveTo == _B8)
			cout << "queenside";
		else
			cout << "kingside";
	}
	cout << "\n\n";
}	

void Board::changeTurn() {
	side = side ? 0 : 1;
}
