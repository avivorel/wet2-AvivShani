//
// Created by ofek on 05/01/2022.
//


#ifndef DS2_HASHTABLE_H
#define DS2_HASHTABLE_H

#include "Player.h"
#include "AVLTree.h"
#include <cstdlib>


class HashTable{

    private:
    int arr_size;
    int keys_counter;
    AVLTree<std::shared_ptr<Player>>** arr;

    int HashFunction(int i) const;

    public:

    HashTable();

    void Insert(const std::shared_ptr<Player>& p);

    std::shared_ptr<Player> Member(int key);

    void Delete(int key);

    void resize();

    ~HashTable();

};

#endif //DS2_HASHTABLE_H
