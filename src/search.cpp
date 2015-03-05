/*
----------------------------------
	~Moosey Chess Engine~
	      search.cpp
----------------------------------
*/

#include <ctime>
#include <cstring>
#include "search.h"
#include "board.h"
#include <iostream>
#include "display.h"

int nodes, terminalNodes;

int think(Board& b, int depth) {
	using std::vector;

	const clock_t begin_time = clock();
	int bestMoveIndex;
	int bestMove, temp;
	int bestScore;
	int alpha = -99999, beta = 99999;
	vector<int> moveList;
	LINE prinVarLine;

	b.generateMoveListFor(b.getSide(), moveList);
	b.checkCheckForBot(b.getSide(), moveList);
	b.orderMoveList(b.getSide(), moveList);
	
	for (int i = depth; i <= depth; i++) {
		nodes = 0;
		terminalNodes = 0;
		bestScore = alphaBeta(b, alpha, beta, depth, &prinVarLine);
		std::cout << "Total nodes searched: " << nodes << '\n';
		std::cout << "Terminal nodes searched: " << terminalNodes << '\n';
		std::cout << "Time elapsed: " << float(clock()-begin_time) << '\n';
		std::cout << "Nodes / sec: " << nodes / (float(clock()-begin_time) / CLOCKS_PER_SEC) << '\n';
	}
	
	std::cout << "\nThink time elapsed: " << float(clock()-begin_time) / CLOCKS_PER_SEC << '\n';
	for (int i = 0; i < prinVarLine.count; i++) {
		std::cout << intToSquare(prinVarLine.move[i]/100) << " to ";
		std::cout << intToSquare(prinVarLine.move[i]%100) << "\n";

	}
	return prinVarLine.move[0];
}

int alphaBeta(Board& b, int alpha, int beta, int depthLeft, LINE* pline) {
	using std::vector;

	int score;
	int mF, mT;
	vector<int> moveList;

	LINE line;

	if (depthLeft) {
		b.generateMoveListFor(b.getSide(), moveList);
		b.checkCheckForBot(b.getSide(), moveList);
	//	if (depthLeft > 2)
	//		b.orderByScore(moveList);
	//	else
			if (depthLeft > 0)
				b.orderMoveList(b.getSide(), moveList);
	}

	if (depthLeft == 0) {
		pline->count = 0;
		terminalNodes++;
		nodes++;
		return b.eval();
	}

	for (int i = 0; i < (int)moveList.size(); i++) {
		vector<int> localPV;
		nodes++;

		mF = moveList[i]/100;
		mT = moveList[i]%100;
		b.setMove(mF, mT);
		b.movePiece();
		b.changeTurn();
		
		score = -alphaBeta(b, -beta, -alpha, depthLeft - 1, &line);
		b.changeTurn();

		if (score >= beta) {
			b.unmovePiece();
			return beta;
		}
		if (score > alpha) {
			alpha = score;
			pline->move[0] = mF*100 + mT;
			memcpy(pline->move + 1, line.move, line.count * sizeof(int));
			pline->count = line.count + 1;
		}

		b.unmovePiece();

	}
	
	return alpha;
}
