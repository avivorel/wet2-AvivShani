

#include "Player.h"
#include <memory>
#include <utility>

class Team{};


Player::Player(int playerId, int teamId, const permutation_t& spirit, int gamesplayed, int ability, int cards, bool
isGoalKeeper) :
        player_id(playerId), team_id(teamId), games_played(gamesplayed),ability(ability), cards(cards),
        isGoalie(isGoalKeeper), team(nullptr), parent(nullptr), spirit(spirit),  teamDeleted(false){};



int Player::comparePlayerId(const std::shared_ptr<Player> &player1, const std::shared_ptr<Player> &player2) {
    if (player1->player_id > player2->player_id) return 1;
    else if(player1->player_id < player2->player_id ) return -1;
    return 0;
}
