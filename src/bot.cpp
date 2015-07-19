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
	transTable = new HASHENTRY[TTSIZE];
	clearTT();
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
	int alpha = -CHECKMATE_VAL-1, beta = CHECKMATE_VAL+1;
	int asp = P_VAL/2;
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
				alpha = -CHECKMATE_VAL-1;
			}
			else if (bestScore >= beta) { 
				std::cout << "FAIL HIGH DEPTH " << i << "\n";
				beta = CHECKMATE_VAL+1;
			}
			else break;
		}

		bestMoveSoFar = prinVarLine.move[0];
		//bestMoveSoFar = abs(transTable[int(b.getZobrist()%TTSIZE)].bestMoveAndScore);
		//bestMoveSoFar = bestMoveSoFar/10000;
		
		std::cout << "TRANSTABLE INFO\n----------\n";
		std::cout << "actual zobrist: " << b.getZobrist() << '\n';
		std::cout << "entry at TT zobrist..\nkey: " << transTable[int(b.getZobrist()%TTSIZE)].zKey;
		std::cout << "\nbestMoveAndScore: " << transTable[int(b.getZobrist()%TTSIZE)].bestMoveAndScore;
		std::cout << "\ndepthAndNodeType: " << transTable[int(b.getZobrist()%TTSIZE)].depthAndNodeType;
		std::cout << "\n\n";

		//clearTT();

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
		if (bestScore >= MATING_VAL)
			break;

		if (i == depth && bestScore < STALEMATE_VAL && bestScore > -STALEMATE_VAL) {
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

	clearTT();

	return prinVarLine.move[0];
	//return bestMoveSoFar;
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
		storeTTEntry(b, 9999, depthLeft, -CHECKMATE_VAL + depthGone - 1, 0);
		return -CHECKMATE_VAL + depthGone - 1;
	}

	//Repetition detection
	if (allowNull && depthGone) {
		if (b.drawCheck()) {
			pline->count = 0;
			return 0;
		}
	}

	LINE line;

	//Useful to know this for later	
	bool inCheck = b.inCheck(s);

	//Transposition table look-up

	HASHENTRY hashEntry = getTTEntry(b.getZobrist());

	if (hashEntry.zKey == b.getZobrist() && depthGone != 0) {
		int hashMF = abs(hashEntry.bestMoveAndScore / 1000) / 100;
		int hashMT = abs(hashEntry.bestMoveAndScore / 1000) % 100;

		if (hashEntry.depthAndNodeType / 10 >= depthLeft) {
			if (hashEntry.depthAndNodeType % 10 == 0) {
				int score2 = hashEntry.bestMoveAndScore % 10000;
			//	std::cout << "hashEntry.zKey: " << hashEntry.zKey << "\n";
			//	std::cout << "hashEntry.depthAndNodeType: " << hashEntry.depthAndNodeType << "\n";
			//	std::cout << "hashEntry.bestMoveAndScore: " << hashEntry.bestMoveAndScore << "\n";
			//	std::cout << "score2: " << score2 << "\n";
			
				//Add the move to the principal variation
				line.count = 0;
				pline->move[0] = hashMF * 100 + hashMT;
				memcpy(pline->move + 1, line.move, line.count * sizeof(int));
				pline->count =  line.count + 1;

				if (score2 >= beta) { //Fail-high
					return beta;
				}
				else if (score2 < alpha) { //Fail-low
					return alpha;
				}
				return score2;
			}
			//Lower bound
			else if (hashEntry.depthAndNodeType % 10 == 1) {
				int score2 = hashEntry.bestMoveAndScore % 10000;

				if (score2 >= beta) { //Fail-high
					return score2;
				}
				if (score2 > alpha) {
					//alpha = score2;
				}
			}
			//Upper bound
			else if (hashEntry.depthAndNodeType % 10 == 2) {
				int score2 = hashEntry.bestMoveAndScore % 10000;

				if (score2 <= alpha) { //Fail-low
					return score2;
				}
				if (score2 < beta) {
					//beta = score2;
				}
			}
		}
	}


	//Horizon nodes, quiescence search
	if (depthLeft <= 0) {
		//If we're in check, search a little further
		if (allowNull && b.inCheck(s)) {
			return alphaBeta(b, alpha, beta, 1, depthGone, pline, 0, 0);
		}

		//Otherwise, do a quiescence search
		pline->count = 0;
		int v = quies(b, alpha, beta, depthGone);
		//storeTTEntry(b, 9999, 0, v, 0);
		return v;
	}

	int score;

	//Give extensions
	while (ext >= 100) {
		depthLeft++;
		ext -= 100;
	}
	
	//Null move reduction
	if (allowNull && !inCheck) {
		if ((s && b.getWhiteMaterial() > ENDGAME_VAL) || (!s && b.getBlackMaterial() > ENDGAME_VAL)) {
			r = depthLeft > 6 ? 4 : 3;
			b.changeTurn();
			b.zobristXorSide();
			score = -alphaBeta(b, -beta, -beta+1, depthLeft-r-1, depthGone, pline, 0, 0);
			b.changeTurn();
			b.zobristXorSide();
			if (score >= beta) { //Fail-high
				pline->count = 0;
				return score;
			}
			//Extend on nearby checkmates
			else if (score < -MATING_VAL)
				ext += 50;
		}
	}

	bool foundPV = false;
	int mF, mT;
	vector<int> moveList;

	b.genOrderedMoveList(s, moveList);
	b.cleanMoveList(s, moveList);
	
	//Evading check extension
	if (inCheck)
		ext += 75;

	//No legal moves
	if (moveList.size() == 0) {
		pline->count = 0;
		//If we are in checkmate, return bad score
		if (inCheck) {
			storeTTEntry(b, 9999, depthLeft, -CHECKMATE_VAL + depthGone - 1, 0);

			return -CHECKMATE_VAL + depthGone - 1;
		}
		//Only favor stalemate if we're losing
		else {
			if (b.getWhiteMaterial() >= b.getBlackMaterial()) {
				int v = s ? -STALEMATE_VAL : STALEMATE_VAL;

				storeTTEntry(b, 9999, depthLeft, v, 0);

				return v;
			}
		}
	}
	//Singular reply
	else if (moveList.size() == 1) {
		//On a root node, quit early
		if (depthGone == 0) {
			pline->move[0] = moveList[0];
			pline->count = 1;

			storeTTEntry(b, moveList[0], depthLeft, -STALEMATE_VAL, 0);

			return -STALEMATE_VAL;
		}	

		//Otherwise, extend
		ext += 50;
	}
	//Only two replies
	else if (moveList.size() == 2)
		ext += 25;

	//Frontier nodes: futility pruning
	if (depthLeft == 1) {
		if (!inCheck && !(abs(alpha) > MATING_VAL || abs(beta) > MATING_VAL)) {
			if (b.eval() + B_VAL < alpha && moveList.size()) {
				pline->count = 0;
				return quies(b, alpha, beta, depthGone);
			}
		}
	}
	//Pre-frontier nodes: extended futility pruning
	else if (depthLeft == 2) {
		if (!inCheck && !(abs(alpha) > MATING_VAL || abs(beta) > MATING_VAL)) {
			if (b.eval() + R_VAL < alpha && moveList.size()) {
				depthLeft--;
			}
		}
	}
	//Pre-pre-frontier nodes: razoring
	else if (depthLeft == 3) {
		if (!inCheck && !(abs(alpha) > MATING_VAL || abs(beta) > MATING_VAL)) {
			if (b.eval() + Q_VAL < alpha && moveList.size()) {
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
		
		
		//Hash move
		
		std::vector<int>::iterator pvIndex2;
		if (hashEntry.zKey == b.getZobrist()) {
			int pvmove = hashEntry.bestMoveAndScore / 10000;
			if (pvmove != 9999 && pvmove != 0) {
				pvIndex2 = std::find(moveList.begin(), moveList.end(), pvmove);
				if (pvIndex2 != moveList.end()) {
					temp = *pvIndex2;
					moveList.erase(pvIndex2);
					moveList.insert(moveList.begin()+0, temp);
				}
			}
		}
		
		//then PV in front
		std::vector<int>::iterator pvIndex;
		if (depthGone < oldPrinVarLine.count) {
			int pvmove = oldPrinVarLine.move[depthGone];
			if (pvmove != 0 && depthGone < oldPrinVarLine.count) {
				pvIndex = std::find(moveList.begin(), moveList.end(), pvmove);
				if (pvIndex != moveList.end()) {
					temp = *pvIndex;
					moveList.erase(pvIndex);
					moveList.insert(moveList.begin() + 0, temp);
				}
			}
		}
	}
	
	int movesSearched = 0;

	//Loop through moves
	for (size_t i = 0; i < moveList.size(); i++) {
		nodes++;

		mF = moveList[i]/100;
		mT = moveList[i]%100;
		b.setMove(mF, mT);
		b.movePiece();
		b.changeTurn();

		//Pawn push extensions
		if (b[mF] != empty && b.piece[b[mF]].getValue() == P_VAL) {
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
				score = -alphaBeta(b, -alpha-1, -alpha, depthLeft-2, depthGone+1, &line, 1, ext-100);
			}
			else
				score = alpha+1;
			if (score > alpha) {
				score = -alphaBeta(b, -alpha-1, -alpha, depthLeft-1, depthGone+1, &line, 1, ext-100);
				if ((score > alpha) && (score < beta)) //If we were wrong
					score = -alphaBeta(b, -beta, -alpha, depthLeft-1, depthGone+1, &line, 1, ext-100);
			}
		}	
		else
			score = -alphaBeta(b, -beta, -alpha, depthLeft-1, depthGone+1, &line, 1, ext-100);

		b.changeTurn();
		b.unmovePiece();

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
			//Transposition table storage, lower bound
			storeTTEntry(b, mF * 100 + mT, depthLeft, beta, 1);

			return beta;
		}
		if (score > alpha) { //Best so far
			alpha = score;
			foundPV = true;
			//Add the move to the principal variation
			pline->move[0] = mF*100 + mT;
			memcpy(pline->move + 1, line.move, line.count * sizeof(int));
			pline->count = line.count + 1;

			//Transposition table storage, exact score
			storeTTEntry(b, mF * 100 + mT, depthLeft, score, 0);
		}
		else {
			if (transTable[int(b.getZobrist() % TTSIZE)].zKey != b.getZobrist()) {
				storeTTEntry(b, mF * 100 + mT, depthLeft, alpha, 2);
			}
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
		return -CHECKMATE_VAL + depthGone-1;

	int currEval = b.eval();
	int score, mF, mT;
	
	//If standing pat is too good
	if (currEval >= beta) {
		return beta;
	}
	//If standing pat is the best option
	if (currEval > alpha)
		alpha = currEval;

	vector<int> nonQuiesList;

	//Get psuedo-legal captures
	b.generateCaptures(s, nonQuiesList);

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
			if (b[mT] != empty && b.piece[b[mT]].getValue() + 1.2*P_VAL + currEval <= alpha)
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

		if (score >= beta) { //Fail-high
			return beta;
		}
		if (score > alpha) { //Best so far
			alpha = score;
		}
		else {
		}
	}
	
	return alpha;
}

int Bot::getFromHH(int mF, int mT) {
	assert(mF > -1 && mF < 65);
	assert(mT > -1 && mT < 65);
	return hh[mF][mT];
}

void Bot::clearTT() {
	HASHENTRY clear;
	clear.zKey = 0;
	clear.bestMoveAndScore = 0;
	clear.depthAndNodeType = 0;
	for (int i = 0; i < TTSIZE; i++) {
		transTable[i] = clear;
	}
}

void Bot::storeTTEntry(Board& b, int move, int depthLeft, int score, int nodeType) {
	unsigned long zobrist = b.getZobrist();

	if (transTable[int(zobrist%TTSIZE)].depthAndNodeType/10 <= b.getPly()) {
		HASHENTRY storage;
		storage.zKey = zobrist;
		storage.bestMoveAndScore = move * 10000 + abs(score);
		storage.depthAndNodeType = depthLeft * 10 + nodeType;
		transTable[int(zobrist%TTSIZE)] = storage;
	}
}

HASHENTRY Bot::getTTEntry(unsigned long long key) {
	return transTable[int(key%TTSIZE)];
}