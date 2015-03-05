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

int nodes;

int think(Board& b, int depth) {
	using std::vector;

	const clock_t begin_time = clock();
	int bestMoveIndex;
	int bestMove, temp;
	vector<int> moveList;
	LINE prinVarLine;

	b.generateMoveListFor(b.getSide(), moveList);
	b.checkCheckForBot(b.getSide(), moveList);
	b.orderMoveList(b.getSide(), moveList);
	
	for (int i = depth; i <= depth; i++) {
		bestMove = search(b, i, moveList, bestMoveIndex, &prinVarLine);
		temp = moveList[0];
		moveList[0] = bestMove;
		moveList[bestMoveIndex] = temp;
	}
	
	std::cout << "\nThink time elapsed: " << float(clock()-begin_time) / CLOCKS_PER_SEC << '\n';
	for (int i = 0; i < prinVarLine.count; i++) {
		std::cout << intToSquare(prinVarLine.move[i]/100) << " to ";
		std::cout << intToSquare(prinVarLine.move[i]%100) << "\n";

	}
	return bestMove;
}

int search(Board& b, int depth, std::vector<int>& moveList, int& bestMoveIndex, LINE* pline) {
	using std::vector;	

	nodes = 0;
	const clock_t begin_time = clock();
	int bestMove = 0, bestScore = -99999, score, mF, mT;
	int alpha = -99999, beta = 99999;

	LINE line;

	std::cout << "Current eval: " << b.eval() << '\n';

	for (int i = 0; i < (int)moveList.size(); i++) {
		mF = moveList[i]/100;
		mT = moveList[i]%100;
		b.setMove(mF, mT);
		b.movePiece();

		score = -alphaBeta(b, alpha, beta, depth-1, &line);

		if (score > bestScore) {
			bestScore = score;
			bestMove = mF*100 + mT;
			bestMoveIndex = i;

			pline->move[0] = bestMove;
			memcpy(pline->move + 1, line.move, line.count * sizeof(int));
			pline->count = line.count + 1;
		}
		b.unmovePiece();
		b.changeTurn();
		nodes++;
	}

	std::cout << "Best score : " << bestScore << " Best move : " << bestMove << '\n';
	std::cout << "Time elapsed: " << float(clock()-begin_time) / CLOCKS_PER_SEC << '\n';
	std::cout << "Nodes searched: " << nodes << '\n';
	std::cout << "Nodes/sec: " << nodes / (float(clock()-begin_time) / CLOCKS_PER_SEC) << '\n';

	if (bestScore < -9999) std::cout << "BESTSCORE < -9999\n";
	if (bestScore < -9999) bestMove = 0;
	if (bestMove == 0) std::cout << "BESTMOVE: 0\n";
	return bestMove;
}

int alphaBeta(Board& b, int alpha, int beta, int depthLeft, LINE* pline) {
	using std::vector;

	int score;
	int mF, mT;
	vector<int> moveList;

	LINE line;

	b.changeTurn();
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
//		std::cout << "Scoring... " << b.eval() << '\n';
		pline->count = 0;
		return b.eval();
	}

	for (int i = 0; i < (int)moveList.size(); i++) {
		vector<int> localPV;
		nodes++;

		mF = moveList[i]/100;
		mT = moveList[i]%100;
		b.setMove(mF, mT);
		b.movePiece();
		
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
