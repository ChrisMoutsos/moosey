/*
----------------------------------
	~Moosey Chess Engine~
	       bot.cpp
----------------------------------
*/

#include <iostream>
#include <cstring>
#include <chrono>
#include <algorithm>
#include "common.h"
#include "bot.h"
#include "board.h"
#include "display.h"

Bot::Bot() {
	reset();
	level = 7;
}

void Bot::reset() {
	prinVarLine.count = 0;
	oldPrinVarLine.count = 0;
	nodes = qNodes = r = 0;
	totalTime = 0;
	//Clear HH table
	for (int f = 0; f < 64; f++)
			for (int t = 0; t < 64; t++)
				hh[f][t] = 0;
/*
	//Clear killer moves
	for (int i = 0; i < 30; i++) {
		killers[i][0] = 0;
		killers[i][1] = 0;
	}
*/
}

int Bot::think(Board& b, int depth) {
	using std::vector;

	//Save starting time
	auto beginTime1 = std::chrono::high_resolution_clock::now();
	typedef std::chrono::duration<float> fsec;

	int bestMoveSoFar = 0, bestScore = 0;
	int alpha = -99999, beta = 99999;
	int asp = 50;
	vector<int> moveList;

	//Reset everything if you restart the game
	if (b.getNumMovesMade() < 2)
		reset();	

	//Generate legal moveLists
	b.genOrderedMoveList(b.getSide(), moveList);
	b.checkCheck(b.getSide(), moveList);
	
	//Iterative deepening
	for (int i = 1; i <= depth; i++) {
		std::cout << "Search to ply " << i << "...\n";

		//Age HH tables
		for (int f = 0; f < 64; f++)
			for (int t = 0; t < 64; t++)
				hh[f][t] /= 2;
/*
		//Clear killer moves
		for (int i = 0; i < 30; i++) {
			killers[i][0] = 0;
			killers[i][1] = 0;
		}
*/

		oldPrinVarLine = prinVarLine;

		//Get start time of this specific search
		auto beginTime2 = std::chrono::high_resolution_clock::now();
		nodes = 0;
		qNodes = 0;

		//Set aspiration window
		alpha = bestScore - asp;
		beta = bestScore + asp;

		for (;;) {
			bestScore = alphaBeta(b, alpha, beta, i, 0, &prinVarLine, 1, 0);
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

		bestMoveSoFar = prinVarLine.move[0];

		auto endTime3 = std::chrono::high_resolution_clock::now();
		fsec diff3 = endTime3 - beginTime1;

		std::cout << "Main nodes: " << nodes << ", q-nodes: " << qNodes << ", total: " << nodes+qNodes << '\n';
		std::cout << "Took: ";
		auto endTime1 = std::chrono::high_resolution_clock::now();
		fsec diff1 = endTime1 - beginTime2;
		std::cout << diff1.count() << "s, ";
		std::cout << "nodes / sec: ";
		std::cout << (nodes+qNodes) / diff1.count() << '\n';
		std::cout << "Current score: " << b.eval() << ", best score: " << bestScore << ", move: " << bestMoveSoFar << '\n';;
		std::cout << "Total time taken: " << diff3.count() << "\n\n";

		//Don't search further if you can see checkmate
		if (bestScore >= 9000)
			break;

		if (i == depth && bestScore < 8000 && bestScore > -8000) {
			if (diff3.count() < 0 && prinVarLine.count > 1) {
				depth++;
				i = depth - 1;
				continue;
			}
		}
	}
	for (int i = 0; i < prinVarLine.count; i++) {
		std::cout << intToSquare(prinVarLine.move[i]/100) << " to ";
		std::cout << intToSquare(prinVarLine.move[i]%100) << ", ";
	}
	std::cout << '\n';
	
	auto endTime2 = std::chrono::high_resolution_clock::now();
	fsec diff2 = endTime2 - beginTime1;
	totalTime += diff2.count();
	std::cout << "Total time elapsed: " << totalTime << '\n';

	return prinVarLine.move[0];
}

int Bot::alphaBeta(Board& b, int alpha, int beta, int depthLeft, int depthGone, LINE* pline, bool allowNull, int ext) {
	while (SDL_PollEvent(&e)) {
		if (e.type == SDL_QUIT)
			exit(0);
	}

	using std::vector;

	bool s = b.getSide();

	//If our king is dead, return bad score
	if ((s && !b.piece[wK].getAlive()) || (!s && !b.piece[bK].getAlive())) {
		pline->count = 0;
		return -9999 + depthGone-1;
	}

	//Repetition detection
	if (allowNull && depthGone) {
		if (b.drawCheck()) {
			pline->count = 0;
			return 0;
		}
	}

	//Horizon nodes, quiescence search
	if (depthLeft <= 0) {
		//If we're in check, search a little further
		if (allowNull && b.inCheck(s))
			return alphaBeta(b, alpha, beta, 1, depthGone, pline, 0, 0); 

		//Otherwise, do a quiescence search
		pline->count = 0;
		return quies(b, alpha, beta, depthGone);
	}

	LINE line;

	//Useful to know this for later	
	bool inCheck = b.inCheck(s);

	int score;
	
	//Null move reduction
	if (allowNull && !inCheck) {
		if ((s && b.getWhiteMaterial() > ENDGAME_VAL) || (!s && b.getBlackMaterial() > ENDGAME_VAL)) {
			r = depthLeft > 6 ? 4 : 3;
			b.changeTurn();
			score = -alphaBeta(b, -beta, -beta+1, depthLeft-r-1, depthGone, pline, 0, 0);
			b.changeTurn();
			if (score >= beta) { //Fail-high
				depthLeft -= 4;
				ext = 0;
				if (depthLeft <= 0) {
					pline->count = 0;
					return quies(b, alpha, beta, depthGone);
				}
			}
			//Extend on nearby checkmates
			else if (score < -8500)
				ext += 50;
		}
	}

	bool foundPV = false;
	int mF, mT;
	vector<int> moveList;

	//Generate ordered legal moveList
	b.genOrderedMoveList(s, moveList);
	b.cleanMoveList(s, moveList);

	//Evading check extension
	if (inCheck)
		ext += 75;

	//No legal moves
	if (moveList.size() == 0) {
		pline->count = 0;
		//If we are in checkmate, return bad score
		if (inCheck)
			return -9999 + depthGone-1;
		//Only favor stalemate if we're losing
		else
			if (s)
				return b.getWhiteMaterial() > b.getBlackMaterial() ? -8000 : 8000;
			else 
				return b.getBlackMaterial() > b.getWhiteMaterial() ? -8000 : 8000;
	}
	//Singular reply
	else if (moveList.size() == 1) {
		//On a root node, quit early
		if (depthGone == 0) {
			pline->move[0] = moveList[0];
			pline->count = 1;
			return -8000;
		}	

		//Otherwise, extend
		ext += 50;
	}
	//Only two replies
	else if (moveList.size() == 2)
		ext += 25;

	//Frontier nodes: futility pruning
	if (depthLeft == 1) {
		if (!inCheck && !(abs(alpha) > 9000 || abs(beta) > 9000)) {
			if (b.eval() + B_VAL < alpha && moveList.size() > 0) {
				pline->count = 0;
				return quies(b, alpha, beta, depthGone);
			}
		}
	}
	//Pre-frontier nodes: extended futility pruning
	else if (depthLeft == 2) {
		if (!inCheck && !(abs(alpha) > 9000 || abs(beta) > 9000)) {
			if (b.eval() + R_VAL < alpha && moveList.size() > 0) {
				depthLeft--;
			}
		}
	}
	//Pre-pre-frontier nodes: razoring
	else if (depthLeft == 3) {
		if (!inCheck && !(abs(alpha) > 9000 || abs(beta) > 9000)) {
			if (b.eval() + Q_VAL < alpha && moveList.size() > 0) {
				depthLeft--;
			}
		}
	}

	//Put principal variation and killer moves first
	int temp;
	if (allowNull) { //Except if we already null-moved, or checking a check
		//Killer moves in front,
/*
		int killerMove;
		for (int i = 1; i >= 0; i--) {
			killerMove = killers[depthGone][i];
			if (killerMove == 0) continue;
			std::vector<int>::iterator kIndex;
			kIndex = std::find(moveList.begin(), moveList.end(), killerMove);
			if (kIndex != moveList.end()) {
				moveList.erase(kIndex);
				moveList.insert(moveList.begin()+0, killerMove);
			}
		}
*/
		//then PV in front
		std::vector<int>::iterator pvIndex;
		if (depthGone < oldPrinVarLine.count) {
			int pvmove = oldPrinVarLine.move[depthGone];
			if (pvmove != 0 && depthGone < oldPrinVarLine.count) {
				pvIndex = std::find(moveList.begin(), moveList.end(), pvmove);
				if (pvIndex != moveList.end()) {
					temp = *pvIndex;
					moveList.erase(pvIndex);
					moveList.insert(moveList.begin()+0, temp);
				}
			}
		}

	}
	
	int movesSearched = 0;

	//Loop through moves
	for (size_t i = 0; i < moveList.size(); i++) {
		vector<int> localPV;
		nodes++;

		mF = moveList[i]/100;
		mT = moveList[i]%100;
		b.setMove(mF, mT);
		b.movePiece();
		b.changeTurn();

		//Pawn push extensions
		if (b.piece[b[mF]].getValue() == P_VAL) {
			if (s && (mT/10 == 80 || mT/10 == 90))
				ext += 50;
			else if (!s && (mT/10 == 30 || mT/10 == 20))
				ext += 50;
		}

		//If we had an alpha cutoff, do a zero-window search (guessing we were right)
		if (foundPV) {
			if (movesSearched >= 4 && b.getPly() > 4 &&
			    b.getPrevOnMoveTo(b.getNumMovesMade()-1) == empty && 
			    !b.inCheck(s) && mT != b.getPmSq(b.getNumMovesMade()-1)) {
				score = -alphaBeta(b, -alpha-1, -alpha, depthLeft-2, depthGone+1, &line, 1, ext%100);
			}
			else
				score = alpha+1;
			if (score > alpha) {
				score = -alphaBeta(b, -alpha-1, -alpha, depthLeft-1, depthGone+1, &line, 1, ext%100);
				if ((score > alpha) && (score < beta)) //If we were wrong
					score = -alphaBeta(b, -beta, -alpha, depthLeft-1, depthGone+1, &line, 1, ext%100);
			}
		}	
		else
			score = -alphaBeta(b, -beta, -alpha, depthLeft-1, depthGone+1, &line, 1, ext%100);

		b.unmovePiece();
		b.changeTurn();

		if (score >= beta) { //Fail-high
			//If it wasn't a capture, update HH table and killer moves
			if (b[mT] == empty) {
				hh[to64(mF)-1][to64(mT)-1] += depthGone*depthGone;
/*
				if (killers[depthGone][0] != mF*100+mT) {
					killers[depthGone][1] = killers[depthGone][0];
					killers[depthGone][0] = mF*100+mT;
				}
*/
			}
			return beta;
		}
		if (score > alpha) { //Best so far
			alpha = score;
			foundPV = true;
			//Add the move to the principal variation
			pline->move[0] = mF*100 + mT;
			memcpy(pline->move + 1, line.move, line.count * sizeof(int));
			pline->count = line.count + 1;
		}

		movesSearched++;
	}
	
	return alpha;
}

int Bot::quies(Board& b, int alpha, int beta, int depthGone) {
	using std::vector;

	bool s = b.getSide();

	//If our king is dead, return a bad score
	if ((s && !b.piece[wK].getAlive()) || (!s && !b.piece[bK].getAlive()))
		return -9999 + depthGone-1;

	int currEval = b.eval();
	int score, mF, mT;
	
	//If standing pat is too good
	if (currEval >= beta)
		return beta;
	//If standing pat is the best option
	if (currEval > alpha)
		alpha = currEval;

	vector<int> nonQuiesList;

	//Get psuedo-legal captures
	b.getCaptures(s, nonQuiesList);

	//Order the captures by MVVLVA
	b.sortCaptures(nonQuiesList);

	//Add promotions to start
	b.addPromotions(s, nonQuiesList);
	
	//Loop through the captures/promotions
	for (size_t i = 0; i < nonQuiesList.size(); i++) {
		mF = nonQuiesList[i]/100;
		mT = nonQuiesList[i]%100;

		//Futility (AKA delta) pruning
		if (b.getWhiteMaterial() > ENDGAME_VAL && b.getBlackMaterial() > ENDGAME_VAL)
			if (b.piece[b[mT]].getValue() + 120 + currEval <= alpha)
				if (!b.inCheck(s)) 
					continue;

		qNodes++;

		b.setMove(mF, mT);
		b.movePiece();
		//If we are in check, keep searching
		if (b.inCheck(s)) {
			b.unmovePiece();
			continue;
		}

		b.changeTurn();
		score = -quies(b, -beta, -alpha, depthGone+1);
		b.unmovePiece();
		b.changeTurn();
		
		if (score >= beta) //Fail-high
			return beta;
		if (score > alpha) //Best so far
			alpha = score;
	}
	
	return alpha;
}

int Bot::getFromHH(int mF, int mT) {
	assert(mF > -1 && mF < 65);
	assert(mT > -1 && mT < 65);
	return hh[mF][mT];
}
