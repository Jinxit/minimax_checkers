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
		 		int score = alphabeta(lNextStates[i], depth, INT_MIN, INT_MAX, true, pDue);
		 		
		 		if (maxScore < score)
		 		{
		 			maxScore = score;
		 			maxIndex = i;
		 		}
		 	}
	 	}

	    return lNextStates[maxIndex];
	}

	int Player::alphabeta(const GameState &node, int depth, int alpha, int beta, bool maximizingPlayer, const Deadline &pDue)
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
		
		/*if (pDue <= Deadline::now())
		{
			if (maximizingPlayer)
			{
				return INT_MIN + 1;
			}
			else
			{
				return INT_MAX - 1;
			}
		}*/

		std::vector<GameState> children;
	    node.findPossibleMoves(children);

    	if (maximizingPlayer)
    	{
		    for (uint i = 0; i < children.size(); i++)
		    {
	    		alpha = max(alpha, alphabeta(children[i], depth - 1, alpha, beta, false, pDue));
	    		if (beta <= alpha)
	    		{
	    			break;
	    		}
	    	}

		    scoreMap[nodeString] = alpha;
		    // also add the reverse
		    scoreMap[node.reversed().toMessage()] = -alpha;
	    	return alpha;
    	}
    	else
    	{
		    for (uint i = 0; i < children.size(); i++)
		    {
	    		beta = min(beta, alphabeta(children[i], depth - 1, alpha, beta, true, pDue));
	    		if (beta <= alpha)
	    		{
	    			break;
	    		}
    		}

		    scoreMap[nodeString] = beta;
		    // also add the reverse
		    scoreMap[node.reversed().toMessage()] = -beta;
    		return beta;
    	}
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
