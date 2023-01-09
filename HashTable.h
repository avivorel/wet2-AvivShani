#ifndef DS2_HASHTABLE_H
#define DS2_HASHTABLE_H

#include "Player.h"
#include <cstdlib>

#include "LinkedList.h"

class HashTable{

    private:
    int array_size;
    int elements_counter;
    LinkedList<std::shared_ptr<Player>>** arr;
    int hashFunction(int i) const;

    public:

    HashTable();

    void Insert(const std::shared_ptr<Player>& p);

    std::shared_ptr<Player> Search(int key);

    void Delete(int key);

    void resize();

    ~HashTable();

};

#endif //DS2_HASHTABLE_H
