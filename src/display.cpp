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
#include "common.h"
#include "sdl.h"
#include "board.h"
#include "ltexture.h"
#include "button.h"
#include "display.h"

SDL_Rect spriteClips[12], buttonClips[6], titleTextClips[27];
Button buttons[26];
LTexture spriteSheetTexture, buttonTexture, titleTexture, titleTextTexture;
LTexture turnText, checkText, moveText, rankText, fileText;
SDL_Color textColor;

void displayBoard(Board& b, const int& mF, const int& mT) {
	using std::string;

	static bool sidey = !b.getSide(); //Keeps track of if text needs to be updated
	static bool flag = false; //Used to load fileText only once
	string rankStr;
	string fileStr = "a         b        c         d         e         f         g         h";

	//Clear screen
	SDL_SetRenderDrawColor(renderer, 209, 224, 255, 255);
	SDL_RenderClear(renderer);

	b.setPiecesOnSquares();	//Update piece positions
	drawSquares(b, mF, mT);	//Draw the squares
	drawPieces(b, mF, mT);	//Draw pieces on squares
	drawBorder();		//Draw border around board

	drawButtons(b);		//Draw buttons (undo, restart)
	updateText(b, sidey);	//Updates text if someone moved
	if (start)
		drawMoveTable(b);	//Draw movetable (with text)
	else	
		drawTitleScreen(b);	//Draw title screen

	//Draw rank numbers
	for (int i = '8'; i >= '1'; i--) {
		rankStr = i;
		rankText.loadFromRenderedText(rankStr, textColor, Cicero26);
		rankText.render(BXSTART-35, BYSTART+30+75*('8'-i));
	}
	if (!flag) {	//Only load file text once
		fileText.loadFromRenderedText(fileStr, textColor, Cicero26); //Load file text
		flag = true;
	}
	//Render all the rest of the text
	checkText.render(BXSTART+B_SIZE-200, BYSTART+B_SIZE+40);
	fileText.render(BXSTART+33, BYSTART+B_SIZE+10);

	//Update screen
	SDL_RenderPresent(renderer);
}

void setButtons() {
	for (int i = 0; i < 26; i++)
		buttons[i].setButt(i);
	//Restart and undo
	buttons[0].setPos(1185, 25);
	buttons[0].setSize(51, 31);
	buttons[1].setPos(buttons[0].getX(), buttons[0].getY()+50);
	buttons[1].setSize(51, 31);
	//Human, computer (white and black)
	for (int i = 2; i < 6; i++) {
		buttons[i].setPos(BXSTART+B_SIZE+125+((i-2)%2)*200,
			          BYSTART+250+(i/4)*150);
		buttons[i].setSize(titleTextClips[i].w, titleTextClips[i].h);
	}
	//1-9 (white and black)
	for (int i = 6; i < 24; i++) {
		buttons[i].setPos(BXSTART+B_SIZE+280+(i-6)%9*titleTextClips[i].w,
				  BYSTART+300+(i/15)*150);
		buttons[i].setSize(titleTextClips[i].w, titleTextClips[i].h);
	}
	//Flip board
	buttons[24].setPos(BXSTART+B_SIZE+200, BYSTART+500);
	buttons[24].setSize(titleTextClips[24].w, titleTextClips[24].h);
	//Start, and light version
	buttons[25].setPos(BXSTART+B_SIZE+230, BYSTART+590);
	buttons[25].setSize(titleTextClips[25].w, titleTextClips[25].h);
}

void setSpriteClips() {
	for (int i = 0; i < 12; i++) {
		spriteClips[i].x = i%6*SQ_SIZE;
		spriteClips[i].y = i/6*SQ_SIZE;
		spriteClips[i].w = SQ_SIZE;
		spriteClips[i].h = SQ_SIZE;
	}
	for (int i = 0; i < 6; i++) {
		buttonClips[i].x = i%2*51;
		buttonClips[i].y = i/2*31;
		buttonClips[i].w = 51;
		buttonClips[i].h = 31;
	}
	//"White:", "Black:"
	for (int i = 0; i < 2; i++) {
		titleTextClips[i].x = 0+i*100;
		titleTextClips[i].y = 0;
		titleTextClips[i].w = 100;
		titleTextClips[i].h = 40;
	}
	//"Human", "Computer", and light versions
	for (int i = 2; i < 6; i++) {
		titleTextClips[i].x = 0+(i%2)*100;
		titleTextClips[i].y = 45+(i/4)*40;
		titleTextClips[i].w = 100+(i%2)*40;
		titleTextClips[i].h = 40;
	}
	//1-9, and light versions
	for (int i = 6; i < 24; i++) {
		titleTextClips[i].x = 0+25*((i-6)%9);
		titleTextClips[i].y = 130 + 33*(i/15);
		titleTextClips[i].w = 25;
		titleTextClips[i].h = 30;
	}
	//"Flip board"
	titleTextClips[24].x = 0;
	titleTextClips[24].y = 200;
	titleTextClips[24].w = 150;
	titleTextClips[24].h = 50;
	//"Flip board" light
	titleTextClips[27].x = 0;
	titleTextClips[27].y = 294;
	titleTextClips[27].w = 150;
	titleTextClips[27].h = 50;

	//"Start"
	titleTextClips[25].x = 10;
	titleTextClips[25].y = 250;
	titleTextClips[25].w = 90;
	titleTextClips[25].h = 40;
	//"Start" light
	titleTextClips[26].x = 125;
	titleTextClips[26].y = 250;
	titleTextClips[26].w = 90;
	titleTextClips[26].h = 40;
}

void displayBotText(const Board& b) {
	std::string botStr;

	botStr = b.getSide() ? "White" : "Black";
	botStr += " is thinking..";
	turnText.loadFromRenderedText(botStr, textColor, Garamond26);
	turnText.render(BXSTART, BYSTART+B_SIZE+40);
	SDL_RenderPresent(renderer);
}

void updateText(const Board& b, bool& sidey) {
	std::string checkStr = " ";
	if (sidey != b.getSide() || b.getNumMovesMade() == 0) {
		sidey = b.getSide();
		if (b.getSideInCheck()) { //Load check text
			if (b.getSideInCheckmate() == 1)
				checkStr = "Black wins!";
			else if (b.getSideInCheckmate() == 2) 
				checkStr = "White wins!";
			else if (b.getSideInCheck() == 1)
				checkStr = "White is in check";
			else if (b.getSideInCheck() == 2)
				checkStr = "Black is in check";
		}
		checkText.loadFromRenderedText(checkStr, textColor, Garamond26);
	}
}

void drawButtons(const Board& b) {
	SDL_Rect clipSq;
	for (int i = 0; i < 2; i++) {
		if (buttons[i].getInside())
			clipSq = buttons[i].getClicking() ? buttonClips[i+4] : buttonClips[i+2];
		else
			clipSq = buttonClips[i];
		buttonTexture.render(buttons[i].getX(), buttons[i].getY(), &clipSq);
	}
}

void drawBorder() {
	SDL_Rect borderRect = {BXSTART, BYSTART, B_SIZE, B_SIZE};
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderDrawRect(renderer, &borderRect);
	borderRect = {BXSTART-1, BYSTART-1, B_SIZE+2, B_SIZE+2};
	SDL_RenderDrawRect(renderer, &borderRect);
}

void drawSquares(const Board& b, const int& mF, const int& mT) {
	int sq;
	SDL_Rect sqPos;
	for (int r = 1; r <= 8; r++) {
		for (int f = 1; f <= 8; f++) {
			sq = FR2SQ64(f, r)-1;
			//If this square is clicked, or has had a move made on it (moveFrom)
			if (mF == sq+1 || to64(b.getLastMove()/100) == sq+1)
				SDL_SetRenderDrawColor(renderer, 248, 195, 248, 255);
			//If this square is clicked, or has had a move made on it (moveTo)
			else if (mT == sq+1 || to64(b.getLastMove()%100) == sq+1)
				SDL_SetRenderDrawColor(renderer, 238, 157, 242, 255);
			//Otherwise, color light
			else if ((r+f)%2 == 1)			
				SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
			//or color dark
			else 						
				SDL_SetRenderDrawColor(renderer, 0, 153, 153, 255);
			sqPos = {b.squares[sq].getX(),	//X start
				 b.squares[sq].getY(),	//Y start
				 SQ_SIZE, SQ_SIZE};	//Width, height of square
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
				if (b.piece[b[from64(sq+1)]].getValue() == P_VAL)
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
				if (b.piece[b[from64(sq+1)]].getValue() == P_VAL)
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

	static int extra = 0;
	//Add "#. ... " if loaded from a FEN and black to move
	if (b.getNumMovesMade() == 0) {
		if (b.getSide() == BLACK) {
			plyStr = std::to_string((b.getPly()-1)/2+1) + ". ";
			plyStr += " ... ";
			plyStrings.push_back(plyStr);
			extra = 1;
		}
		else extra = 0;
	}

	while ((int)plyStrings.size() > b.getNumMovesMade() + extra) 
		plyStrings.pop_back();

	int lastMove = b.getNumMovesMade()-1+extra;

	//If a new move has been made
	if ((int)plyStrings.size() < b.getNumMovesMade() + extra) {
		mF2 = b.getLastMove()/100;
		mT2 = b.getLastMove()%100;
		if ((b.getPly()-1)%2 == 0)	//Add number in front for white's moves
			plyStr = std::to_string((b.getPly()-1)/2+1) + ". ";
		p = b.getPieceMoved(lastMove);
		if (b.piece[p].getValue() == Q_VAL && b.getPmSq(lastMove) != mT2)
			plyStr += "Q";
		else if (b.piece[p].getValue() == K_VAL) {
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
		else if (b.piece[p].getValue() == R_VAL) {
			plyStr += "R";
			//Check same side and same piecetype, for ambiguous moves
			otherPiece = !b.getSide() ? (p == 0 ? 7 : 0) : (p == 16 ? 23 : 16);
			int oPos = b.piece[otherPiece].getPos(), e = 0, small, big;

			if (b.piece[otherPiece].getAlive())
				if (b.validateHozMove(oPos, mT2)) {
					dupMove = true;
					//Make sure we're not needlessly disambiguating
					if (oPos%10 == mT2%10)	//Same file
						e = 10;
					else if (oPos/10 == mT2/10) //Same rank
						e = 1;
					//Loop through the squares between otherPiece
					//and moveTo, and if any of them are moveFrom,
					//it's not really an ambiguous move
					if (e) {
						big = oPos > mT2 ? oPos : mT2;
						small = oPos < mT2 ? oPos : mT2;
						for (int i = small+e; i < big; i += e)
							if (i == mF2) dupMove = false;
					}
				}
		}
		else if (b.piece[p].getValue() == B_VAL) 
			plyStr += "B";
		else if (b.piece[p].getValue() == N_VAL) { 
			plyStr += "N";
			//Check same side and same piecetype, for ambiguous moves
			otherPiece = !b.getSide() ? (p == 1 ? 6 : 1) : (p == 17 ? 22 : 17);
			if (b.piece[otherPiece].getAlive())
				if (b.validateKnightMove(b.piece[otherPiece].getPos(), mT2))
					dupMove = true;
		}
		if (dupMove) { //If the move was ambiguous, de-ambiguate
			if (mF2%10 != b.piece[otherPiece].getPos()%10) //Not same file
				plyStr += mF2%10+'a'-1; //so, file is sufficient
			else 					   //Same file
				plyStr += mF2/10+'1'-2; //so, rank is sufficient
		}
		if (b.getPrevOnMoveTo(lastMove) != empty) { //If move was a capture
			//Special case for pawns, display the file of departure
			if (b.piece[p].getValue() == P_VAL || 
			    b.getPmSq(b.getNumMovesMade()-1+extra) == mT2)
				plyStr += mF2%10+'a'-1;
			plyStr += "x";
		}
		else if (b.piece[p].getValue() == P_VAL)
			if (abs(mF2 - mT2) == 9 || abs(mF2 - mT2) == 11) { //En passant
				plyStr += mF2%10+'a'-1;
				plyStr += 'x';
			}

		if (!castling)	//Add moveTo 
			plyStr += intToSquare(mT2);

		if (b.getPmSq(lastMove) == mT2) //If it was a promotion
			plyStr += "=Q";

		if (b.getSideInCheck())
			plyStr += b.getSideInCheckmate() ? '#' : '+';

		plyStrings.push_back(plyStr);	    //Add it to the list of ply-moves
	}
	for (size_t i = 0; i < plyStrings.size(); i+=2) { //Loop through moves
		plyStr = plyStrings[i];	   		    //Load white move
		if (i+1 < plyStrings.size())		    //If black has moved,
			plyStr += " " + plyStrings[i+1];    //load their move, too
		moveText.loadFromRenderedText(plyStr, textColor, Cicero22);
		moveText.render(BXSTART+(i/42*180)+B_SIZE+40, 
				BYSTART+10+((i/2)%21)*30); 
	}
}

void drawTitleScreen(const Board& b) {
	SDL_Rect clipSq;

	//Draw the border and background
	SDL_Rect borderRect = {BXSTART+B_SIZE+25, BYSTART, 500, 650};
	SDL_SetRenderDrawColor(renderer, 236, 247, 235, 255);
	SDL_RenderFillRect(renderer, &borderRect);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderDrawRect(renderer, &borderRect);
	borderRect = {BXSTART+B_SIZE+24, BYSTART-1, 500, 650};
	SDL_RenderDrawRect(renderer, &borderRect);

	titleTexture.render(BXSTART+B_SIZE+75, BYSTART);

	//Draw white options
	//"White:"
	clipSq = titleTextClips[0];
	titleTextTexture.render(BXSTART+B_SIZE+230, BYSTART+200, &clipSq);
	//"Human"
	if (buttons[2].getInside() || !b.getWhiteIsBot())
		clipSq = titleTextClips[4];
	else
		clipSq = titleTextClips[2];
	titleTextTexture.render(buttons[2].getX(), buttons[2].getY(), &clipSq);
	//"Computer"
	if (buttons[3].getInside() || b.getWhiteIsBot())
		clipSq = titleTextClips[5];
	else
		clipSq = titleTextClips[3];
	titleTextTexture.render(buttons[3].getX(), buttons[3].getY(), &clipSq);
	//Numbers 1-9
	if (b.getWhiteIsBot()) {
		for (int i = 1; i < 10; i++) {
			if (buttons[i+5].getInside() || b.whiteBot.getLevel() == i)
				clipSq = titleTextClips[i+14];
			else
				clipSq = titleTextClips[i+5];
			titleTextTexture.render(buttons[i+5].getX(), 
						buttons[i+5].getY(), &clipSq);
		}
	}

	//Draw black options
	//"Black:"
	clipSq = titleTextClips[1];
	titleTextTexture.render(BXSTART+B_SIZE+230, BYSTART+350, &clipSq);
	//"Human"
	if (buttons[4].getInside() || !b.getBlackIsBot())
		clipSq = titleTextClips[4];
	else
		clipSq = titleTextClips[2];
	titleTextTexture.render(buttons[4].getX(), buttons[4].getY(), &clipSq);
	//"Computer"
	if (buttons[5].getInside() || b.getBlackIsBot())
		clipSq = titleTextClips[5];
	else
		clipSq = titleTextClips[3];
	titleTextTexture.render(buttons[5].getX(), buttons[5].getY(), &clipSq);
	//Numbers 1-9
	if (b.getBlackIsBot()) {
		for (int i = 1; i < 10; i++) {
			if (buttons[i+14].getInside() || b.blackBot.getLevel() == i)
				clipSq = titleTextClips[i+14];
			else
				clipSq = titleTextClips[i+5];
			titleTextTexture.render(buttons[i+14].getX(), 
						buttons[i+14].getY(), &clipSq);
		}
	}

	//"Flip board"
	if (buttons[24].getInside())
		clipSq = titleTextClips[27];
	else
		clipSq = titleTextClips[24];
	titleTextTexture.render(buttons[24].getX(), buttons[24].getY(), &clipSq);

	//"Start"
	if (buttons[25].getInside())
		clipSq = titleTextClips[26];
	else
		clipSq = titleTextClips[25];
	titleTextTexture.render(buttons[25].getX(), buttons[25].getY(), &clipSq);

}
