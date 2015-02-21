/*
----------------------------------
	~Moosey Chess Engine~
	      display.h
----------------------------------
*/

#ifndef DISPLAY_H
#define DISPLAY_H

class Board;
class Square;

const int B_SIZE = 600;
const int SQ_SIZE = 75;
const int BXSTART = 50;
const int BYSTART = 25;

void displayBoard(Board& b, const int& mF = -1, const int& mT = -1);
void setPiecesOnSquares(const Board& b);
void setSquarePositions();
void setSpriteClips();
void drawSquares(const Board& b, const int& mF = -1, const int& mT = -1);
void drawPieces(const Board& b, const int& mF = -1, const int& mT = -1);
void drawBorder();
void drawMoveTable(const Board&);

extern Square squares[64];

#endif
