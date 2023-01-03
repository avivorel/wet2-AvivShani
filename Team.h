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
    std::shared_ptr<Player> root_player;
    int points;
    permutation_t teamSpirit_without_root;
    int team_ability;


    static int compareTeamId(const std::shared_ptr<Team> &a, const std::shared_ptr<Team> &b);
    explicit Team(int teamid);
    void add_player(const std::shared_ptr<Player>& playerToAdd);

};

class compareTeamId{
    int operator()(const std::shared_ptr<Team> &a, const std::shared_ptr<Team> &b)
    {
        if(a->team_id > b->team_id) return 1;
        if(a->team_id < b->team_id) return -1;
        return 0;
    }
};
class compareTeamAbility{
    int operator()(const std::shared_ptr<Team> &a, const std::shared_ptr<Team> &b){
        if (a->team_ability == b->team_ability){
            if (a->team_id == b->team_id){
                return 0;
            }
            return (a->team_id > b->team_id) ? 1:-1;
        }
        else if (a->team_ability > b->team_ability){
            return 1;
        }
        return -1;
    }
};

#endif //SOLUTION_TEAM_H