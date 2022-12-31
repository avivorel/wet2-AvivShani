//
// Created by Aviv on 11/24/2022.
//

#ifndef SOLUTION_TEAM_H
#define SOLUTION_TEAM_H

#include "AVLTree.h"
#include "Player.h"
#include <memory>
#include <iostream>

class Team {
public:

    int team_id;
    int games_played;
    int numberOfPlayers;
    int numberOfGK;
    bool hasGK;
    //int playMatchEquation;
    Player* root_player;
    int points;
    permutation_t team_spirit;
    int team_ability;
    //bool is_teamactive;


    static int compareTeamId(const std::shared_ptr<Team> &a, const std::shared_ptr<Team> &b);
    explicit Team(int teamid);
    void add_player(const std::shared_ptr<Player>& playerToAdd);


};


#endif //SOLUTION_TEAM_H