#include "HashTable.h"

int HashTable::HashFunction(int i) const {
    return (i % this->arr_size);
}


HashTable::HashTable():arr(nullptr), keys_counter(0), arr_size(5) {
    this->arr = new AVLTree<std::shared_ptr<Player>>*[5];
    for (int i = 0; i < 5; ++i) {
        this->arr[i] = nullptr;
    }
}

std::shared_ptr<Player> HashTable::Member(int key) {
    int index = HashFunction(key);

    if(this->arr[index] == nullptr)
    {
        return nullptr;
    }

    AVLTree<std::shared_ptr<Player>>* player_tree = this->arr[index];

    std::shared_ptr<Player> dummy_player(new Player(key,0,permutation_t(),0,0,0,0));

    AVLNode<std::shared_ptr<Player>>* temp_node =  player_tree->Find(dummy_player);
    if (temp_node == nullptr)
    {
        return nullptr;
    }

    return temp_node->GetValue();

}

void HashTable::Delete(int key) {
    int index = HashFunction(key);
    //if his cell is empty - means he wasn't inserted
    if ((AVLTree<std::shared_ptr<Player>>*)arr[index] == nullptr)
    {
        return;
    }

    auto* temp_tree = (AVLTree<std::shared_ptr<Player>>*)(arr[index]);
    std::shared_ptr<Player> dummy_player(new Player(key,0,permutation_t(),0,0,0,0));
    AVLNode<std::shared_ptr<Player>>* temp_node = temp_tree->Find(dummy_player);

    //if he isn't in tree - means he wasn't inserted
    if (temp_node == nullptr)
    {
        return;
    }


    //now for sure he exists and we need to delete him
    temp_tree->Remove(dummy_player);
    if (temp_tree->IsEmpty())
    {
        delete temp_tree;
        arr[index] = nullptr;
    }

    this->keys_counter--;
    if ( this->keys_counter == (this->arr_size / 4))
    {
        this->resize();
    }
}




void HashTable::resize() {

    int new_size;

    //if we are making it bigger
    if (this->arr_size == this->keys_counter) {
        new_size = 2 * this->arr_size;
    }
    if (this->keys_counter == (this->arr_size / 4)) {
        new_size = (this->arr_size / 2);
    }

    auto** resized_arr = new AVLTree<std::shared_ptr<Player>>*[new_size] ;
    for (int k = 0; k < new_size; ++k) {
        resized_arr[k] = nullptr;
    }

    for (int i = 0; i < this->arr_size; ++i) {
        if (arr[i] != nullptr) {
            std::shared_ptr<Player>* temp_arr = new std::shared_ptr<Player>[arr[i]->GetTreeSize()];


            (((AVLTree<std::shared_ptr<Player>>*)(arr[i]))->ToArray(temp_arr));

            int j = 0;
            for (int j = 0; j < arr[i]->GetTreeSize(); ++j) {
                std::shared_ptr<Player> my_player = temp_arr[j];
                int index = my_player->player_id % new_size;

                if (resized_arr[index] == nullptr) {
                    resized_arr[index] = new AVLTree<std::shared_ptr<Player>>(Player::comparePlayerId);
                }
                auto* players_tree =  (AVLTree<std::shared_ptr<Player>>*)(resized_arr[index]);
                players_tree->Insert(my_player);
            }

            delete[] temp_arr;
            delete arr[i];
        }


    }
    delete [] this->arr;
    this->arr = resized_arr;
    this->arr_size = new_size;

}


void HashTable::Insert(const std::shared_ptr<Player>& p) {
    int index = HashFunction(p->player_id);
    this->keys_counter++;

    //if he doesn't exist we create one
    if (this->arr[index] == nullptr)
    {
        this->arr[index] = new AVLTree<std::shared_ptr<Player>>(Player::comparePlayerId);
    }

    auto* players_tree =  (AVLTree<std::shared_ptr<Player>>*)(this->arr[index]);
    players_tree->Insert(p);

    //busy factor
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

