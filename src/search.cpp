/*
----------------------------------
	~Moosey Chess Engine~
	      search.cpp
----------------------------------
*/

#include <iostream>
#include <cstring>
#include <chrono>
#include "common.h"
#include "search.h"
#include "board.h"
#include "display.h"

int nodes, qNodes, r = 2;
LINE prinVarLine, oldPrinVarLine;
double totalTimeW = 0, totalTimeB = 0;

int think(Board& b, int depth) {
	using std::vector;

	auto beginTime1 = std::chrono::high_resolution_clock::now();
	typedef std::chrono::duration<float> fsec;

	int bestScore = 0;
	int alpha = -99999, beta = 99999;
	int asp = 100;
	vector<int> moveList;

	b.genOrderedMoveList(b.getSide(), moveList);
	b.checkCheck(b.getSide(), moveList);
	
	for (int i = 1; i <= depth; i++) {
		//Age HH tables
		for (int f = 0; f < 64; f++)
			for (int t = 0; t < 64; t++)
				b.hh[b.getSide()][f][t] /= 2;

		oldPrinVarLine = prinVarLine;

		auto beginTime2 = std::chrono::high_resolution_clock::now();
		nodes = 0;
		qNodes = 0;
		
		alpha = bestScore - asp;
		beta = bestScore + asp;
		for (;;) {
			bestScore = alphaBeta(b, alpha, beta, i, 0, &prinVarLine, 1);
			if (bestScore <= alpha)  {
				std::cout << "FAIL LOW DEPTH " << i << "\n";
				alpha = -99999;
			}
			else if (bestScore >= beta) { 
				std::cout << "FAIL HIGH DEPTH " << i << "\n";
				beta = 99999;
			}
			else break;
		}

		if (i == depth) {
			std::cout << "Search to ply " << i << "...\n";
			std::cout << "Main nodes searched: " << nodes << '\n';
			std::cout << "Quies nodes searched: " << qNodes << '\n';
			std::cout << "Time elapsed: ";
			auto endTime1= std::chrono::high_resolution_clock::now();
			fsec diff1 = endTime1 - beginTime2;
			std::cout << diff1.count() << '\n';
			std::cout << "Nodes / sec: ";
			std::cout << (nodes+qNodes) / diff1.count() << '\n';
			std::cout << "Best score: " << bestScore << "\n\n";
		}
	}
	for (int i = 0; i < prinVarLine.count; i++) {
		std::cout << intToSquare(prinVarLine.move[i]/100) << " to ";
		std::cout << intToSquare(prinVarLine.move[i]%100) << "\n";
	}
	
	auto endTime2 = std::chrono::high_resolution_clock::now();
	fsec diff2 = endTime2 - beginTime1;
	std::cout << "Total time elapsed: " << diff2.count() << '\n';
	if (b.getSide())
		totalTimeW += diff2.count();
	else
		totalTimeB += diff2.count();

	std::cout << "Total time taken so far by ";
	if (b.getSide()) std::cout << " White: " << totalTimeW << '\n';
	else std::cout << "Black: " << totalTimeB << '\n';

	std::cout << "Best move: " << prinVarLine.move[0] << "\n\n";
	return prinVarLine.move[0];
}

int alphaBeta(Board& b, int alpha, int beta, int depthLeft, int depthGone, LINE* pline, bool allowNull) {
	using std::vector;

	LINE line;
	bool s = b.getSide();

	//If our king is dead, return bad score
	if (s && !b.piece[wK].getAlive()) {
		return -9999 + depthGone-1;
	}
	else if (!s && !b.piece[bK].getAlive()) {
		return -9999 + depthGone-1;
	}

	//Terminal nodes
	if (depthLeft == 0) {
		//If we're in check, search a little further
		if (b.inCheck(s))
			return alphaBeta(b, alpha, beta, 1, depthGone, pline, 0); 
		//Otherwise, do a quiescence search
		pline->count = 0;
		//std::cout << "alphabeta callin quies\n";
		return quies(b, alpha, beta, depthGone, pline);
	}

	int score;
	
	if (b.inCheck(s)) {
		if (s) b.setSideInCheck(1);
		else b.setSideInCheck(2);
	}
	else b.setSideInCheck(0);

	//Null move
	if (allowNull && depthLeft > r) {
		if (!((s && b.getSideInCheck() == 1) || (!s && b.getSideInCheck() == 2))) {
			b.changeTurn();
			score = -alphaBeta(b, -beta, -beta+1, depthLeft-1-r, depthGone-1-r, pline, 0);
			b.changeTurn();
			if (score >= beta) 
				return score;
		}
	}

	bool foundPV = false;
	int mF, mT;
	vector<int> moveList;

	b.genOrderedMoveList(s, moveList);

	//If we are in checkmate
	if ((s && b.getSideInCheck() == 1) || (!s && b.getSideInCheck() == 2)) {
		b.cleanMoveList(s, moveList);
		if ((int)moveList.size() == 0) {
			pline->count = 0;
			return -9999 + depthGone-1;
		}
	}

/*
	if (b.checkCheck(s, moveList)) {
		pline->count = 0;
		return -9999 + depthGone-1;
	}
*/
	//Put principal variation first
	int temp;
	for (int i = 0; i < (int)moveList.size(); i++) {
		if (moveList[i] == oldPrinVarLine.move[depthGone] && allowNull) {
			temp = moveList[i];
			moveList.erase(moveList.begin()+i);
			moveList.insert(moveList.begin()+0, temp);
		}
	}	


	for (int i = 0; i < (int)moveList.size(); i++) {
		vector<int> localPV;
		nodes++;

		mF = moveList[i]/100;
		mT = moveList[i]%100;
		b.setMove(mF, mT);
		b.movePiece();
		b.changeTurn();
		//If we had an alpha cutoff, do a zero-window search (guessing we were right)
		if (foundPV) {
			score = -alphaBeta(b, -alpha-1, -alpha, depthLeft-1, depthGone+1, &line, 1);
			if ((score > alpha) && (score < beta)) //If we were wrong
				score = -alphaBeta(b, -beta, -alpha, depthLeft-1, depthGone+1, &line, 1);
		}	
		else
			score = -alphaBeta(b, -beta, -alpha, depthLeft-1, depthGone+1, &line, 1);

		b.unmovePiece();
		b.changeTurn();

		if (score >= beta) {
			//If it wasn't a capture, add it to the HH table
			if (b.getBoard120(mT) == empty)
				b.hh[s][to64(mF)-1][to64(mT)-1] += depthGone*depthGone;
			return beta;
		}
		if (score > alpha) {
			alpha = score;
			foundPV = true;
			//Add it to the principal variation
			pline->move[0] = mF*100 + mT;
			memcpy(pline->move + 1, line.move, line.count * sizeof(int));
			pline->count = line.count + 1;
		}
	}
	
	return alpha;
}

int quies(Board& b, int alpha, int beta, int depthGone, LINE* pline) {
	using std::vector;

	vector<int> captureList;
	bool s = b.getSide();

	//if (b.inCheck(b.getSide()))
	//	return alphaBeta(b, alpha, beta, 1, depthGone+1, pline, 1); 

	if ((s && !b.piece[wK].getAlive()) || (!s && !b.piece[bK].getAlive()))
		return -9999 + depthGone-1;

	int score = b.eval();
	int mF, mT;
	
	if (score >= beta)
		return beta;
	if (score > alpha)
		alpha = score;

	b.generatePieceMoveLists(s);
	b.getCaptures(s, captureList);

	if ((int)captureList.size() == 0)
		return score;
	
	b.sortCaptures(captureList);

	for (int i = 0; i < (int)captureList.size(); i++) {
		qNodes++;

		mF = captureList[i]/100;
		mT = captureList[i]%100;
		b.setMove(mF, mT);
		b.movePiece();
		if (b.inCheck(s)) {
			b.unmovePiece();
			continue;
		}
		b.changeTurn();
		score = -quies(b, -beta, -alpha, depthGone+1, pline);
		b.unmovePiece();
		b.changeTurn();
		
		if (score >= beta)
			return beta;
		if (score > alpha) {
			alpha = score;
		}
	}
	
	return alpha;
}
