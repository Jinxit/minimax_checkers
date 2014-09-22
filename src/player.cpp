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


		currentPlayer = pState.getNextPlayer();

		found = false;

		for (int depth = 10; depth < 15; depth++)
		{
			negamax(pState, depth, -1000, 1000, 1, pDue, depth);
			
			if (pDue <= Deadline::now())
			{
				break;
			}
		}
		if (found)
		{
			return next;
		}
		else
		{
			std::vector<GameState> lNextStates;
			pState.findPossibleMoves(lNextStates);
			return lNextStates[0];
		}
	}

	int Player::negamax(const GameState &node, int depth, int alpha, int beta, int color, const Deadline &pDue, int depthOrig)
	{
		//int alphaOrig = alpha;

		/*std::string nodeString = node.toMessage();
		if (scoreMap.count(nodeString) > 0 && scoreMap[nodeString].depth >= depth)
		{
			Tentry entry = scoreMap[nodeString];
			if (entry.flag == EXACT)
			{
				if (depth == depthOrig - 1)
				{
					next = node;
					found = true;
				}
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
				if ((depth == depthOrig - 1))
				{
					next = node;
					found = true;
				}
				return entry.value;
			}
		}*/

		if (depth == 0 || node.isEOG())
		{
			return color * getScore(node);
		}
		
		if (pDue <= Deadline::now())
		{
			return color * -1000;
		}

		std::vector<GameState> children;
		node.findPossibleMoves(children);

		int bestValue = -1000;
		for (uint i = 0; i < children.size(); i++)
		{
			int val = -negamax(children[i], depth - 1, -beta, -alpha, -color, pDue, depthOrig);
			bestValue = max(bestValue, val);
			alpha = max(alpha, val);
			if (alpha >= beta)
			{
				break;
			}
			else
			{
				if (depth == depthOrig)
				{
					next = children[i];
					found = true;
				}	
			}
		}

		/*Tentry entry;
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

		if (depth == depthOrig - 1)
		{
			next = node;
			found = true;
		}*/

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
				score += (currentPlayer & CELL_WHITE ? 1 : -1);
			}
			else if (piece & CELL_RED)
			{
				score += (currentPlayer & CELL_RED ? 1 : -1);
			}
		}

		if (node.isEOG())
		{
			score *= 100;
		}

		return score;
	}

	/*namespace checkers*/ 
}
