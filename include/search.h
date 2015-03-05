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
int search(Board& b, int depth, std::vector<int>& moveList, int& bestMoveIndex, LINE* pline);
int alphaBeta(Board& b, int alpha, int beta, int depthLeft, LINE* pline);

#endif
