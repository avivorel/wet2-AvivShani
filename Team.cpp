

#include "Team.h"
#include <memory>

Team::Team(int teamId): team_id(teamId), points(0),  games_played(0), numberOfPlayers(0), numberOfGK(0),
                        hasGK(false), root_player() ,teamSpirit(permutation_t::neutral()),
                        rootSpirit(permutation_t::neutral()), team_ability(0){}



void Team::add_player(const std::shared_ptr<Player>& playerToAdd)
{
    if (this->numberOfPlayers == 0){
        this->root_player = playerToAdd;
    }
    this->numberOfPlayers = this->numberOfPlayers + 1;
    if (playerToAdd->isGoalie)
    {
        this->hasGK = true;
        this->numberOfGK += 1;
    }
    team_ability += playerToAdd->ability;
}

