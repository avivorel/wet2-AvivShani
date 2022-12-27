#ifndef DS2_AVLRANKTREEE_H
#define DS2_AVLRANKTREEE_H


#include <iostream>
//#include <cassert>
#include "Player.h"

template<class T>
class AVLPlayerNode {

    T data;
    AVLPlayerNode<T> *left_son;
    AVLPlayerNode<T> *right_son;
    AVLPlayerNode<T> *parent;
    int height;

    int counter_players_in_subtree;
    int sum_levels_in_subtree;

public:

    AVLPlayerNode() : data(nullptr), left_son(nullptr), right_son(nullptr), parent(nullptr), height(0), counter_players_in_subtree(0), sum_levels_in_subtree(0) {}

    explicit AVLPlayerNode(const T &value) : data(value), left_son(nullptr), right_son(nullptr), parent(nullptr), height(0), counter_players_in_subtree(1), sum_levels_in_subtree(0) {
        int my_level = std::shared_ptr<Player>(value)->GetLevel();
        this->sum_levels_in_subtree += my_level;
    }

    ~AVLPlayerNode();

    const AVLPlayerNode<T> *GetMinNode() const;

    T GetValue() const { return data; }

    void SetValue(const T &new_data);

    void SetLeft(AVLPlayerNode<T> *left) { left_son = left; }

    AVLPlayerNode<T> *GetLeft() const { return left_son; }

    void SetRight(AVLPlayerNode<T> *right) { right_son = right; }

    AVLPlayerNode<T> *GetRight() const { return right_son; }

    void SetParent(AVLPlayerNode<T> *parent_) { parent = parent_; }

    AVLPlayerNode<T> *GetParent() const { return parent; }

    void SetHeight(int new_height) { this->height = new_height; }

    int GetHeight() const { return this->height; }

    void UpdateHeight();

    int GetBalanceFactor() const;

    AVLPlayerNode<T> *GetGreateParent();

    AVLPlayerNode<T> *GetGreateParentAux(AVLPlayerNode<T> *node);

    void ToArray(T *array, int *location);


    int GetCounterPlayerInSub();
    int GetSumLevelsInSub();

    int GetLevel();
};

template<class T>
class AVLPlayerTreee {

    AVLPlayerNode<T> *root;

    int (*compare)(const T &a, const T &b);
/*
 * compare return value:
 * 1 - a value is bigger than b
 * 0 - a value is equal than b
 * -1 - a value is smaller than b
 */
    void RotateLeft(AVLPlayerNode<T> *node);

    void RotateRight(AVLPlayerNode<T> *B);

    void InsertAux(AVLPlayerNode<T> *node, AVLPlayerNode<T> *new_node);


    void RotateByNeeded(AVLPlayerNode<T> *node);

public:

    explicit AVLPlayerTreee(int (*comp)(const T &a, const T &b)) : root(nullptr), compare(comp) {}

    ~AVLPlayerTreee();

    AVLPlayerNode<T> *RemoveBinary(AVLPlayerNode<T> *to_delete, const T& value);

    bool Insert(const T &data);

    bool Remove(const T &data);

    bool IsEmpty();

    const AVLPlayerNode<T> *GetMinNode() const;

    AVLPlayerNode<T> *Find(const T &value) const;

    AVLPlayerNode<T> *FindAux(AVLPlayerNode<T> *node, const T &value) const;

    AVLPlayerNode<T> *GetSmallestInTree();

    void merge(T *arr1, T *arr2, T *merge_to, int arr1_size, int arr2_size) const;

    AVLPlayerNode<T> *GetRoot();

    void SetRoot(AVLPlayerNode<T> *new_root);

    static AVLPlayerNode<T> *FromArrayToTree(T *array, int start, int end, int *to_fill);

    static AVLPlayerTreee<T> *FromArrayToTree(T *array, int start, int size, int (*comp)(const T &, const T &));

    AVLPlayerNode<T> *FindMinValInTree(AVLPlayerNode<T> *node);

    AVLPlayerNode<T> *FindMaxValInTree(AVLPlayerNode<T> *node);

    int GetTreeSize();

    int GetSizeAux(AVLPlayerNode<T> *node);

    void ToArray(T *array);

    int NumOfSmallerThan_Aux(int level, AVLPlayerNode<T> *node);
    int NumOfSmallerThan(int level);

    double FindHigherMRank(int m);
   double FindHigherMRank_Aux(AVLPlayerNode<T>* node, int* m);

    AVLPlayerTreee();
};

template<class T>
void AVLPlayerNode<T>::ToArray(T *array, int *location) {
    if (this->GetLeft() != nullptr)
        this->GetLeft()->ToArray(array, location);
    array[*location] = this->GetValue();
    *location = *location + 1;
    if (this->GetRight() != nullptr)
        this->GetRight()->ToArray(array, location);
}

template<class T> //Also updating the counter and sum fields
void AVLPlayerNode<T>::UpdateHeight() {

    int left_height = 0;
    int right_height = 0;

    //reset the fields
    this->counter_players_in_subtree = 1;
    this->sum_levels_in_subtree = std::shared_ptr<Player>(this->GetValue())->GetLevel();

    if (right_son == nullptr) {
        right_height = -1;
    } else {
        right_height = right_son->GetHeight();
        this->counter_players_in_subtree += right_son->counter_players_in_subtree;
        this->sum_levels_in_subtree += right_son->sum_levels_in_subtree;
    }

    if (left_son == nullptr) {
        left_height = -1;
    } else {
        left_height = left_son->GetHeight();
        this->counter_players_in_subtree += left_son->counter_players_in_subtree;
        this->sum_levels_in_subtree += left_son->sum_levels_in_subtree;
    }

    this->height = (std::max(left_height, right_height) + 1);
}


template<class T>
int AVLPlayerNode<T>::GetBalanceFactor() const {
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

template<class T>
AVLPlayerNode<T> *AVLPlayerNode<T>::GetGreateParent() {
    if (this->parent == nullptr)
        return this;
    return GetGreateParentAux(this->parent);
}

template<class T>
AVLPlayerNode<T> *AVLPlayerNode<T>::GetGreateParentAux(AVLPlayerNode<T> *node) {
    if (node->parent == nullptr)
        return node;
    return GetGreateParentAux(node->parent);
}

template<class T>
AVLPlayerNode<T>::~AVLPlayerNode() {
    delete left_son;
    delete right_son;
}

template<class T>
void AVLPlayerNode<T>::SetValue(const T &new_data) {
    this->data = new_data;
}

template<class T>
const AVLPlayerNode<T> *AVLPlayerNode<T>::GetMinNode() const {
    if (GetLeft() == nullptr)
        return this;
    else
        return GetLeft()->GetMinNode();
}

template<class T>
int AVLPlayerNode<T>::GetLevel() {
    return ((std::shared_ptr<Player>)(this->GetValue()))->GetLevel();
}

template<class T>
int AVLPlayerNode<T>::GetCounterPlayerInSub() {
    return this->counter_players_in_subtree;
}

template<class T>
int AVLPlayerNode<T>::GetSumLevelsInSub() {
    return this->sum_levels_in_subtree;
}


template<class T>
AVLPlayerNode<T> *AVLPlayerTreee<T>::Find(const T &value) const {
    AVLPlayerNode<T> *tmp = this->root;
    return FindAux(tmp, value);
}


// basic rotations
template<class T>
void AVLPlayerTreee<T>::RotateLeft(AVLPlayerNode<T> *B) {
    if (B == nullptr) { return; }

    AVLPlayerNode<T> *A = B->GetRight();


    AVLPlayerNode<T> *A_L = A->GetLeft();
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

template<class T>
void AVLPlayerTreee<T>::RotateRight(AVLPlayerNode<T> *B) {
    if (B == nullptr) { return; }

    AVLPlayerNode<T> *A = B->GetLeft();


    AVLPlayerNode<T> *A_R = A->GetRight();
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
template<class T>
bool AVLPlayerTreee<T>::Insert(const T &data) {

    //check if this value already exists

    if (this->Find(data)) {
        return false;
    }

    auto *new_node = new AVLPlayerNode<T>(data);


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


template<class T>
void AVLPlayerTreee<T>::InsertAux(AVLPlayerNode<T> *node, AVLPlayerNode<T> *new_node) {
    if (compare(node->GetValue(), new_node->GetValue()) == 1) {

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
template<class T>
AVLPlayerNode<T> *AVLPlayerTreee<T>::FindMinValInTree(AVLPlayerNode<T> *node) {
    if (node == nullptr)
        return node;

    if (node->GetLeft() == nullptr)
        return node;
    return FindMinValInTree(node->GetLeft());
}


template <class T>
AVLPlayerNode<T>* AVLPlayerTreee<T>::RemoveBinary(AVLPlayerNode<T>* node, const T& data)
{
    AVLPlayerNode<T>* to_delete = Find(data);
    //start with the greatest if shrink the next conditions
    AVLPlayerNode<T>* big_parent = nullptr;

    //if to_delete has two sons
    if (to_delete->GetLeft() && to_delete->GetRight()) {
        //here can be minVal
        AVLPlayerNode<T>* new_root = FindMinValInTree(to_delete->GetRight());

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
    AVLPlayerNode<T>* parent = to_delete->GetParent();
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

template<class T>
void AVLPlayerTreee<T>::RotateByNeeded(AVLPlayerNode<T> *node) {
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


template<class T>
bool AVLPlayerTreee<T>::Remove(const T &data) {
    if (root == nullptr)//if root is empty
        return false;


    AVLPlayerNode<T> *to_delete = this->Find(data);
    if (to_delete == nullptr)//if data needed to delete doesn't exist
    {
        return false;
    }

    //regular binary tree remove
    AVLPlayerNode<T> *parent_of_deleted = RemoveBinary(root, data);

    while (parent_of_deleted != nullptr) {
        RotateByNeeded(parent_of_deleted);
        parent_of_deleted = parent_of_deleted->GetParent();
    }
    return true;

}


template<class T>
int AVLPlayerTreee<T>::GetTreeSize() {
    return GetSizeAux(this->root);
}

template<class T>
int AVLPlayerTreee<T>::GetSizeAux(AVLPlayerNode<T> *node) {
    if (node == nullptr)
        return 0;

    return 1 + GetSizeAux(node->GetLeft()) + GetSizeAux(node->GetRight());
}

template<class T>
void AVLPlayerTreee<T>::ToArray(T *array) {
    int location = 0;
    if (root != nullptr)
        root->ToArray(array, &location);
}

template<class T>
AVLPlayerTreee<T> *AVLPlayerTreee<T>::FromArrayToTree(T *array, int start, int size, int (*comp)(const T &a, const T &b)) {
    auto tree = new AVLPlayerTreee<T>(comp);
    int to_fill = size;
    tree->root = FromArrayToTree(array, start, size, &to_fill);
    //tree->Validate();
    return tree;
}

template <class T>
AVLPlayerNode<T> *AVLPlayerTreee<T>::FromArrayToTree(T *array, int start, int end, int* to_fill) {

    if (*to_fill ==0 || start >end)
    {
        return nullptr;
    }

    int mini_root_index = (start+end)/2;

    auto* mini_root = new AVLPlayerNode<T>(array[mini_root_index]);
    mini_root->SetParent(nullptr);
    *to_fill = *to_fill -1;

    if (start-end == 0)
    {
        mini_root->SetLeft(nullptr);
        mini_root->SetRight(nullptr);
        mini_root->SetHeight(0);
        return mini_root;
    }


    AVLPlayerNode<T>* left_son = FromArrayToTree(array, start, mini_root_index - 1, to_fill);

    AVLPlayerNode<T>* right_son = FromArrayToTree(array, mini_root_index + 1, end, to_fill);



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


template<class T>
AVLPlayerNode<T> *AVLPlayerTreee<T>::FindAux(AVLPlayerNode<T> *node, const T &value) const {
    if (node == nullptr) {
        return nullptr;
    }
    if (compare(node->GetValue(), value) == 0) {
        return node;
    }
    if (compare(value, node->GetValue()) == 1) {
        return FindAux(node->GetRight(), value);
    } else {
        return FindAux(node->GetLeft(), value);
    }

}

template<class T>
bool AVLPlayerTreee<T>::IsEmpty() {
    return this->root == nullptr;
}

template<class T>
AVLPlayerNode<T> *AVLPlayerTreee<T>::GetSmallestInTree() {
    if (root == nullptr)
        return nullptr;

    AVLPlayerNode<T> *smallest = root;
    while (smallest->GetLeft() != nullptr) {
        smallest = smallest->GetLeft();
    }
    return smallest;
}

template<class T>
AVLPlayerNode<T> *AVLPlayerTreee<T>::GetRoot() {
    return this->root;
}

template<class T>
void AVLPlayerTreee<T>::SetRoot(AVLPlayerNode<T> *new_root) {
    this->root = new_root;
}

template<class T>
AVLPlayerNode<T> *AVLPlayerTreee<T>::FindMaxValInTree(AVLPlayerNode<T> *node) {
    if (node->GetRight() == nullptr && node->GetLeft() == nullptr)
        return node;
    return FindMaxValInTree(node->GetRight());
}


template<class T>
AVLPlayerTreee<T>::~AVLPlayerTreee() {
    delete root;
}


template<class T>
void AVLPlayerTreee<T>::merge(T *arr1, T *arr2, T *merge_to, int arr1_size, int arr2_size) const {

    while (arr1_size != 0 && arr2_size != 0) {
        if (compare(*arr1, *arr2) == -1) {
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

template<class T>
const AVLPlayerNode<T> *AVLPlayerTreee<T>::GetMinNode() const {
    if (root == nullptr)
        return nullptr;
    return root->GetMinNode();
}



template<class T>
int AVLPlayerTreee<T>::NumOfSmallerThan(int level) {
        return NumOfSmallerThan_Aux(level, this->root);
}

template<class T>
int AVLPlayerTreee<T>::NumOfSmallerThan_Aux(int level, AVLPlayerNode<T> *node) {

    if(node == nullptr)
        return 0;


    if(node->GetLevel() == level){
        if(node->GetLeft() != nullptr){
            if(node->GetLeft()->GetLevel() == level){
                return NumOfSmallerThan_Aux(level, node->GetLeft());
            }
            else{

                AVLPlayerNode<T>* tmp_node = node->GetLeft()->GetRight();
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


template<class T>
double AVLPlayerTreee<T>::FindHigherMRank(int m) {

    int * temp_m = new int (m);
    double temp =  FindHigherMRank_Aux(this->root,temp_m);
    delete temp_m;
    return  temp;
}

template<class T>
double AVLPlayerTreee<T>::FindHigherMRank_Aux(AVLPlayerNode<T> *node, int *m) {
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




#endif //DS2_AVLRANKTREEE_H
