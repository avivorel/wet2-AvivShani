#include "worldcup23a2.h"


world_cup_t::world_cup_t()
{
    team_tree_by_id = new AVLTree<compareTeamId,std::shared_ptr<Team>>;
    team_tree_by_ability = new AVLTree<compareTeamAbility,std::shared_ptr<Team>>;
    players_hashTable= new HashTable();
}

world_cup_t::~world_cup_t()
{
	// TODO: Your code goes here
}

StatusType world_cup_t::add_team(int teamId)
{
    if (teamId <= 0)
    {
        return StatusType::INVALID_INPUT;
    }
    try {
        std::shared_ptr<Team> teamfinder(new Team(teamId));
        auto *foundTeam = this->team_tree_by_id->find(teamfinder);
        if (foundTeam != nullptr)
        {
            return StatusType::FAILURE;
        }

        this->team_tree_by_id->insert(teamfinder);
        this->team_tree_by_ability->insert(teamfinder);

    }
    catch (const std::bad_alloc &) { return  StatusType::ALLOCATION_ERROR;}
    return  StatusType::SUCCESS;
}

StatusType world_cup_t::remove_team(int teamId)
{
    if (teamId<=0)
    {
        return StatusType::INVALID_INPUT;
    }

    try
    {
        std::shared_ptr<Team> toRemove(new Team(teamId));
        auto *team_toRemove = this->team_tree_by_id->find(toRemove);

        if (team_toRemove == nullptr)
        {
            return StatusType::FAILURE;
        }
        team_toRemove->getValue()->root_player.lock()->team= nullptr;
        team_toRemove->getValue()->root_player.lock()->teamDeleted= true; // need more?

        this->team_tree_by_id->remove(team_toRemove->getValue());
        this->team_tree_by_ability->remove(team_toRemove->getValue());
    }
    catch (const std::bad_alloc &) { return  StatusType::ALLOCATION_ERROR;}

    return StatusType::SUCCESS;

}

StatusType world_cup_t::add_player(int playerId, int teamId, const permutation_t &spirit, int gamesPlayed,
                                   int ability, int cards, bool goalKeeper)
{

    if (teamId <= 0 || playerId <= 0 || (!(spirit.isvalid())) || gamesPlayed<0 || cards< 0){
        return StatusType::INVALID_INPUT;
    }

    try
    {
        if(this->players_hashTable->Search(playerId) != nullptr) //if the player already exists
            return StatusType::FAILURE;

        //creating the player's shared_ptr
        std::shared_ptr<Player> player = std::make_shared<Player>(playerId, teamId, spirit, gamesPlayed,ability
                                                                  ,cards,goalKeeper);

        //adding the player to the players hashTable, and to the score level=0 array
        players_hashTable->Insert(player);

        // checking if the team is exists
        std::shared_ptr<Team> team(new Team(teamId));
        auto *foundteam = this->team_tree_by_id->find(team);
        if (foundteam == nullptr)
        {
            return StatusType::FAILURE;
        }

        foundteam->getValue()->add_player(player);

        if (foundteam->getValue()->numberOfPlayers == 1)
        {
            player->team=foundteam->getValue();
            player->parent= nullptr;
            foundteam->getValue()->rootSpirit = spirit; /// maybe not needed
        }
        else
        {
            /// permutation !!!!
            player->parent= foundteam->getValue()->root_player.lock();
            foundteam->getValue()->teamSpirit_without_root = foundteam->getValue()->teamSpirit_without_root * spirit;
            player->fixed_spirit = foundteam->getValue()->teamSpirit_without_root;
        }

    } catch (const std::bad_alloc &) { return  StatusType::ALLOCATION_ERROR;}
	return StatusType::SUCCESS;
}

output_t<int> world_cup_t::play_match(int teamId1, int teamId2)
{ // אוסיף אחד למשחקים גם בשורש, שכחתי למה אבל אעשה את זה
    // מעדכנים משחקים גם בשורש
    if (teamId2 <= 0  or teamId1 <= 0 or teamId2 == teamId1){
        return StatusType::INVALID_INPUT;
    }
    try {
        std::shared_ptr<Team> team1(new Team(teamId1));
        std::shared_ptr<Team> team2(new Team(teamId2));
        auto *found_team_1 = this->team_tree_by_id->find(team1);
        auto *found_team_2 = this->team_tree_by_id->find(team2);
        if (found_team_1 == nullptr or found_team_2 == nullptr) {
            return StatusType::FAILURE;
        }

        std::shared_ptr<Team> real_team_1 = found_team_1->getValue();
        std::shared_ptr<Team> real_team_2 = found_team_2->getValue();
        if (!real_team_1->hasGK or !real_team_2->hasGK) {
            return StatusType::FAILURE;
        }

        // actual match playing
        int team1_ability = real_team_1->points + real_team_1->team_ability;
        int team2_ability = real_team_2->points + real_team_2->team_ability;
        real_team_1->games_played += 1; // adding 1 to the games count of the team and root.
        real_team_1->root_player.lock()->games_played += 1;
        real_team_2->games_played += 1;
        real_team_2->root_player.lock()->games_played += 1;
        if (team1_ability > team2_ability){
            real_team_1->points += 3;
            return {1};
        }

        else if ( team2_ability > team1_ability){
            real_team_2->points += 3;
            return {3};
        }

        else{
            permutation_t team1_spirit =real_team_1->rootSpirit * real_team_1->teamSpirit_without_root;
            permutation_t team2_spirit =real_team_2->rootSpirit * real_team_2->teamSpirit_without_root;
            if (team1_spirit.strength() > team2_spirit.strength()){
                real_team_1->points += 3;
                return {2};
            }

            else if (team1_spirit.strength() < team2_spirit.strength()){
                real_team_2->points += 3;
                return {4};
            }
            else{
                real_team_1->points += 1;
                real_team_2->points += 1;
                return {0};
            }
        }

    }
    catch (const std::bad_alloc &) { return  StatusType::ALLOCATION_ERROR;}
    // TODO: Your code goes here
	return StatusType::SUCCESS;
}

output_t<int> world_cup_t::num_played_games_for_player(int playerId)
{
	// TODO: Your code goes here
	return 22;
}

StatusType world_cup_t::add_player_cards(int playerId, int cards)
{
	// TODO: Your code goes here
	return StatusType::SUCCESS;
}

output_t<int> world_cup_t::get_player_cards(int playerId)
{
	if (playerId <= 0){
        return StatusType::INVALID_INPUT;
    }
    try {
        std::shared_ptr<Player> player = this->players_hashTable->Search(playerId);
        if (player == nullptr) {
            return StatusType::FAILURE;
        }
        return output_t<int>(player->cards);
    }
    catch (const std::bad_alloc &) {return  StatusType::ALLOCATION_ERROR;}
}

output_t<int> world_cup_t::get_team_points(int teamId)
{
	/*if (teamId <= 0){
        return StatusType::INVALID_INPUT;
    }
    try
    {
        std::shared_ptr<Team> newTeam(new Team(teamId));
        auto* teamnode = this->team_tree_by_id->Find(newTeam);
        if (teamnode == nullptr){
            return StatusType::FAILURE;
        }
        else{
            return output_t<int>(teamnode->GetValue()->points);
        }
    } catch (const std::bad_alloc &) {return  StatusType::ALLOCATION_ERROR;}*/
}

output_t<int> world_cup_t::get_ith_pointless_ability(int i)
{
	// TODO: Your code goes here
	return 12345;
}

output_t<permutation_t> world_cup_t::get_partial_spirit(int playerId)
{
	// TODO: Your code goes here
	return permutation_t();
}

StatusType world_cup_t::buy_team(int teamId1, int teamId2)
{
	// TODO: Your code goes here
	return StatusType::SUCCESS;
}
