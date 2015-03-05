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

int search(Board& b, int depth);
int alphaBeta(Board& b, int alpha, int beta, int depthLeft, std::vector<int> & PV);

#endif
