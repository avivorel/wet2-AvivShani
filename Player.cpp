

#include "Player.h"
#include <memory>
#include <utility>
#include "Team.h"


Player::Player(int playerId, int teamId, const permutation_t& spirit, int gamesplayed, int ability, int cards, bool
isGoalKeeper) :
        player_id(playerId), games_played(gamesplayed),ability(ability), cards(cards),
        isGoalie(isGoalKeeper), team(nullptr), parent(nullptr), fixed_spirit(spirit),  teamDeleted(false){};



int Player::comparePlayerId(const std::shared_ptr<Player> &player1, const std::shared_ptr<Player> &player2) {
    if (player1->player_id > player2->player_id) return 1;
    else if(player1->player_id < player2->player_id ) return -1;
    return 0;
}

void Player::Union(std::shared_ptr<Team> &buying_team ,std::shared_ptr<Team> &acquired_team)
{
    permutation_t a_max = buying_team->teamSpirit_without_root;
    permutation_t a_old= buying_team->root_player->fixed_spirit;
    permutation_t b_old= acquired_team->root_player->fixed_spirit;

    // check which team is bigger
    if (buying_team->numberOfPlayers >= acquired_team->numberOfPlayers)
    {
        // in this case we take the acuired_team's root and link it to this root.

        acquired_team->root_player->fixed_spirit=  a_max * b_old;

        acquired_team->root_player->parent = buying_team->root_player;
        buying_team->numberOfPlayers += acquired_team->numberOfPlayers;
        acquired_team->root_player->games_played -= buying_team->games_played;
        // we need go update all of the neccesary fields. (games played, permutations, etc...)
    }
    else
    { // in this case, the acquired team is bigger, so we have to change the acquired team details to the buying team's details
        acquired_team->root_player->fixed_spirit=  a_old * a_max * b_old;
        buying_team->root_player->fixed_spirit=  acquired_team->root_player->fixed_spirit.inv() * a_old;

        buying_team->root_player->parent = acquired_team->root_player;
        acquired_team->numberOfPlayers += buying_team->numberOfPlayers;
        acquired_team->root_player->team = buying_team;
        buying_team->root_player->games_played -= acquired_team->games_played;
    }

}

std::shared_ptr<Player> Player::Find() {
    if (this->parent == nullptr){
        return nullptr;
    }

    std::shared_ptr<Player> everyones_parent = this->parent;

    permutation_t multSpirit = this->fixed_spirit;
    permutation_t sub_forSpirit = permutation_t::neutral();

    int sum_games_played = this->games_played;
    int sub_forGames = 0;

    while(everyones_parent->parent != nullptr)
    { // gets root
        sum_games_played += everyones_parent->games_played;
        multSpirit= everyones_parent->fixed_spirit * multSpirit;
        everyones_parent = everyones_parent->parent;
    }

    sub_forSpirit= this->fixed_spirit;
    this->fixed_spirit= multSpirit;

    sub_forGames = this->games_played;
    this->games_played=sum_games_played;

    std::shared_ptr<Player> iterator = this->parent;
    while (iterator->parent != nullptr)
    {
        permutation_t tmpForspirit= iterator->fixed_spirit;
        iterator->fixed_spirit= multSpirit * sub_forSpirit.inv();
        sub_forSpirit = tmpForspirit * sub_forSpirit;

        int tmpsub = iterator->games_played;
        iterator->games_played = sum_games_played - sub_forGames;
        sub_forGames += tmpsub;


        std::shared_ptr<Player> tmp = iterator->parent;
        iterator->parent = everyones_parent;
        iterator = tmp;
    }
    this->parent = everyones_parent;
    return iterator;
}
