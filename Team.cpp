

#include "Team.h"
#include <memory>

Team::Team(int teamId):team_id(teamId),games_played(0),numberOfPlayers(0),numberOfGK(0),
hasGK(false), root_player(nullptr), points(0), team_spirit(permutation_t::neutral()), team_ability(0){};

int Team::compareTeamId(const std::shared_ptr<Team> &a, const std::shared_ptr<Team> &b)
{
    if(a->team_id > b->team_id) return 1;
    if(a->team_id < b->team_id) return -1;
    return 0;
}


void Team::add_player(const std::shared_ptr<Player>& playerToAdd)
{
        this->numberOfPlayers = this->numberOfPlayers + 1;
        if (playerToAdd->isGoalie)
        {
            this->hasGK = true;
            this->numberOfGK += 1;
        }

}