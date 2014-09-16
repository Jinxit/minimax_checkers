#ifndef _CHECKERS_PLAYER_HPP_
#define _CHECKERS_PLAYER_HPP_

#include "constants.hpp"
#include "deadline.hpp"
#include "move.hpp"
#include "gamestate.hpp"
#include <vector>
#include <unordered_map>
#include <string>

namespace checkers
{

class Player
{
public:
    ///perform a move
    ///\param pState the current state of the board
    ///\param pDue time before which we must have returned
    ///\return the next state the board is in after our move
    GameState play(const GameState &pState, const Deadline &pDue);

    int alphabeta(const GameState &node, int depth, int alpha, int beta, bool maximizingPlayer, const Deadline &pDue);
    int getScore(const GameState &node);

private:
	uint8_t currentPlayer;
	bool firstTime = true;
	Deadline due;
	std::unordered_map<std::string, int> scoreMap;
};

/*namespace checkers*/ }

#endif
