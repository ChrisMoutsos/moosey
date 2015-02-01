#ifndef DISPLAY_H
#define DISPLAY_H

class Board;

void displayBoard(const Board&);
void printRow(const Board&, bool&, int);
void emptyRow(bool&, int&);

#endif
