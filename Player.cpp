

#include "Player.h"
#include <memory>
#include <utility>
#include "Team.h"


Player::Player(int playerId, int teamId, const permutation_t& spirit, int gamesplayed, int ability, int cards, bool
isGoalKeeper) :
        player_id(playerId), games_played(gamesplayed),ability(ability), cards(cards),
        isGoalie(isGoalKeeper), team(nullptr), parent(nullptr), fixed_spirit(spirit),  teamDeleted(false){}



int Player::comparePlayerId(const std::shared_ptr<Player> &player1, const std::shared_ptr<Player> &player2) {
    if (player1->player_id > player2->player_id) return 1;
    else if(player1->player_id < player2->player_id ) return -1;
    return 0;
}

void Player::Union(std::shared_ptr<Team> &buying_team ,std::shared_ptr<Team> &acquired_team)
{

    // check which team is bigger
    std::shared_ptr<Player> root_parent_buying = (buying_team->root_player.lock()->Find() != nullptr) ? buying_team->root_player.lock()->Find() : buying_team->root_player.lock() ;
    std::shared_ptr<Player> root_parent_acquired = (acquired_team->root_player.lock()->Find() != nullptr) ? acquired_team->root_player.lock()->Find() : acquired_team->root_player.lock();
    permutation_t a_old = root_parent_buying->fixed_spirit;
    permutation_t b_old = root_parent_acquired->fixed_spirit;
    permutation_t a_max = buying_team->teamSpirit_without_root;
    if (buying_team->numberOfPlayers >= acquired_team->numberOfPlayers)
    {
        root_parent_acquired->parent = root_parent_buying;
        root_parent_acquired->fixed_spirit = a_max * b_old;
        root_parent_acquired->games_played -= root_parent_buying->games_played;

    }
    else
    {
        root_parent_buying->parent = root_parent_acquired;
        root_parent_acquired->fixed_spirit = a_old * a_max * b_old;
        root_parent_buying->fixed_spirit =  root_parent_acquired->fixed_spirit.inv() * a_old;
        root_parent_buying->games_played -= root_parent_acquired->games_played;

    }
    buying_team->numberOfPlayers += acquired_team->numberOfPlayers;
    buying_team->team_ability+=acquired_team->team_ability;
    buying_team->teamSpirit_without_root = buying_team->teamSpirit_without_root * acquired_team->rootSpirit *
            acquired_team->teamSpirit_without_root; // shani added?????
    acquired_team->root_player.lock()->team = buying_team;
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

void Player::setTeam(std::shared_ptr<Team> t)
{
    this->team=std::move(t);
}


void Player::UnionBuyingEmpty(std::shared_ptr<Team> &buying_team, std::shared_ptr<Team> &acquired_team) {
        // שינוי המצביעים לקבוצה החדשה
        buying_team->root_player = acquired_team->root_player;
        acquired_team->root_player.lock()->team = buying_team;
        buying_team->numberOfPlayers += acquired_team->numberOfPlayers;
        buying_team->games_played = acquired_team->games_played;
        buying_team->teamSpirit_without_root = acquired_team->teamSpirit_without_root;
        buying_team->rootSpirit = acquired_team->rootSpirit;
        buying_team->numberOfGK = acquired_team->numberOfGK;
        buying_team->hasGK = acquired_team->hasGK;
        buying_team->team_ability+=acquired_team->team_ability;

}
void Player::UnionAcquiredEmpty(std::shared_ptr<Team> &buying_team, std::shared_ptr<Team> &acquired_team) {
    // האם בכלל אמור לקרות משהו?
}

