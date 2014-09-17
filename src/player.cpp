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
		if (firstTime)
		{
			due = Deadline::now() + 14 * 1000 * 1000;
			firstTime = false;
		}
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
	 	for (int depth = 5; depth < 500; depth++)
	 	{
	 		if (pDue <= Deadline::now())
	 		{
	 			break;
	 		}
	 		maxScore = INT_MIN;
	 		maxIndex = -1;
		 	for (uint i = 0; i < lNextStates.size(); i++)
		 	{
		 		int score = negamax(lNextStates[i], depth, INT_MIN, INT_MAX, 1, pDue);
		 		
		 		if (maxScore < score)
		 		{
		 			maxScore = score;
		 			maxIndex = i;
		 		}
		 	}
	 	}

	    return lNextStates[maxIndex];
	}

	int Player::negamax(const GameState &node, int depth, int alpha, int beta, int color, const Deadline &pDue)
	{
		int alphaOrig = alpha;

		std::string nodeString = node.toMessage();
		if (scoreMap.count(nodeString) > 0 && scoreMap[nodeString].depth >= depth)
		{
			Tentry entry = scoreMap[nodeString];
			if (entry.flag == EXACT)
			{
				return entry.value;
			}
			else if (entry.flag == LOWERBOUND)
			{
				alpha = max(alpha, entry.value);
			}
			else if (entry.flag == UPPERBOUND)
			{
				beta = min(beta, entry.value);
			}

			if (alpha >= beta)
			{
				return entry.value;
			}
		}

		if (depth == 0 || node.isEOG())
		{
			return color * getScore(node);
		}
		
		if (pDue <= Deadline::now())
		{
			if (color == 1)
			{
				return INT_MIN + 1;
			}
			else
			{
				return INT_MAX - 1;
			}
		}

		std::vector<GameState> children;
	    node.findPossibleMoves(children);

	    int bestValue = INT_MIN;
	    for (uint i = 0; i < children.size(); i++)
	    {
	    	int val = -negamax(children[i], depth - 1, -beta, -alpha, -color, pDue);
	    	bestValue = max(bestValue, val);
	    	alpha = max(alpha, val);
	    	if (alpha >= beta)
	    	{
	    		break;
	    	}
	    }

	    Tentry entry;
	    entry.value = bestValue;
	    if (bestValue <= alphaOrig)
	    {
	    	entry.flag = UPPERBOUND;
	    }
	    else if (bestValue >= beta)
	    {
	    	entry.flag = LOWERBOUND;
	    }
	    else
	    {
	    	entry.flag = EXACT;
	    }
	    entry.depth = depth;
		scoreMap[nodeString] = entry;

		return bestValue;
	}

	int Player::getScore(const GameState &node)
	{
		int score = 0;

		//simple: count number of pieces for each player, subtract mine - yours
		for (int i = 1; i <= node.cSquares; i++)
		{
			auto piece = node.at(i);

			int factor = 1;
			if (piece & CELL_KING)
			{
				factor = 2;
			}

			if (piece & CELL_WHITE)
			{
				score += (currentPlayer & CELL_WHITE ? 1 : -1) * factor;
			}
			else if (piece & CELL_RED)
			{
				score += (currentPlayer & CELL_RED ? 1 : -1) * factor;
			}
		}

		return score;
	}

	/*namespace checkers*/ 
}
