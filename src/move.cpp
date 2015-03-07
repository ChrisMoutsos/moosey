/*
----------------------------------
	~Moosey Chess Engine~
	      move.cpp
----------------------------------
*/

#include <iostream>
#include <array>
#include "common.h"
#include "board.h"

void Board::movePiece() {
	movePiece(moveFrom, moveTo);
}

void Board::movePiece(int mF, int mT) {
	int epExtra = 0, mFVal = piece[board120[mF]].getValue();
	bool s = piece[board120[mF]].getColor(), passanting = false;
	int localEpSq = null, localPmSq = null;
	int localPieceMoved = board120[mF], localPrevOnMoveTo = board120[mT];

	movesMade.push_back(mF*100+mT);

	if (piece[board120[mF]].getValue() == K_VAL && abs(mF-mT) == 2)
		castling = mF < mT ? KINGSIDE : QUEENSIDE;
	else castling = 0;
	
	if (!castling) {	
		//If the move is an en passant
		if (piece[board120[mF]].getValue() == P_VAL && ply > 0 && mT == moveInfo.back().epSq) {
			passanting = true;
			epExtra = s ? -10 : 10;
			if (s) 
				whiteMaterial -= P_VAL;
			else
				blackMaterial -= P_VAL;
		}

		//Set potential en passant square
		if (piece[board120[mF]].getValue() == P_VAL && abs(mF-mT) == 20)
			localEpSq = s ? mF+10 : mF-10;

		//If the move is a promotion
		if (mFVal == P_VAL && ((s && mT/10 == 9) || (!s && mT/10 == 2))) {
			localPmSq = mT;
			piece[board120[mF]].setValue(Q_VAL);
			piece[board120[mF]].setName("Queen");
			if (s)
				piece[board120[mF]].setAbbr('Q');
			else
				piece[board120[mF]].setAbbr('q');
			piece[board120[mF]].setPromoted(true);

			int * temp = new int[27]; 		//Make bigger movelist
			for (int i = 0; i < 4; i++)	 	//Copy any old values over
				temp[i] = piece[board120[mF]].getFromMoveList(i);
			piece[board120[mF]].freeMoveList();	//Free old moveList
			piece[board120[mF]].setMoveList(temp);	//Point at new array
			for (int i = 4; i < 27; i++ )		//Fill rest of array with zeroes
				piece[board120[mF]].setInMoveList(i, 0);
			piece[board120[mF]].setMoveListSize(27); //Update moveListSize
		}

		//If move is a capture
		if (board120[mT+epExtra] != empty) {
			if (s) 
				whiteMaterial -= piece[board120[mT+epExtra]].getValue();
			else
				blackMaterial -= piece[board120[mT+epExtra]].getValue();
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
		if (s) whiteCastled = true;
		else blackCastled = true;

		int cExtras[3];
		if (castling == KINGSIDE) {
			cExtras[0] = 2; //kingmT - kingmf
			cExtras[1] = 1; //rookmT - kingmF
			cExtras[2] = 3; //emptymT - kingmF
		}
		else {
			cExtras[0] = -2; //kingmT - kingmf
			cExtras[1] = -1; //rookmT - kingmF
			cExtras[2] = -4; //emptymT - kingmF
		}
	
		board120[mF+cExtras[0]] = board120[mF]; //Move king
		board120[mF] = empty;			//Empty old king sq
		board120[mF+cExtras[1]] = board120[mF+cExtras[2]]; //Move rook
		board120[mF+cExtras[2]] = empty;		   //Empty old rook sq

		piece[board120[mF+cExtras[0]]].setPos(mF+cExtras[0]); 
		piece[board120[mF+cExtras[1]]].setPos(mF+cExtras[1]);
	
		piece[board120[mF+cExtras[0]]].incrMoved();
		piece[board120[mF+cExtras[1]]].incrMoved();

	}

	moveInfo.push_back({localPmSq, localEpSq, localPieceMoved, localPrevOnMoveTo});
	ply++;
	castling = 0;
}

void Board::unmovePiece() {
	unmovePiece(movesMade.back()/100, movesMade.back()%100);
}

void Board::unmovePiece(int mF, int mT) {
	int epExtra = 0;
	bool s = piece[board120[mT]].getColor(), unpassanting = false;

	if (piece[board120[mT]].getValue() == K_VAL && abs(mF-mT) == 2)
			castling = mF < mT ? KINGSIDE : QUEENSIDE;
	else castling = 0;

	if (!castling) {
		//Unpassanting
		if (piece[moveInfo.back().pieceMoved].getValue() == P_VAL && moveInfo.back().prevOnMoveTo == empty) {
			int diffMTMF = abs(mT - mF);
			if (diffMTMF == 11 || diffMTMF == 9) {
				unpassanting = true;
				epExtra = s ? -10 : 10;
			}
		}
		//Unpromoting
		if (mT == moveInfo.back().pmSq) {
			piece[board120[mT]].setValue(P_VAL);
			piece[board120[mT]].setName("Pawn");
			if (s)
				piece[board120[mT]].setAbbr('P');
			else
				piece[board120[mT]].setAbbr('p');
			piece[board120[mT]].setPromoted(false);
	
			int * temp = new int[4]; 		//Create smaller movelist
			piece[board120[mT]].freeMoveList();     //Free old moveList
			piece[board120[mT]].setMoveList(temp);	//Point at new moveList
			piece[board120[mT]].setMoveListSize(4); //Update moveListSize
		}	
		board120[mF] = moveInfo.back().pieceMoved;
		piece[board120[mF]].setPos(mF);
		piece[board120[mF]].decrMoved();
	
		board120[mT] = moveInfo.back().prevOnMoveTo;
		if (unpassanting) {
			board120[mT+epExtra] = mT%10 - 1 + (s ? wPa : bPa);
			if (s) 
				whiteMaterial += piece[moveInfo.back().prevOnMoveTo].getValue();
			else
				blackMaterial += piece[moveInfo.back().prevOnMoveTo].getValue();
		}
		
		if (moveInfo.back().prevOnMoveTo != empty) {
			if (s) 
				whiteMaterial += piece[moveInfo.back().prevOnMoveTo].getValue();
			else
				blackMaterial += piece[moveInfo.back().prevOnMoveTo].getValue();
		}

		if (unpassanting || board120[mT] != empty) { 
			piece[board120[mT+epExtra]].setPos(mT+epExtra);
			piece[board120[mT+epExtra]].unkill();
		}
	}
	else { //Castling
		if (s) whiteCastled = false;
		else blackCastled = false;

		int cExtras[3];
		if (castling == KINGSIDE) {
			cExtras[0] = 2; //kingmT - kingmf
			cExtras[1] = 1; //rookmT - kingmF
			cExtras[2] = 3; //emptymT - kingmF
		}
		else {
			cExtras[0] = -2; //kingmT - kingmf
			cExtras[1] = -1; //rookmT - kingmF
			cExtras[2] = -4; //emptymT - kingmF
		}

		board120[mF] = board120[mF+cExtras[0]];		   //Set king
		board120[mT] = empty;
		board120[mF+cExtras[2]] = board120[mF+cExtras[1]]; //Set rook
		board120[mF+cExtras[0]] = empty;
		board120[mF+cExtras[1]] = empty;

		piece[board120[mF]].setPos(mF);
		piece[board120[mF+cExtras[2]]].setPos(mF+cExtras[2]);
		
		piece[board120[mF]].decrMoved();
		piece[board120[mF+cExtras[2]]].decrMoved();	

	}

	movesMade.pop_back();
	moveInfo.pop_back();
	ply--;
	castling = 0;
}

void Board::changeTurn() {
	side = side ? BLACK : WHITE;
}

void Board::undoMove() {
	if (ply == 0) return;
	
	std::cout << "\nUndid ply " << ply << '\n';

	int mF2 = movesMade.back()/100;
	int mT2 = movesMade.back()%100;

	changeTurn();

	//Set castling flag if necessary
	if (piece[moveInfo.back().pieceMoved].getValue() == K_VAL) {
		if (mF2-mT2 == -2) castling = KINGSIDE;
		else if (mF2-mT2 == 2) castling = QUEENSIDE;
	}
	unmovePiece();
	genOrderedMoveList(); //Regen moves
	checkCheck(side);    //Re-checkCheck
	
	//Update moveFrom and moveTo
	moveFrom = movesMade.back()/100;     
	moveTo = movesMade.back()%100;	   
}

void Board::restart() {
	while (ply > 0)
		undoMove();	
	moveTo = null;
	moveFrom = null;
	std::cout << "\nRestarted game\n";
}
