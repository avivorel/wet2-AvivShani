#include "worldcup23a2.h"


world_cup_t::world_cup_t()
{
    team_tree_by_id = new RankAVLTeamTree<compareTeamId,std::shared_ptr<Team>>;
    team_tree_by_ability = new RankAVLTeamTree<compareTeamAbility,std::shared_ptr<Team>>;
    players_hashTable= new HashTable();
    numofTeams=0;
}

world_cup_t::~world_cup_t()
{
    delete team_tree_by_ability;
    delete team_tree_by_id;
    delete players_hashTable;
}

StatusType world_cup_t::add_team(int teamId)
{
    if (teamId <= 0)
    {
        return StatusType::INVALID_INPUT;
    }
    try {
        std::shared_ptr<Team> teamFinder(new Team(teamId));

        auto *foundTeam = this->team_tree_by_id->Find(teamFinder);
        if (foundTeam != nullptr)
        {
            return StatusType::FAILURE;
        }

        this->team_tree_by_id->Insert(teamFinder);
        this->team_tree_by_ability->Insert(teamFinder);
        numofTeams++;

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
        auto *team_toRemove = this->team_tree_by_id->Find(toRemove);

        if (team_toRemove == nullptr)
        {
            return StatusType::FAILURE;
        }

        std::shared_ptr<Team> actual_team1 = team_toRemove->GetTeam();

        auto *team_toRemove_ability = this->team_tree_by_ability->Find(actual_team1);

        if (team_toRemove_ability != nullptr)
        {
            std::shared_ptr<Team> team_ability = team_toRemove_ability->GetTeam();
            this->team_tree_by_ability->Remove(team_ability);

        }


        if (actual_team1->numberOfPlayers != 0)
        {
            std::shared_ptr<Player> players_root = actual_team1->root_player.lock()->Find();

            if (players_root == nullptr)
            {
                actual_team1->root_player.lock()->team = nullptr;
                actual_team1->root_player.lock()->teamDeleted = true; // need more?
                actual_team1->root_player.lock() = nullptr; // ?
            }
            else
            {
                players_root->team = nullptr;
                players_root->teamDeleted = true; // need more?
            }
        }

        this->team_tree_by_id->Remove(team_toRemove->GetTeam());
        numofTeams--;
    }
    catch (const std::bad_alloc &) { return  StatusType::ALLOCATION_ERROR;}

    return StatusType::SUCCESS;

}

StatusType world_cup_t::add_player(int playerId, int teamId, const permutation_t &spirit, int gamesPlayed,
                                   int ability, int cards, bool goalKeeper)
{
    if (teamId <= 0 || playerId <= 0 || (!(spirit.isvalid())) || gamesPlayed<0 || cards< 0)
    {
        return StatusType::INVALID_INPUT;
    }

    try
    {
        if(this->players_hashTable->Search(playerId) != nullptr) { //if the team already exists
           return StatusType::FAILURE;
        }
        //creating the team's shared_ptr
        std::shared_ptr<Player> player = std::make_shared<Player>(playerId, teamId, spirit, gamesPlayed,ability
                ,cards,goalKeeper);

        // checking if the team is exists
        std::shared_ptr<Team> team(new Team(teamId));
        auto *foundteam = this->team_tree_by_id->Find(team);
        if (foundteam == nullptr)
        {
            return StatusType::FAILURE;
        }

        players_hashTable->Insert(player);


        /// for index::::
        std::shared_ptr<Team> toRemove(new Team(teamId));
        auto *team_toRemove = this->team_tree_by_id->Find(toRemove);
        std::shared_ptr<Team> Removeteam = team_toRemove->GetTeam();
        this->team_tree_by_ability->Remove(Removeteam);


        foundteam->GetTeam()->add_player(player);

        if (foundteam->GetTeam()->numberOfPlayers == 1)
        {
            player->team= foundteam->GetTeam();
            player->parent= nullptr;
            foundteam->GetTeam()->rootSpirit = spirit;
            foundteam->GetTeam()->teamSpirit= spirit;
        }
        else
        {

            player->parent = (foundteam->GetTeam()->root_player.lock()->Find() != nullptr) ?
                             foundteam->GetTeam()->root_player.lock()->Find() : foundteam->GetTeam()->root_player.lock();

            foundteam->GetTeam()->teamSpirit = foundteam->GetTeam()->teamSpirit * spirit;

            player->fixed_spirit = player->parent->fixed_spirit.inv() * foundteam->GetTeam()->teamSpirit;

            player->games_played= gamesPlayed - player->parent->games_played;

        }

        this->team_tree_by_ability->Insert(Removeteam);

    } catch (const std::bad_alloc &) { return  StatusType::ALLOCATION_ERROR;}
    return StatusType::SUCCESS;
}

output_t<int> world_cup_t::play_match(int teamId1, int teamId2)
{ // ?????????? ?????? ?????????????? ???? ??????????, ?????????? ?????? ?????? ???????? ???? ????
    // ?????????????? ???????????? ???? ??????????
    if (teamId2 <= 0  or teamId1 <= 0 or teamId2 == teamId1)
    {
        return StatusType::INVALID_INPUT;
    }


    try {
        std::shared_ptr<Team> team1(new Team(teamId1));
        std::shared_ptr<Team> team2(new Team(teamId2));
        auto *found_team_1 = this->team_tree_by_id->Find(team1);
        auto *found_team_2 = this->team_tree_by_id->Find(team2);
        if (found_team_1 == nullptr or found_team_2 == nullptr)
        {
            return StatusType::FAILURE;
        }

        std::shared_ptr<Team> real_team_1 = found_team_1->GetTeam();
        std::shared_ptr<Team> real_team_2 = found_team_2->GetTeam();
        if (!real_team_1->hasGK or !real_team_2->hasGK)
        {
            return StatusType::FAILURE;
        }

        // actual match playing
        int team1_ability = real_team_1->points + real_team_1->team_ability;
        int team2_ability = real_team_2->points + real_team_2->team_ability;

        real_team_1->games_played += 1;
        real_team_2->games_played += 1;// adding 1 to the games count of the team and root.

        std::shared_ptr<Player> real_team1_root = real_team_1->root_player.lock()->Find();
        if (real_team1_root != nullptr){
            real_team1_root->games_played += 1;
        }
        else {
            real_team_1->root_player.lock()->games_played += 1;
        }
        std::shared_ptr<Player> real_team2_root = real_team_2->root_player.lock()->Find();
        if (real_team2_root != nullptr){
            real_team2_root->games_played += 1;
        }
        else {
            real_team_2->root_player.lock()->games_played += 1;
        }



        if (team1_ability > team2_ability){
            real_team_1->points += 3;
            return {1};
        }

        else if ( team2_ability > team1_ability){
            real_team_2->points += 3;
            return {3};
        }

        else{
            permutation_t team1_spirit = real_team_1->teamSpirit;
            permutation_t team2_spirit = real_team_2->teamSpirit;
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
}

output_t<int> world_cup_t::num_played_games_for_player(int playerId)
{

    if (playerId <= 0)
    {
        return StatusType::INVALID_INPUT;
    }

    try {
        if (this->players_hashTable->Search(playerId) == nullptr)
        {
            return StatusType::FAILURE;
        }

        std::shared_ptr<Player> player = this->players_hashTable->Search(playerId);
        if (player->parent == nullptr){//is the root
            return {player->games_played};
        }
        std::shared_ptr<Player> players_root = player->Find();
        return {player->games_played+ players_root->games_played}; // ?????????? ?????? ??????????


    } catch (const std::bad_alloc &) {return  StatusType::ALLOCATION_ERROR;}

}

StatusType world_cup_t::add_player_cards(int playerId, int cards)
{
    if (playerId <= 0 or cards < 0){
        return StatusType::INVALID_INPUT;
    }

    try {
        if (this->players_hashTable->Search(playerId) == nullptr)
        {
            return StatusType::FAILURE;
        }

        std::shared_ptr<Player> player = this->players_hashTable->Search(playerId);
        std::shared_ptr<Player> players_root = player->Find();
        if (player->parent == nullptr and player->team == nullptr){
            return StatusType::FAILURE;
        }
        if (players_root != nullptr and players_root->teamDeleted)
        {
            return StatusType::FAILURE;
        }

        player->cards += cards;
    } catch (const std::bad_alloc &) {return  StatusType::ALLOCATION_ERROR;}
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
        return {player->cards};
    }
    catch (const std::bad_alloc &) {return  StatusType::ALLOCATION_ERROR;}
}

output_t<int> world_cup_t::get_team_points(int teamId)
{
    if (teamId <= 0){
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
            return {teamnode->GetTeam()->points};
        }
    } catch (const std::bad_alloc &) {return  StatusType::ALLOCATION_ERROR;}
}

output_t<int> world_cup_t::get_ith_pointless_ability(int i)
{
    if (numofTeams == 0 || i<0 || i>= numofTeams )
    {
        return StatusType::FAILURE;
    }
    try
    {
        auto* newTeam = team_tree_by_ability->findIndex(team_tree_by_ability->GetRoot(), i+1 );
        return {newTeam->GetTeam()->team_id};
    } catch (const std::bad_alloc &){return  StatusType::ALLOCATION_ERROR;}

}

output_t<permutation_t> world_cup_t::get_partial_spirit(int playerId)
{
    if (playerId <= 0)
    {
        return StatusType::INVALID_INPUT;
    }

    try {
        if (this->players_hashTable->Search(playerId) == nullptr)
        {
            return StatusType::FAILURE;
        }

        std::shared_ptr<Player> player = this->players_hashTable->Search(playerId);
        std::shared_ptr<Player> players_root = player->Find();
        if (player->parent == nullptr and player->team == nullptr){
            return StatusType::FAILURE;
        }
        if (players_root != nullptr && players_root->teamDeleted)
        {
            return StatusType::FAILURE;
        }
        if (players_root == nullptr)
        {
            return {player->fixed_spirit};
        }
        return { players_root->fixed_spirit * player->fixed_spirit}; /// how to return

    } catch (const std::bad_alloc &){return  StatusType::ALLOCATION_ERROR;}
}

StatusType world_cup_t::buy_team(int teamId1, int teamId2)
{
    /// team1= buyer , team2= bought
    if (teamId1 <= 0 || teamId2 <= 0 || teamId1 == teamId2)
    {
        return StatusType::INVALID_INPUT;
    }

    try {
        std::shared_ptr<Team> team1(new Team(teamId1));
        std::shared_ptr<Team> team2(new Team(teamId2));
        auto *found_team_1 = this->team_tree_by_id->Find(team1);
        auto *found_team_2 = this->team_tree_by_id->Find(team2);

        if (found_team_1 == nullptr || found_team_2 == nullptr)
        {
            return StatusType::FAILURE;
        }
        std::shared_ptr<Team> actual_team1 = found_team_1->GetTeam();
        std::shared_ptr<Team> actual_team2 = found_team_2->GetTeam();

        if(actual_team2->numberOfPlayers == 0 )
        {
            this->team_tree_by_id->Remove(team2);
            this->team_tree_by_ability->Remove(actual_team2);
        }

        else if (actual_team1->numberOfPlayers == 0 and actual_team2->numberOfPlayers != 0)
        {// ?????????? ???????? ???????? ???????? ???? ???????????????? ???????? ????????????, ?????????? ?????????????? ????????
            this->team_tree_by_ability->Remove(actual_team1);
            this->team_tree_by_ability->Remove(actual_team2);
            this->team_tree_by_id->Remove(team2);

            actual_team2->root_player.lock()->UnionBuyingEmpty(actual_team1, actual_team2);
            this->team_tree_by_ability->Insert(actual_team1);
        }

        else if (actual_team1->numberOfPlayers != 0 and actual_team2->numberOfPlayers != 0)
        {
            this->team_tree_by_ability->Remove(actual_team1);
            this->team_tree_by_id->Remove(team2);
            this->team_tree_by_ability->Remove(actual_team2);

            actual_team1->root_player.lock()->Union(actual_team1, actual_team2);

            this->team_tree_by_ability->Insert(actual_team1);
        }

        if (!actual_team1->hasGK and actual_team2->hasGK){
            actual_team1->hasGK = true;
        }

        actual_team1->points += actual_team2->points;
        numofTeams--;
        if (actual_team1->root_player.lock() != nullptr){
            actual_team1->root_player.lock()->Find();
        }
    } catch (const std::bad_alloc &) { return  StatusType::ALLOCATION_ERROR;}

    return StatusType::SUCCESS;
}