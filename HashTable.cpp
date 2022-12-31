#include "HashTable.h"

int HashTable::hashFunction(int i) const {
    return (i % this->arr_size);
}

HashTable::HashTable():arr(nullptr), keys_counter(0), arr_size(5) {
    this->arr = new LinkedList<std::shared_ptr<Player>>*[5];
    for (int i = 0; i < 5; ++i) {
        this->arr[i] = nullptr;
    }
}

std::shared_ptr<Player> HashTable::Search(int key) {
    int index = HashFunction(key);
    if(this->arr[index] == nullptr)
    {
        return nullptr;
    }
    LinkedList<std::shared_ptr<Player>>* players_list = this->arr[index];
    Node<std::shared_ptr<Player>> player_iterator = players_list->getHead();
    while(player_iterator != nullptr and player_iterator.data->player_id != key){
        players_iterator = player_iterator.next;
    }
    return player_iterator.data;
}

void HashTable::Delete(int key) {
    int index = HashFunction(key);
    if(this->arr[index] == nullptr){
        return;
    }
    auto* players_list = arr[index];
    Node<std::shared_ptr<Player>> player_iterator = players_list->getHead();

    while(player_iterator != nullptr and player_iterator.data->player_id != key)
    {
        players_iterator = player_iterator.next;
    }

    if (player_iterator == nullptr)
    {
     return;
    }
    players_list->remove(player_iterator);
    if (players_list->getHead() == nullptr)
    {
        delete players_list;
        arr[index]= nullptr;
    }
    this->keys_counter--;
    if ( this->keys_counter == (this->arr_size / 4))
    {
        this->resize();
    }
}




void HashTable::resize() {

    int new_size;

    if (this->arr_size == this->keys_counter) {
        new_size = 2 * this->arr_size;
    }
    if (this->keys_counter == (this->arr_size / 4)) {
        new_size = (this->arr_size / 2);
    }

    auto** resized_arr = new LinkedList<std::shared_ptr<Player>>*[new_size] ;
    for (int k = 0; k < new_size; ++k) {
        resized_arr[k] = nullptr;
    }

    for (int i = 0; i < this->arr_size; ++i) {
        if (arr[i] != nullptr)
        {
            auto* players_list = arr[i];
            auto* player_iterator = players_list->getTail();
            while (player_iterator != nullptr){
                int index = player_iterator->data->player_id % new_size;
                if (resized_arr[index] == nullptr){
                    resized_arr[index] = new LinkedList<std::shared_ptr<Player>>;
                }
                resized_arr[index]->insert(player_iterator->data);
                players_list->remove(player_iterator);
                player_iterator = player_iterator->prev;
            }
            delete arr[i];
        }

    }
    delete[] this->arr;
    this->arr = resized_arr;
    this->arr_size = new_size;
}


void HashTable::Insert(const std::shared_ptr<Player>& p) {
    int index = HashFunction(p->player_id);
    this->keys_counter++;

    if (this->arr[index] == nullptr)
    {
        this->arr[index] = new LinkedList<std::shared_ptr<Player>>;
    }
    auto* players_list =  this->arr[index];
    players_list->insert(p);
    if(this->keys_counter == this->arr_size )
    {
        this->resize();
    }
}

HashTable::~HashTable() {
    for (int i = 0; i < this->arr_size; ++i) {
        delete arr[i];
    }
    delete[] arr;
}

