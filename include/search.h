/*
----------------------------------
	~Moosey Chess Engine~
	      search.h
----------------------------------
*/

#ifndef SEARCH_H
#define SEARCH_H

#include <vector>

class Board;

struct LINE {
	int count;
	int move[20];
};

int think(Board& b, int depth);
int alphaBeta(Board& b, int alpha, int beta, int depthLeft, int depthGone, LINE* pline, bool allowNull, int ext);
int quies(Board& b, int alpha, int beta, int depthGone);

#endif
