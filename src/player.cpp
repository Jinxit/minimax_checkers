#include "player.hpp"
#include <cstdlib>
#include <limits.h>
#include <algorithm>

#include <iostream>

namespace checkers
{

	using std::min;
	using std::max;

	GameState Player::play(const GameState &pState,const Deadline &pDue)
	{
	    //std::cerr << "Processing " << pState.toMessage() << std::endl;
		std::vector<GameState> lNextStates;
	    pState.findPossibleMoves(lNextStates);
	    
	    if (lNextStates.size() == 0) return GameState(pState, Move());

	    /*
	     * Here you should write your clever algorithms to get the best next move, ie the best
	     * next state. This skeleton returns a random move instead.
	     */

	    currentPlayer = pState.getNextPlayer();

	    int maxScore = INT_MIN;
	 	int maxIndex = -1;
	 	for (uint i = 0; i < lNextStates.size(); i++)
	 	{
	 		int depth = 20;
	 		int score = minimax(lNextStates[i], depth, true, pDue);
	 		
	 		if (maxScore < score)
	 		{
	 			maxScore = score;
	 			maxIndex = i;
	 		}
	 	}

	    return lNextStates[maxIndex];
	}

	int Player::minimax(const GameState &node, int depth, bool maximizingPlayer, const Deadline &pDue)
	{
		std::string nodeString = node.toMessage();
		if (scoreMap.count(nodeString) > 0)
		{
			return scoreMap[nodeString];
		}
		if (depth == 0 || node.isEOG())
		{
			return getScore(node);
		}
		
		int bestValue = INT_MAX;
		if (maximizingPlayer)
		{
			bestValue = INT_MIN;
		}
		if (pDue < Deadline::now())
		{
			return bestValue + (maximizingPlayer ? 1 : -1);
		}

		std::vector<GameState> children;
	    node.findPossibleMoves(children);

	    for (uint i = 0; i < children.size(); i++)
	    {
	    	int val = minimax(children[i], depth - 1, !maximizingPlayer, pDue);

	    	if (maximizingPlayer)
	    	{
	    		bestValue = max(bestValue, val);
	    	}
	    	else
	    	{
	    		bestValue = min(bestValue, val);
	    	}
	    }

	    scoreMap[nodeString] = bestValue;
	    return bestValue;
	}

	int Player::getScore(const GameState &node)
	{
		int score = 0;

		//simple: count number of pieces for each player, subtract mine - yours
		for (int i = 1; i <= node.cSquares; i++)
		{
			auto piece = node.at(i);
			if (piece & CELL_WHITE)
			{
				score += currentPlayer & CELL_WHITE ? 1 : -1;
			}
			else if (piece & CELL_RED)
			{
				score += currentPlayer & CELL_RED ? 1 : -1;
			}
		}

		return score;
	}

	/*namespace checkers*/ 
}
