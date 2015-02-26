/*
----------------------------------
	~Moosey Chess Engine~
	      display.cpp
----------------------------------
*/

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <string>
#include <vector>
#include "sdl.h"
#include "display.h"
#include "board.h"
#include "ltexture.h"

SDL_Rect spriteClips[12];
LTexture spriteSheetTexture;
LTexture turnText, checkText, moveText, rankText, fileText;
SDL_Color textColor;
void showMoveLists(Board& board);
void showPieceMoveLists(Board& board);

void displayBoard(Board& b, const int& mF, const int& mT) {
	using std::string;

	static bool sidey = !b.getSide();
	static string rankStr;
	string fileStr = "a         b        c         d         e         f         g         h";
	string temp = "";

	//Clear screen
	SDL_SetRenderDrawColor(renderer, 209, 224, 255, 255);
	SDL_RenderClear(renderer);

	setPiecesOnSquares(b);	//Update piece positions
	drawSquares(b, mF, mT);	//Draw the squares
	drawPieces(b, mF, mT);	//Draw pieces on squares
	drawBorder();		//Draw border around board

	//If someone made a move, update the important text
	if (sidey != b.getSide()) {
		sidey = b.getSide();
		if (b.getSide()) 
			turnText.loadFromRenderedText("White to move", textColor, Garamond26);
		else
			turnText.loadFromRenderedText("Black to move", textColor, Garamond26);
		fileText.loadFromRenderedText(fileStr, textColor, Cicero26);
		
	//	showPieceMoveLists(b);
	}

	drawMoveTable(b);	//Draw movetable

	//Draw rank numbers
	for (int i = int('8'); i >= int('1'); i--) {
		rankStr = char(i);
		rankText.loadFromRenderedText(rankStr, textColor, Cicero26);
		rankText.render(BXSTART-35, BYSTART+30+75*(int('8')-i));
	}
	//Render all the rest of the text
	turnText.render(BXSTART, BYSTART+B_SIZE+40);
	checkText.render(BXSTART+B_SIZE-200, BYSTART+B_SIZE+40);
	fileText.render(BXSTART+33, BYSTART+B_SIZE+10);

	//Update screen
	SDL_RenderPresent(renderer);
}

void setPiecesOnSquares(Board& b) {
	for (int i = 0; i < 64; i++)
		b.squares[i].setPiece(b.getBoard120(from64(i+1)));
}

void setSquarePositions(Board& b) {
	for (int i = 0; i < 64; i++) {
		b.squares[i].setPos(BXSTART+(SQ_SIZE*(i%8)),
				  BYSTART+B_SIZE-(SQ_SIZE*(i/8+1)));
		b.squares[i].setSq(i+1);
	}
}

void setSpriteClips() {
	for (int i = 0; i < 12; i++) {
		spriteClips[i].x = i%6*SQ_SIZE;
		spriteClips[i].y = i/6*SQ_SIZE;
		spriteClips[i].w = SQ_SIZE;
		spriteClips[i].h = SQ_SIZE;
	}
}

void drawSquares(const Board& b, const int& mF, const int& mT) {
	int sq;
	SDL_Rect sqPos;
	for (int r = 1; r <= 8; r++) {
		for (int f = 1; f <= 8; f++) {
			sq = FR2SQ64(f, r)-1;
			if (mF == sq+1 || to64(b.getMoveFrom()) == sq+1)	//moveFrom square
				SDL_SetRenderDrawColor(renderer, 248, 195, 248, 255);
			else if (mT == sq+1 || to64(b.getMoveTo()) == sq+1)	//moveTo square
				SDL_SetRenderDrawColor(renderer, 238, 157, 242, 255);
			else if ((r+f)%2 == 1)				//Light squares
				SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
			else 						//Dark squares
				SDL_SetRenderDrawColor(renderer, 0, 153, 153, 255);
			sqPos = {b.squares[sq].getX(),	//X start
				 b.squares[sq].getY(),	//Y start
				 SQ_SIZE, SQ_SIZE};	 //Width, height of square
			SDL_RenderFillRect(renderer, &sqPos);
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		}
	}
}

void drawPieces(const Board& b, const int& mF, const int& mT) {
	int p, sq, x, y, putOnTop = -1;
	SDL_Rect sqPos;
	SDL_Rect clipSq, pOTClipSq;
	for (int r = 1; r <= 8; r++) {
		for (int f = 1; f <= 8; f++) {
			sq = FR2SQ64(f, r)-1;
			sqPos = {b.squares[sq].getX(), 	//X start
				 b.squares[sq].getY(), 	//Y start
				 SQ_SIZE, SQ_SIZE};	//Width, height of square
		
			p = b.squares[sq].getPiece();
			if (p == wqR || p == wkR)
				clipSq = spriteClips[wRook];		
		    	else if (p == wqN || p == wkN)
				clipSq = spriteClips[wKnight];		
		    	else if (p == wqB || p == wkB)
				clipSq = spriteClips[wBishop];		
		    	else if (p == wQ)
				clipSq = spriteClips[wQueen];		
		    	else if (p == wK)
				clipSq = spriteClips[wKing];		
		    	else if (p >= wPa && p <= wPh) {
				if (b.getValue(b.getBoard120(from64(sq+1))) == P_VAL)
					clipSq = spriteClips[wPawn];
				else
					clipSq = spriteClips[wQueen];
			}		
			if (p == bqR || p == bkR)
				clipSq = spriteClips[bRook];		
		    	else if (p == bqN || p == bkN)
				clipSq = spriteClips[bKnight];		
		    	else if (p == bqB || p == bkB)
				clipSq = spriteClips[bBishop];		
		    	else if (p == bQ)
				clipSq = spriteClips[bQueen];		
		    	else if (p == bK)
				clipSq = spriteClips[bKing];		
		    	else if (p >= bPa && p <= bPh) {
				if (b.getValue(b.getBoard120(from64(sq+1))) == P_VAL)
					clipSq = spriteClips[bPawn];
				else
					clipSq = spriteClips[bQueen];
			}		

			if (p != empty) { 
				//Save piece being dragged, to rerender on top
				if (b.squares[sq].getDragging()) {
					putOnTop = sq;
					pOTClipSq = clipSq;
				}
				else	//Every other piece
					spriteSheetTexture.render(sqPos.x, sqPos.y, &clipSq);
			}
		}
	}
	if (putOnTop != -1) { //Rerender piece being dragged
		SDL_GetMouseState(&x, &y);
		if (x < BXSTART + SQ_SIZE/2) 
			x = BXSTART + SQ_SIZE/2;
		if (x > BXSTART + B_SIZE - SQ_SIZE/2) 
			x = BXSTART + B_SIZE - SQ_SIZE/2;
		if (y < BYSTART + SQ_SIZE/2) 
			y = BYSTART + SQ_SIZE/2;
		if (y > BYSTART + B_SIZE - SQ_SIZE/2) 
			y = BYSTART + B_SIZE - SQ_SIZE/2;
		spriteSheetTexture.render(x-SQ_SIZE/2, y-SQ_SIZE/2, &pOTClipSq);
	}
}

void drawBorder() {
	SDL_Rect borderRect = {BXSTART, BYSTART, B_SIZE, B_SIZE};
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderDrawRect(renderer, &borderRect);
	borderRect = {BXSTART-1, BYSTART-1, B_SIZE+2, B_SIZE+2};
	SDL_RenderDrawRect(renderer, &borderRect);
}

void drawMoveTable(const Board& b) {
	using std::string;
	using std::vector;
	static vector<string> plyStrings;
	string plyStr = "";
	int mF2, mT2, p, otherPiece;
	bool castling = false, dupMove = false;

	//Draw the border and background
	SDL_Rect borderRect = {BXSTART+B_SIZE+25, BYSTART, 500, 650};
	SDL_SetRenderDrawColor(renderer, 236, 247, 235, 255);
	SDL_RenderFillRect(renderer, &borderRect);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderDrawRect(renderer, &borderRect);
	borderRect = {BXSTART+B_SIZE+24, BYSTART-1, 500, 650};
	SDL_RenderDrawRect(renderer, &borderRect);

	if ((int)plyStrings.size() < b.getPly()) { //If a new move has been made
		mF2 = b.getMoveMade(b.getPly()-1)/100;
		mT2 = b.getMoveMade(b.getPly()-1)%100;	
		if ((b.getPly()-1)%2 == 0)	//Add number in front for white's moves
			plyStr = std::to_string((b.getPly()-1)/2+1) + ". ";
		p = b.getPieceMoved(b.getPly()-1);
		if (b.getValue(p) == Q_VAL)
			plyStr += "Q";
		else if (b.getValue(p) == K_VAL) {
			if (mF2 == _E1 && (mT2 == _B1 || mT2 == _G1)) { //White castled
				castling = true;
				plyStr += (mT2 == _G1) ? "0-0" : "0-0-0";
			}
			else if (mF2 == _E8 && (mT2 == _B8 || mT2 == _G8)) { //Black castled
				castling = true;
				plyStr += (mT2 == _G8) ? "0-0" : "0-0-0";
			}
			else
				plyStr += "K";
		}
		else if (b.getValue(p) == R_VAL) {
			plyStr += "R";
			//Check same side and same piecetype, for ambiguous moves
			otherPiece = !b.getSide() ? (p == 0 ? 7 : 0) : (p == 16 ? 23 : 16);
			if (b.getAlive(otherPiece))
				if (b.validateHozMove(b.getPos(otherPiece), mT2))
					dupMove = true;
		}
		else if (b.getValue(p) == B_VAL) 
			plyStr += "B";
		else if (b.getValue(p) == N_VAL) { 
			plyStr += "N";
			//Check same side and same piecetype, for ambiguous moves
			otherPiece = !b.getSide() ? (p == 1 ? 6 : 1) : (p == 17 ? 22 : 17);
			if (b.getAlive(otherPiece))
				if (b.validateKnightMove(b.getPos(otherPiece), mT2))
					dupMove = true;
		}
		if (dupMove) { //If the move was ambiguous, de-ambiguate
			if (mF2%10 != b.getPos(otherPiece)%10) //Not same file
				plyStr += char(mF2%10+int('a')-1); //so, file is sufficient
			else 					//Same file
				plyStr += char(mF2/10+int('1')-2); //so, rank is sufficient
		}
		if (b.getPrevOnMoveTo(b.getPly()-1) != empty) { //If move was a capture
			if (b.getValue(p) == P_VAL)	//Special case for pawns, e.g. "axb4"
				plyStr += char(mF2%10+int('a')-1);
			plyStr += "x";
		}
		else if (b.getValue(p) == P_VAL)	//Check for en passant
			if (abs(mF2 - mT2) == 9 || abs(mF2 - mT2) == 11) {
				plyStr += char(mF2%10+int('a')-1);
				plyStr += 'x';
			}

		if (!castling) 
			plyStr += intToSquare(mT2);
		if (b.getSideInCheck())
			plyStr += b.getSideInCheckmate() ? '#' : '+';

		plyStrings.push_back(plyStr); //Add it to the list of ply-moves
	}
	for (int i = 0; i < (int)plyStrings.size(); i+=2) { //Loop through moves
		plyStr = plyStrings[i];			//Load white move
		if (i+1 < (int)plyStrings.size())	//If black has moved,
			plyStr += " " + plyStrings[i+1];  //load their move, too
		moveText.loadFromRenderedText(plyStr, textColor, Cicero22);
		moveText.render(BXSTART+(i/42*150)+B_SIZE+40, 
				BYSTART+10+((i/2)%21)*30); 
	}
}

void showPieceMoveLists(Board &b) {
	int mF, mT;
	std::cout << "Piece movelists: " << std::endl;
	for (int i = wqR; i <= bPh; i++) {
		std::cout << b.getName(i) << ": ";
		for (int j = 0; j < b.piece[i].moveListSize; j++) {
			mF = b.getPos(i);
			mT = b.getFromPieceMoveList(i, j);
			if (mT != 0) {
				std::cout << mF << " to " << mT << ", ";
			}
		}
		std::cout << " ... \n";
	}
	std::cout << "\n\n";
}
