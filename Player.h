//
// Created by Aviv on 11/24/2022.
//

#ifndef SOLUTION_PLAYER_H
#define SOLUTION_PLAYER_H
#include <memory>
#include "wet2util.h"
class Team;


class Player {
public:
    int player_id;
    int team_id;
    int games_played;
    int ability;
    int cards;
    bool isGoalie;
    Team* team;
    Player *parent;
    permutation_t spirit;
    bool teamDeleted; // if his team deleted, the bool is false, and he can't join other team



    Player(int playerId, int teamId, const permutation_t& spirit, int gamesplayed, int ability, int cards, bool
    isGoalKeeper);
    ~Player() = default;


    static int comparePlayerId(const std::shared_ptr<Player> &a, const std::shared_ptr<Player> &b);
    //static int comparePlayerGoalsCardsId(const std::shared_ptr<Player> &a, const std::shared_ptr<Player> &b);


};



#endif //SOLUTION_PLAYER_H