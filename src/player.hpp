#ifndef _CHECKERS_PLAYER_HPP_
#define _CHECKERS_PLAYER_HPP_

#include "constants.hpp"
#include "deadline.hpp"
#include "move.hpp"
#include "gamestate.hpp"
#include <vector>
#include <unordered_map>
#include <string>
#include <utility>

namespace checkers
{

enum Flag 
{
	EXACT,
	LOWERBOUND,
	UPPERBOUND	
};

struct Tentry
{
	Flag flag;
	int depth;
	int value;
};

class Player
{
public:
    ///perform a move
    ///\param pState the current state of the board
    ///\param pDue time before which we must have returned
    ///\return the next state the board is in after our move
    GameState play(const GameState &pState, const Deadline &pDue);

    int negamax(const GameState &node, int depth, int alpha, int beta, int color, const Deadline &pDue, int depthOrig);
    int getScore(const GameState &node);

private:
	uint8_t currentPlayer;
	std::unordered_map<std::string, Tentry> scoreMap;
	GameState next;
	bool found;
};

/*namespace checkers*/ }

#endif
