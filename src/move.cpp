/*
----------------------------------
	~Moosey Chess Engine~
	      move.cpp
----------------------------------
*/

#include <iostream>
#include "common.h"
#include "board.h"

void Board::movePiece() {
	//Uses Board's moveFrom and moveTo, set previously with setMove(int, int)
	movePiece(moveFrom, moveTo);
}

void Board::movePiece(int mF, int mT) {
	bool s = piece[board120[mF]].getColor();
	int  mFVal = piece[board120[mF]].getValue(), epExtra = 0;
	int localEpSq = null, localPmSq = null;
	int localPrevOnMoveTo = board120[mT], localHalfMoveClock;
	
	localHalfMoveClock = ply == 0 ? 0 : moveInfo.back().halfMoveClock;

	//Add the move to movesMade
	movesMade.push_back(mF*100+mT);

	//Set castling flag appropriately
	if (mFVal == K_VAL && abs(mF-mT) == 2)
		castling = mF < mT ? KINGSIDE : QUEENSIDE;
	
	if (!castling) {	
		if (mFVal == P_VAL ) {
			//If the move is an en passant
			if (ply > 0 && mT == moveInfo.back().epSq)
				epExtra = s ? -10 : 10; //The offset to the killed pawn from mT
			//Set potential en passant target square if appropriate
			if (abs(mF-mT) == 20)
				localEpSq = s ? mF+10 : mF-10;  //The square the pawn skipped
			localHalfMoveClock = 0;
		}

		//If the move is a promotion
		if (mFVal == P_VAL && ((s && mT/10 == 9) || (!s && mT/10 == 2))) {
			localPmSq = mT;
			piece[board120[mF]].setValue(Q_VAL);
			piece[board120[mF]].setName("Queen");
			if (s) {
				piece[board120[mF]].setAbbr('Q');
				whiteMaterial += Q_VAL - P_VAL;
			}
			else {
				piece[board120[mF]].setAbbr('q');
				blackMaterial += Q_VAL - P_VAL;
			}
			piece[board120[mF]].setPromoted(true);


			int * temp = new int[27]; 		 //Make bigger movelist
			for (int i = 0; i < 4; i++)	 	 //Copy any old values over
				temp[i] = piece[board120[mF]].getFromMoveList(i);
			piece[board120[mF]].freeMoveList();	 //Free old moveList
			piece[board120[mF]].setMoveList(temp);	 //Point at new array
			piece[board120[mF]].setMoveListSize(27); //Update moveListSize
			for (int i = 4; i < 27; i++ )		 //Fill rest of array with zeroes
				piece[board120[mF]].setInMoveList(i, 0);
		}

		//If move is a capture
		if (board120[mT+epExtra] != empty) {
			//Update material
			if (!s) 
				whiteMaterial -= piece[board120[mT+epExtra]].getValue();
			else
				blackMaterial -= piece[board120[mT+epExtra]].getValue();
			//Kill the piece
			piece[board120[mT+epExtra]].kill();
			piece[board120[mT+epExtra]].setPos(null);
			if (epExtra != 0)
				board120[mT+epExtra] = empty;
	
			localHalfMoveClock++;
		}
	
		//Move the piece
		board120[mT] = board120[mF];
		piece[board120[mT]].setPos(mT);
		piece[board120[mT]].incrMoved();
		board120[mF] = empty;
	}
	else { //Castling
		if (s) whiteCastled = true;
		else blackCastled = true;

		//cExtras contains the distance from the king
		//to the square the rook's moving to,
		//and to the square the rook's moving from.
		int cExtras[2];
		if (castling == KINGSIDE) {
			cExtras[0] = 1; //rookmT - kingmF
			cExtras[1] = 3; //emptymT - kingmF
		}
		else {
			cExtras[0] = -1; //rookmT - kingmF
			cExtras[1] = -4; //emptymT - kingmF
		}
	
		board120[mT] = board120[mF];		 	    //Move king
		piece[board120[mT]].setPos(mT); 
		piece[board120[mT]].incrMoved();

		board120[mF+cExtras[0]] = board120[mF+cExtras[1]];  //Move rook
		piece[board120[mF+cExtras[0]]].setPos(mF+cExtras[0]);
		piece[board120[mF+cExtras[0]]].incrMoved();

		board120[mF] = empty;				    //Empty old king sq
		board120[mF+cExtras[1]] = empty;		    //Empty old rook sq

		castling = 0;
		localHalfMoveClock++;
	}

	//Update move info, increase ply
	moveInfo.push_back({localPmSq, localEpSq, board120[mT],
			    localPrevOnMoveTo, localHalfMoveClock});
	ply++;
}

void Board::unmovePiece() {
	//Uses most recently made move
	unmovePiece(movesMade.back()/100, movesMade.back()%100);
}

void Board::unmovePiece(int mF, int mT) {
	int epExtra = 0;
	bool s = piece[board120[mT]].getColor();

	//Set castling flag appropriately
	if (piece[board120[mT]].getValue() == K_VAL && abs(mF-mT) == 2)
			castling = mF < mT ? KINGSIDE : QUEENSIDE;

	if (!castling) {
		//If we are undoing an enpassant move
		if (piece[board120[mT]].getValue() == P_VAL && moveInfo.back().prevOnMoveTo == empty) {
			int diffMTMF = abs(mT - mF);
			if (diffMTMF == 11 || diffMTMF == 9) {
				epExtra = s ? -10 : 10; //The offset to the dead pawn from mT
			}
		}
		//If we are undoing a promotion
		else if (mT == moveInfo.back().pmSq) {
			piece[board120[mT]].setValue(P_VAL);
			piece[board120[mT]].setName("Pawn");
			if (s) {
				piece[board120[mT]].setAbbr('P');
				whiteMaterial -= Q_VAL - P_VAL;
			}
			else {
				piece[board120[mT]].setAbbr('p');
				blackMaterial -= Q_VAL - P_VAL;
			}
			piece[board120[mT]].setPromoted(false);
	
			int * temp = new int[4]; 		//Create smaller movelist
			piece[board120[mT]].freeMoveList();     //Free old moveList
			piece[board120[mT]].setMoveList(temp);	//Point at new moveList
			piece[board120[mT]].setMoveListSize(4); //Update moveListSize
		}	

		//Put the piece moved back
		board120[mF] = board120[mT];
		piece[board120[mF]].setPos(mF);

		//Decrease the piece's move count
		piece[board120[mF]].decrMoved();
	
		//Put whatever was on mT back
		board120[mT] = moveInfo.back().prevOnMoveTo;

		//If we are undoing an enpassant, put the dead pawn back where it was
		if (epExtra != 0)
			board120[mT+epExtra] = mT%10 - 1 + (!s ? wPa : bPa);
		
		//If we are undoing a capture or an en passant
		if (epExtra != 0 || board120[mT] != empty) { 
			//Put it back and unkill it
			piece[board120[mT+epExtra]].setPos(mT+epExtra);
			piece[board120[mT+epExtra]].unkill();
			if (!s) 
				whiteMaterial += piece[board120[mT+epExtra]].getValue();
			else
				blackMaterial += piece[board120[mT+epExtra]].getValue();
		}
	}
	else { //Uncastling
		if (s) whiteCastled = false;
		else blackCastled = false;
	
		//See comments in move() about cExtras
		int cExtras[2];
		if (castling == KINGSIDE) {
			cExtras[0] = 1; //rookmT - kingmF
			cExtras[1] = 3; //emptymT - kingmF
		}
		else {
			cExtras[0] = -1; //rookmT - kingmF
			cExtras[1] = -4; //emptymT - kingmF
		}

		//Move the king back
		board120[mF] = board120[mT];
		piece[board120[mF]].setPos(mF);
		piece[board120[mF]].decrMoved();
		board120[mT] = empty;

		//Move the rook back
		board120[mF+cExtras[1]] = board120[mF+cExtras[0]];
		piece[board120[mF+cExtras[1]]].setPos(mF+cExtras[1]);
		piece[board120[mF+cExtras[1]]].decrMoved();	
		board120[mF+cExtras[0]] = empty;

		castling = 0;
	}

	//Remove previous movesMade and moveInfo, decrease ply
	movesMade.pop_back();
	moveInfo.pop_back();
	ply--;
}

void Board::changeTurn() {
	side = side ? BLACK : WHITE;
}

void Board::undoMove() {
	if (ply == 0) return;
	
	changeTurn();
	unmovePiece();
	genOrderedMoveList();
	checkCheck(side);
	
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
