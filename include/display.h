#ifndef DISPLAY_H
#define DISPLAY_H

class Board;
class Square;

const int B_SIZE = 600;
const int SQ_SIZE = 75;
const int BXSTART = 50;
const int BYSTART = 50;

void displayBoard(Board& b);
void setPiecesOnSquares(Board& b);
void setSquarePositions();
void setSpriteClips();
void drawSquares(Board& b);
void drawBorder();

extern Square squares[64];

#endif
