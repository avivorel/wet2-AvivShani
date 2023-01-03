

#include "Team.h"
#include <memory>

Team::Team(int teamId): team_id(teamId), games_played(0), numberOfPlayers(0), numberOfGK(0),
                        hasGK(false), root_player(nullptr), points(0), teamSpirit_without_root(permutation_t::neutral()), team_ability(0){};



void Team::add_player(const std::shared_ptr<Player>& playerToAdd)
{
        this->numberOfPlayers = this->numberOfPlayers + 1;
        if (playerToAdd->isGoalie)
        {
            this->hasGK = true;
            this->numberOfGK += 1;
        }

}

