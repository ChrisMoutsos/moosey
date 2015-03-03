/*
----------------------------------
	~Moosey Chess Engine~
	     	search.h
----------------------------------
*/
#ifndef SEARCH_H
#define SEARCH_H

class Board;

int search(Board& b);
int alphaBeta(Board& b, int alpha, int beta, int depthLeft);

#endif
