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
    int games_played;
    int ability;
    int cards;
    bool isGoalie;
    std::shared_ptr<Team> team;
    std::shared_ptr<Player> parent;
    permutation_t fixed_spirit; // all except the root spirit
    bool teamDeleted; // if his team deleted, the bool is true, and he can't join other team
    // הקבוצה שקונים אותה מחסירים בשדה פיקס את מספר המשחקים של שקנתה
    // לכל שחקן הפיקס הוא המשספר משחקים שלו פחות השורש אלא אם הוא השורש זה המשחקים שלו בלבד
    // מעדכנים משחקים גם בשורש




    Player(int playerId, int teamId, const permutation_t &spirit, int gamesplayed, int ability, int cards, bool
    isGoalKeeper);

    ~Player() = default;


    static int comparePlayerId(const std::shared_ptr<Player> &a, const std::shared_ptr<Player> &b);

    //static int comparePlayerGoalsCardsId(const std::shared_ptr<Player> &a, const std::shared_ptr<Player> &b);
    void Union(std::shared_ptr<Team> &buying_team, std::shared_ptr<Team> &acquired_team);
    void UnionBuyingEmpty(std::shared_ptr<Team> &buying_team, std::shared_ptr<Team> &acquired_team);
    void UnionAcquiredEmpty(std::shared_ptr<Team> &buying_team, std::shared_ptr<Team> &acquired_team);
    std::shared_ptr<Player> Find();

    void setTeam(std::shared_ptr<Team> t);

};
#endif //SOLUTION_PLAYER_H