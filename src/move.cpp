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
	int type = piece[board120[mF]].getType();
	int  mFVal = piece[board120[mF]].getValue(), epExtra = 0;
	int localEpSq = null, localPmSq = null;
	int localPrevOnMoveTo = board120[mT], localHalfMoveClock;
	
	localHalfMoveClock = moveInfo.size() == 0 ? 0 : moveInfo.back().halfMoveClock+1;

	//Add the move to movesMade
	movesMade.push_back(mF*100+mT);
	
	//Undo last enpassant file in Zobrist key
	if (moveInfo.size() && moveInfo.back().epSq != 0)
		zobrist.key ^= zobrist.enPassant[to64(moveInfo.back().epSq)%10-1];

	//Set castling flag appropriately
	if (mFVal == K_VAL && abs(mF-mT) == 2)
		castling = mF < mT ? KINGSIDE : QUEENSIDE;
	
	if (!castling) {	
		zobrist.key ^= zobrist.piece[type][s][to64(mF)-1];

		if (mFVal == P_VAL ) {
			//If the move is an en passant
			if (moveInfo.size() && mT == moveInfo.back().epSq)
				epExtra = s ? -10 : 10; //The offset to the killed pawn from mT
			//Set potential en passant target square if appropriate
			if (abs(mF-mT) == 20) {
				localEpSq = s ? mF+10 : mF-10;  //The square the pawn skipped
				//Update zobrist key with en passant file
				zobrist.key ^= zobrist.enPassant[to64(mF)-1];
			}
			localHalfMoveClock = 0;
		}
		else if (mFVal == R_VAL) {
			//Take away castling permissions, update Zobrist key
			if ((s && mF == _A1) || (!s && mF == _A8)) {
				if (canCastleZ[s][1]) {
					zobrist.key ^= zobrist.castling[s][1];
					canCastleZ[s][1] = false;
				}
			}
			else if ((s && mF == _H1) || (!s && mF == _H8)) {
				if (canCastleZ[s][0]) {
					zobrist.key ^= zobrist.castling[s][0];
					canCastleZ[s][0] = false;
				}
			}
		}
		else if (mFVal == K_VAL) {
			//Take away castling permissions, update Zobrist key
			if (canCastleZ[s][0]) {
				zobrist.key ^= zobrist.castling[s][0];
				canCastleZ[s][0] = false;
			}
			if (canCastleZ[s][1]) {
				zobrist.key ^= zobrist.castling[s][1];
				canCastleZ[s][1] = false;
			}
		}

		//If the move is a promotion
		if (mFVal == P_VAL && ((s && mT/10 == 9) || (!s && mT/10 == 2))) {
			//Put a queen on mT
			zobrist.key ^= zobrist.piece[3][s][to64(mT)-1];

			localPmSq = mT;
			piece[board120[mF]].setValue(Q_VAL);
			piece[board120[mF]].setName("Queen");
			piece[board120[mF]].setType(3);
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
		else
			zobrist.key ^= zobrist.piece[type][s][to64(mT)-1];

		//If move is a capture
		if (board120[mT+epExtra] != empty) {
			//Update material
			if (!s) 
				whiteMaterial -= piece[board120[mT+epExtra]].getValue();
			else
				blackMaterial -= piece[board120[mT+epExtra]].getValue();
			//Kill the piece
			zobrist.key ^= zobrist.piece[piece[board120[mT+epExtra]].getType()][!s][to64(mT+epExtra)-1];
			piece[board120[mT+epExtra]].kill();
			piece[board120[mT+epExtra]].setPos(null);
			if (epExtra != 0)
				board120[mT+epExtra] = empty;
	
			localHalfMoveClock = 0;
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

		//Update zobrist key
		//Permissions
		if (canCastleZ[s][0] && castling == KINGSIDE) {
			zobrist.key ^= zobrist.castling[s][0];
			canCastleZ[s][0] = false;
		}
		if (canCastleZ[s][1] && castling == QUEENSIDE) {
			zobrist.key ^= zobrist.castling[s][1];
			canCastleZ[s][1] = false;
		}
		zobrist.key ^= zobrist.piece[4][s][to64(mF)-1];		   //Remove king
		zobrist.key ^= zobrist.piece[4][s][to64(mT)-1]; 	   //Place king
		zobrist.key ^= zobrist.piece[0][s][to64(mF+cExtras[1])-1]; //Remove rook
		zobrist.key ^= zobrist.piece[0][s][to64(mT+cExtras[0])-1]; //Place rook
	
		//Update board120
		board120[mT] = board120[mF];		 	    //Move king
		piece[board120[mT]].setPos(mT); 
		piece[board120[mT]].incrMoved();

		board120[mF+cExtras[0]] = board120[mF+cExtras[1]];  //Move rook
		piece[board120[mF+cExtras[0]]].setPos(mF+cExtras[0]);
		piece[board120[mF+cExtras[0]]].incrMoved();

		board120[mF] = empty;				    //Empty old king sq
		board120[mF+cExtras[1]] = empty;		    //Empty old rook sq

		castling = 0;
		localHalfMoveClock = 0;
	}

	zobrist.key ^= zobrist.side;
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
	int type = piece[board120[mT]].getType();

	//Take out recent en passant file in Zobrist key
	if (moveInfo.back().epSq != 0) {
		zobrist.key ^= zobrist.enPassant[to64(moveInfo.back().epSq)%10-1];
	}
	//Put back old en passant file in Zobrist key, if there is one
	if (moveInfo.size() > 1 && moveInfo[moveInfo.size()-2].epSq != 0) {
		zobrist.key ^= zobrist.enPassant[to64(moveInfo[moveInfo.size()-2].epSq)%10-1];
	}

	//Set castling flag appropriately
	if (piece[board120[mT]].getValue() == K_VAL && abs(mF-mT) == 2)
			castling = mF < mT ? KINGSIDE : QUEENSIDE;

	if (!castling) {
		zobrist.key ^= zobrist.piece[type][s][to64(mT)-1];
		zobrist.key ^= zobrist.piece[type][s][to64(mF)-1];

		//If we are undoing an enpassant move
		if (piece[board120[mT]].getValue() == P_VAL && moveInfo.back().prevOnMoveTo == empty) {
			int diffMTMF = abs(mT - mF);
			if (diffMTMF == 11 || diffMTMF == 9) {
				epExtra = s ? -10 : 10; //The offset to the dead pawn from mT
			}
		}
		//If we are undoing a promotion
		else if (mT == moveInfo.back().pmSq) {
			//Put a pawn back on mF
			zobrist.key ^= zobrist.piece[5][s][to64(mF)-1];

			piece[board120[mT]].setType(5);
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
		if (moveInfo.back().prevOnMoveTo != empty)
			zobrist.key ^= zobrist.piece[piece[moveInfo.back().prevOnMoveTo].getType()][!s][to64(mT)-1];

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
			zobrist.key ^= zobrist.piece[piece[board120[mT+epExtra]].getType()][!s][to64(mT+epExtra)-1];
		}

		//Give back castling permissions
		if (!canCastleZ[s][0] || !canCastleZ[s][1]) {
			if (piece[board120[mF]].getMoved() == 1) {
				if (piece[board120[mF]].getValue() == R_VAL) {
					if ((s && mF == _A1) || (!s && mF == _A8)) {
						zobrist.key ^= zobrist.castling[s][1];
						canCastleZ[s][1] = true;
					}
					else if ((s && mF == _H1) || (!s && mF == _H8)) {
						zobrist.key ^= zobrist.castling[s][0];
						canCastleZ[s][0] = true;
					}
				}
				else if (piece[board120[mF]].getValue() == K_VAL) {
					if (piece[board120[mF]-4].getMoved() == 0) {
						zobrist.key ^= zobrist.castling[s][1];
						canCastleZ[s][1] = true;
					}
					if (piece[board120[mF]+3].getMoved() == 0) {
						zobrist.key ^= zobrist.castling[s][0];
						canCastleZ[s][0] = true;
					}
				}
			}
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
			//Zobrist key permissoins
			zobrist.key ^= zobrist.castling[s][0];
			canCastleZ[s][0] = true;
		}
		else {
			cExtras[0] = -1; //rookmT - kingmF
			cExtras[1] = -4; //emptymT - kingmF
			//Zobrist key permissoins
			zobrist.key ^= zobrist.castling[s][1];
			canCastleZ[s][1] = true;
		}
		
		//Update zobrist key
		zobrist.key ^= zobrist.piece[4][s][to64(mT)-1]; 	   //Remove king
		zobrist.key ^= zobrist.piece[4][s][to64(mF)-1]; 	   //Place king
		zobrist.key ^= zobrist.piece[0][s][to64(mF+cExtras[0])-1]; //Remove rook
		zobrist.key ^= zobrist.piece[0][s][to64(mF+cExtras[1])-1]; //Place rook
	
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

	zobrist.key ^= zobrist.side;
	//Remove previous movesMade and moveInfo, decrease ply
	movesMade.pop_back();
	moveInfo.pop_back();
	ply--;
}
