
#ifndef DS2_AVLRANKTREEE_H
#define DS2_AVLRANKTREEE_H


#include <iostream>
//#include <cassert>
#include "Player.h"

template<class Key,class Value>
class AVLPlayerNode {

    Value data;
    AVLPlayerNode<Key,Value> *left_son;
    AVLPlayerNode<Key,Value> *right_son;
    AVLPlayerNode<Key,Value> *parent;
    int height;
    int counter_players_in_subtree;

public:

    AVLPlayerNode() : data(nullptr), left_son(nullptr), right_son(nullptr), parent(nullptr), height(0), counter_players_in_subtree(0){}

    explicit AVLPlayerNode(const Value &value) : data(value), left_son(nullptr), right_son(nullptr), parent(nullptr), height(0), counter_players_in_subtree(1) {}

    ~AVLPlayerNode();

    const AVLPlayerNode<Key,Value> *GetMinNode() const;

    Value GetValue() const { return data; }

    void SetValue(const Value &new_data);

    void SetLeft(AVLPlayerNode<Key,Value> *left) { left_son = left; }

    AVLPlayerNode<Key,Value> *GetLeft() const { return left_son; }

    void SetRight(AVLPlayerNode<Key,Value> *right) { right_son = right; }

    AVLPlayerNode<Key,Value> *GetRight() const { return right_son; }

    void SetParent(AVLPlayerNode<Key,Value> *parent_) { parent = parent_; }

    AVLPlayerNode<Key,Value> *GetParent() const { return parent; }

    void SetHeight(int new_height) { this->height = new_height; }

    int GetHeight() const { return this->height; }

    void UpdateHeight();

    int GetBalanceFactor() const;

    AVLPlayerNode<Key,Value> *GetGreateParent();
    AVLPlayerNode<Key,Value> *GetGreateParentAux(AVLPlayerNode<Key,Value> *node);

    void ToArray(Value *array, int *location);


    int GetCounterPlayerInSub();
    int GetSumLevelsInSub();

    int GetLevel();
};

template<class Key,class Value>
class AVLPlayerTreee {

    AVLPlayerNode<Key,Value> *root;

/*
 * compare return value:
 * 1 - a value is bigger than b
 * 0 - a value is equal than b
 * -1 - a value is smaller than b
 */
    void RotateLeft(AVLPlayerNode<Key,Value> *node);

    void RotateRight(AVLPlayerNode<Key,Value> *B);

    void InsertAux(AVLPlayerNode<Key,Value> *node, AVLPlayerNode<Key,Value> *new_node);


    void RotateByNeeded(AVLPlayerNode<Key,Value> *node);

public:

    explicit AVLPlayerTreee() : root(nullptr) {}

    ~AVLPlayerTreee();

    AVLPlayerNode<Key,Value> *RemoveBinary(AVLPlayerNode<Key,Value> *to_delete, const Value& value);

    bool Insert(const Value &data);

    bool Remove(const Value &data);

    bool IsEmpty();

    const AVLPlayerNode<Key,Value> *GetMinNode() const;

    AVLPlayerNode<Key,Value>* findIndex(AVLPlayerNode<Key, Value> *node, int index);

    AVLPlayerNode<Key,Value> *Find(const Value &value) const;

    AVLPlayerNode<Key,Value> *FindAux(AVLPlayerNode<Key,Value> *node, const Value &value) const;

    AVLPlayerNode<Key,Value> *GetSmallestInTree();

    void merge(Value *arr1, Value *arr2, Value *merge_to, int arr1_size, int arr2_size) const;

    AVLPlayerNode<Key,Value>* GetRoot();

    void SetRoot(AVLPlayerNode<Key,Value> *new_root);

    //static AVLPlayerNode<Key,Value> *FromArrayToTree(Value *array, int start, int end, int *to_fill);

    //static AVLPlayerTreee<Key,Value> *FromArrayToTree(Value *array, int start, int size, int (*comp)(const Value &, const Value &));

    AVLPlayerNode<Key,Value> *FindMinValInTree(AVLPlayerNode<Key,Value> *node);

    AVLPlayerNode<Key,Value> *FindMaxValInTree(AVLPlayerNode<Key,Value> *node);

    int GetTreeSize();

    int GetSizeAux(AVLPlayerNode<Key,Value> *node);

    void ToArray(Value *array);

    int NumOfSmallerThan_Aux(int level, AVLPlayerNode<Key,Value> *node);
    int NumOfSmallerThan(int level);

    double FindHigherMRank(int m);
   double FindHigherMRank_Aux(AVLPlayerNode<Key,Value>* node, int* m);

};


template<class Key,class Value>
void AVLPlayerNode<Key,Value>::ToArray(Value *array, int *location) {
    if (this->GetLeft() != nullptr)
        this->GetLeft()->ToArray(array, location);
    array[*location] = this->GetValue();
    *location = *location + 1;
    if (this->GetRight() != nullptr)
        this->GetRight()->ToArray(array, location);
}

template<class Key,class Value> //Also updating the counter and sum fields
void AVLPlayerNode<Key,Value>::UpdateHeight() {

    int left_height = 0;
    int right_height = 0;

    //reset the fields
    this->counter_players_in_subtree = 1;

    if (right_son == nullptr) {
        right_height = -1;
    } else {
        right_height = right_son->GetHeight();
        this->counter_players_in_subtree += right_son->counter_players_in_subtree;

    }

    if (left_son == nullptr) {
        left_height = -1;
    } else {
        left_height = left_son->GetHeight();
        this->counter_players_in_subtree += left_son->counter_players_in_subtree;
    }

    this->height = (std::max(left_height, right_height) + 1);
}


template<class Key,class Value>
int AVLPlayerNode<Key,Value>::GetBalanceFactor() const {
    int left_height, right_height;
    if (left_son == nullptr) {
        left_height = -1;
    } else {
        left_height = left_son->GetHeight();
    }
    if (right_son == nullptr) {
        right_height = -1;
    } else {
        right_height = right_son->GetHeight();
    }
    return left_height - right_height;
}

template<class Key,class Value>
AVLPlayerNode<Key,Value> *AVLPlayerNode<Key,Value>::GetGreateParent() {
    if (this->parent == nullptr)
        return this;
    return GetGreateParentAux(this->parent);
}

template<class Key,class Value>
AVLPlayerNode<Key,Value> *AVLPlayerNode<Key,Value>::GetGreateParentAux(AVLPlayerNode<Key,Value> *node) {
    if (node->parent == nullptr)
        return node;
    return GetGreateParentAux(node->parent);
}

template<class Key,class Value>
AVLPlayerNode<Key,Value>::~AVLPlayerNode() {
    delete left_son;
    delete right_son;
}

template<class Key,class Value>
void AVLPlayerNode<Key,Value>::SetValue(const Value &new_data) {
    this->data = new_data;
}

template<class Key,class Value>
const AVLPlayerNode<Key,Value> *AVLPlayerNode<Key,Value>::GetMinNode() const {
    if (GetLeft() == nullptr)
        return this;
    else
        return GetLeft()->GetMinNode();
}
/*

template<class Key,class Value>
int AVLPlayerNode<Key,Value>::GetLevel() {
    return ((std::shared_ptr<Player>)(this->GetValue()))->GetLevel();
}
*/

template<class Key,class Value>
int AVLPlayerNode<Key,Value>::GetCounterPlayerInSub() {
    return this->counter_players_in_subtree;
}

/*
template<class Key,class Value>
int AVLPlayerNode<Key,Value>::GetSumLevelsInSub() {
    return this->;
}
*/


template<class Key,class Value>
AVLPlayerNode<Key,Value> *AVLPlayerTreee<Key,Value>::Find(const Value &value) const {
    AVLPlayerNode<Key,Value> *tmp = this->root;
    return FindAux(tmp, value);
}


// basic rotations
template<class Key,class Value>
void AVLPlayerTreee<Key,Value>::RotateLeft(AVLPlayerNode<Key,Value> *B) {
    if (B == nullptr) { return; }

    AVLPlayerNode<Key,Value> *A = B->GetRight();


    AVLPlayerNode<Key,Value> *A_L = A->GetLeft();
    B->SetRight(A_L);
    //if(new_root->GetRight() != NULL) {new_root->GetRight()->SetParent(root);}
    A->SetLeft(B);

    if (B->GetParent() == nullptr) {
        this->root = A;
    } else {
        if (B->GetParent()->GetLeft() == B) {
            B->GetParent()->SetLeft(A);
        } else {
            B->GetParent()->SetRight(A);
        }
    }
    A->SetParent(B->GetParent());
    B->SetParent(A);

    if (A_L != nullptr) {
        A_L->SetParent(B);
    }
    B->UpdateHeight();
    A->UpdateHeight();
}

template<class Key,class Value>
void AVLPlayerTreee<Key,Value>::RotateRight(AVLPlayerNode<Key,Value> *B) {
    if (B == nullptr) { return; }

    AVLPlayerNode<Key,Value> *A = B->GetLeft();


    AVLPlayerNode<Key,Value> *A_R = A->GetRight();
    B->SetLeft(A_R);
    if (A_R != nullptr) {
        A_R->SetParent(B);
    }
    //if(new_root->GetRight() != NULL) {new_root->GetRight()->SetParent(root);}
    A->SetRight(B);

    if (B->GetParent() == nullptr) {
        this->root = A;
    } else {
        if (B->GetParent()->GetLeft() == B) {
            B->GetParent()->SetLeft(A);
        } else {
            B->GetParent()->SetRight(A);
        }
    }
    A->SetParent(B->GetParent());
    B->SetParent(A);

    B->UpdateHeight();
    A->UpdateHeight();
}


//we return true if the insert succeeded
template<class Key,class Value>
bool AVLPlayerTreee<Key,Value>::Insert(const Value &data) {

    //check if this value already exists

    if (this->Find(data)) {
        return false;
    }

    auto *new_node = new AVLPlayerNode<Key,Value>(data);


    if (new_node == nullptr) {
        return false;
    }

    //if our tree is empty
    if (this->root == nullptr) {
        this->root = new_node;
    } else {
        InsertAux(this->root, new_node);
    }

    return true;
}


template<class Key,class Value>
void AVLPlayerTreee<Key,Value>::InsertAux(AVLPlayerNode<Key,Value> *node, AVLPlayerNode<Key,Value> *new_node) {
    if (Key()(node->GetValue(), new_node->GetValue()) == 1) {

        //if we don't have a left child
        if (node->GetLeft() == nullptr) {
            node->SetLeft(new_node);
            new_node->SetParent(node);
        } else { //we need to keep looking in the left tree
            InsertAux(node->GetLeft(), new_node);
        }
    } else {
        if (node->GetRight() == nullptr) {
            node->SetRight(new_node);
            new_node->SetParent(node);
        } else {
            InsertAux(node->GetRight(), new_node);
        }
    }
    //the function updates the node according to the rotates that are needed
    RotateByNeeded(node);

//we need to update the height
}


//find smallest value in tree
template<class Key,class Value>
AVLPlayerNode<Key,Value> *AVLPlayerTreee<Key,Value>::FindMinValInTree(AVLPlayerNode<Key,Value> *node) {
    if (node == nullptr)
        return node;

    if (node->GetLeft() == nullptr)
        return node;
    return FindMinValInTree(node->GetLeft());
}


template <class Key,class Value>
AVLPlayerNode<Key,Value>* AVLPlayerTreee<Key,Value>::RemoveBinary(AVLPlayerNode<Key,Value>* node, const Value& data)
{
    AVLPlayerNode<Key,Value>* to_delete = Find(data);
    //start with the greatest if shrink the next conditions
    AVLPlayerNode<Key,Value>* big_parent = nullptr;

    //if to_delete has two sons
    if (to_delete->GetLeft() && to_delete->GetRight()) {
        //here can be minVal
        AVLPlayerNode<Key,Value>* new_root = FindMinValInTree(to_delete->GetRight());

        //switching the data is soooo smart !!!
        to_delete->SetValue(new_root->GetValue());
        to_delete = new_root;
        if (to_delete->GetRight()) {
            big_parent = to_delete->GetRight();
        }
    }

    else if (to_delete->GetLeft()) {
        big_parent = to_delete->GetLeft();
    } else if (to_delete->GetRight()) {
        big_parent = to_delete->GetRight();
    }

    //All cases had the same below logic in the end - set new child to the parent
    AVLPlayerNode<Key,Value>* parent = to_delete->GetParent();
    if (parent == nullptr) {
        root = big_parent;
    } else if (to_delete->GetParent()->GetLeft() == to_delete) {
        to_delete->GetParent()->SetLeft(big_parent);
    } else {
        to_delete->GetParent()->SetRight(big_parent);
    }
    if (big_parent != nullptr) {
        big_parent->SetParent(parent);
    }

    //deleting the node
    to_delete->SetLeft(nullptr);
    to_delete->SetRight(nullptr);
    delete to_delete;
    return parent;
}

template<class Key,class Value>
void AVLPlayerTreee<Key,Value>::RotateByNeeded(AVLPlayerNode<Key,Value> *node) {
    int balance_root = node->GetBalanceFactor();

    //if an LL is needed
    if (balance_root == 2 && (node->GetLeft())->GetBalanceFactor() >= 0) {
        RotateRight(node);
        //if an LR is needed
    } else if (balance_root == 2 && (node->GetLeft())->GetBalanceFactor() == -1) {
        RotateLeft(node->GetLeft());
        RotateRight(node);
        return;
        //if an RL is needed
    } else if (balance_root == -2 && (node->GetRight())->GetBalanceFactor() == 1) {
        RotateRight(node->GetRight());
        RotateLeft(node);
        return;
        //if an RR is needed
    } else if (balance_root == -2 && (node->GetRight())->GetBalanceFactor() <= 0) {
        RotateLeft(node);
        return;
    }

    node->UpdateHeight();
}


template<class Key,class Value>
bool AVLPlayerTreee<Key,Value>::Remove(const Value &data) {
    if (root == nullptr)//if root is empty
        return false;


    AVLPlayerNode<Key,Value> *to_delete = this->Find(data);
    if (to_delete == nullptr)//if data needed to delete doesn't exist
    {
        return false;
    }

    //regular binary tree remove
    AVLPlayerNode<Key,Value> *parent_of_deleted = RemoveBinary(root, data);

    while (parent_of_deleted != nullptr) {
        RotateByNeeded(parent_of_deleted);
        parent_of_deleted = parent_of_deleted->GetParent();
    }
    return true;

}


template<class Key,class Value>
int AVLPlayerTreee<Key,Value>::GetTreeSize() {
    return GetSizeAux(this->root);
}

template<class Key,class Value>
int AVLPlayerTreee<Key,Value>::GetSizeAux(AVLPlayerNode<Key,Value> *node) {
    if (node == nullptr)
        return 0;

    return 1 + GetSizeAux(node->GetLeft()) + GetSizeAux(node->GetRight());
}

template<class Key,class Value>
void AVLPlayerTreee<Key,Value>::ToArray(Value *array) {
    int location = 0;
    if (root != nullptr)
        root->ToArray(array, &location);
}

/*template<class Key,class Value>
AVLPlayerTreee<Key,Value> *AVLPlayerTreee<Key,Value>::FromArrayToTree(Value *array, int start, int size, int (*comp)(const Value &a, const Value &b)) {
    auto tree = new AVLPlayerTreee<Key,Value>(comp);
    int to_fill = size;
    tree->root = FromArrayToTree(array, start, size, &to_fill);
    //tree->Validate();
    return tree;
}*/

/*
template <class Key,class Value>
AVLPlayerNode<Key,Value> *AVLPlayerTreee<Key,Value>::FromArrayToTree(Value *array, int start, int end, int* to_fill) {

    if (*to_fill ==0 || start >end)
    {
        return nullptr;
    }

    int mini_root_index = (start+end)/2;

    auto* mini_root = new AVLPlayerNode<Key,Value>(array[mini_root_index]);
    mini_root->SetParent(nullptr);
    *to_fill = *to_fill -1;

    if (start-end == 0)
    {
        mini_root->SetLeft(nullptr);
        mini_root->SetRight(nullptr);
        mini_root->SetHeight(0);
        return mini_root;
    }


    AVLPlayerNode<Key,Value>* left_son = FromArrayToTree(array, start, mini_root_index - 1, to_fill);

    AVLPlayerNode<Key,Value>* right_son = FromArrayToTree(array, mini_root_index + 1, end, to_fill);



    mini_root->SetLeft(left_son);
    mini_root->SetRight(right_son);
    if (left_son!= nullptr)
    {
        left_son->SetParent(mini_root);
    }
    if (right_son != nullptr)
    {
        right_son->SetParent(mini_root);
    }

    mini_root->UpdateHeight();
    return mini_root;

}

*/

template<class Key,class Value>
AVLPlayerNode<Key,Value> *AVLPlayerTreee<Key,Value>::FindAux(AVLPlayerNode<Key,Value> *node, const Value &value) const {
    if (node == nullptr) {
        return nullptr;
    }
    if (Key()(node->GetValue(), value) == 0) {
        return node;
    }
    if (Key()(value, node->GetValue()) == 1) {
        return FindAux(node->GetRight(), value);
    } else {
        return FindAux(node->GetLeft(), value);
    }

}

template<class Key,class Value>
bool AVLPlayerTreee<Key,Value>::IsEmpty() {
    return this->root == nullptr;
}

template<class Key,class Value>
AVLPlayerNode<Key,Value> *AVLPlayerTreee<Key,Value>::GetSmallestInTree() {
    if (root == nullptr)
        return nullptr;

    AVLPlayerNode<Key,Value> *smallest = root;
    while (smallest->GetLeft() != nullptr) {
        smallest = smallest->GetLeft();
    }
    return smallest;
}

template<class Key,class Value>
AVLPlayerNode<Key,Value> *AVLPlayerTreee<Key,Value>::GetRoot() {
    return this->root;
}

template<class Key,class Value>
void AVLPlayerTreee<Key,Value>::SetRoot(AVLPlayerNode<Key,Value> *new_root) {
    this->root = new_root;
}

template<class Key,class Value>
AVLPlayerNode<Key,Value> *AVLPlayerTreee<Key,Value>::FindMaxValInTree(AVLPlayerNode<Key,Value> *node) {
    if (node->GetRight() == nullptr && node->GetLeft() == nullptr)
        return node;
    return FindMaxValInTree(node->GetRight());
}


template<class Key,class Value>
AVLPlayerTreee<Key,Value>::~AVLPlayerTreee() {
    delete root;
}


template<class Key,class Value>
void AVLPlayerTreee<Key,Value>::merge(Value *arr1, Value *arr2, Value *merge_to, int arr1_size, int arr2_size) const {

    while (arr1_size != 0 && arr2_size != 0) {
        if (Key()(*arr1, *arr2) == -1) {
            *merge_to = *arr1;
            arr1 = arr1 + 1;
            arr1_size = arr1_size - 1;
            merge_to = merge_to + 1;
        } else {
            *merge_to = *arr2;
            arr2 = arr2 + 1;
            arr2_size = arr2_size - 1;
            merge_to = merge_to + 1;
        }
    }

    while (arr1_size != 0) {
        *merge_to = *arr1;
        arr1 = arr1 + 1;
        arr1_size = arr1_size - 1;
        merge_to = merge_to + 1;
    }

    while (arr2_size != 0) {
        *merge_to = *arr2;
        arr2 = arr2 + 1;
        arr2_size = arr2_size - 1;
        merge_to = merge_to + 1;
    }
}

template<class Key,class Value>
const AVLPlayerNode<Key,Value> *AVLPlayerTreee<Key,Value>::GetMinNode() const {
    if (root == nullptr)
        return nullptr;
    return root->GetMinNode();
}



template<class Key,class Value>
int AVLPlayerTreee<Key,Value>::NumOfSmallerThan(int level) {
        return NumOfSmallerThan_Aux(level, this->root);
}

template<class Key,class Value>
int AVLPlayerTreee<Key,Value>::NumOfSmallerThan_Aux(int level, AVLPlayerNode<Key,Value> *node) {

    if(node == nullptr)
        return 0;


    if(node->GetLevel() == level){
        if(node->GetLeft() != nullptr){
            if(node->GetLeft()->GetLevel() == level){
                return NumOfSmallerThan_Aux(level, node->GetLeft());
            }
            else{

                AVLPlayerNode<Key,Value>* tmp_node = node->GetLeft()->GetRight();
                while(tmp_node != nullptr){
                    if(tmp_node->GetLevel() == level){
                        return NumOfSmallerThan_Aux(level, node->GetLeft());
                    }
                    tmp_node = tmp_node->GetRight();
                }

                return node->GetLeft()->GetCounterPlayerInSub();
            }
        }
        else{ //left son is nullptr
            return 0;
        }
    }

    if(node->GetLevel() > level){
        if(node->GetLeft() != nullptr){
            return NumOfSmallerThan_Aux(level, node->GetLeft());
        }
        else{
            return 0;
        }
    } //node->GetLevel < level
    else{
        if(node->GetRight() == nullptr){ //I'm bigger' and it has no right son
            return node->GetCounterPlayerInSub();
        }
        else{ //he has a right son
            if(node->GetLeft() == nullptr){ //there is no one who's smaller besides the curr node
                return 1 + NumOfSmallerThan_Aux(level, node->GetRight());
            }
            else{
                return 1 + node->GetLeft()->GetCounterPlayerInSub() + NumOfSmallerThan_Aux(level, node->GetRight());
            }
        }
    }

}


template<class Key,class Value>
double AVLPlayerTreee<Key,Value>::FindHigherMRank(int m) {

    int * temp_m = new int (m);
    double temp =  FindHigherMRank_Aux(this->root,temp_m);
    delete temp_m;
    return  temp;
}

template<class Key,class Value>
double AVLPlayerTreee<Key,Value>::FindHigherMRank_Aux(AVLPlayerNode<Key,Value> *node, int *m) {
    if (node == nullptr || *m == 0)
    {
        return 0;
    }
    if(node->GetCounterPlayerInSub() == *m)
    {
        *m = *m - node->GetCounterPlayerInSub();
        return node->GetSumLevelsInSub();
    }
    if (node->GetCounterPlayerInSub()>*m)
    {

           double temp_sum =0;
           temp_sum = temp_sum +  FindHigherMRank_Aux(node->GetRight(),m);

        if (*m == 0)
        {
            return temp_sum;
        }
            //because of the current node
            *m =*m - 1;
             temp_sum = temp_sum + node->GetLevel();

        if (*m == 0) {
            return temp_sum;
        }
            return temp_sum +  FindHigherMRank_Aux(node->GetLeft(),m);
    }
    else
    {
        *m = *m - node->GetCounterPlayerInSub();
        return node->GetSumLevelsInSub();
    }


 return 0; // shouldn't get here

}

template<class Key, class Value>
AVLPlayerNode<Key, Value> *AVLPlayerTreee<Key, Value>::findIndex(AVLPlayerNode<Key, Value> *node, int index) {
    if(node->GetLeft() != nullptr) {
        if (node->GetLeft()->GetCounterPlayerInSub() == index - 1) {
            return node;
        }

        if (node->GetLeft()->GetCounterPlayerInSub() > index - 1) {
            return findIndex(node->GetLeft(), index);
        }
        if (node->GetLeft()->GetCounterPlayerInSub() < index - 1) {
            return findIndex(node->GetRight(), (index - node->GetLeft()->GetCounterPlayerInSub() - 1));
        }
    }
    else if (node->GetCounterPlayerInSub() == index){
        return node;
    }
}


#endif //DS2_AVLRANKTREEE_H
