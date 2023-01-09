
#ifndef DS2_RANKAVLPLAYERTREE_H
#define DS2_RANKAVLPLAYERTREE_H


#include <iostream>


template<class Key,class Value>
class PlayerNode {

    Value player;
    PlayerNode<Key,Value> *NodeLeft;
    PlayerNode<Key,Value> *NodeRight;
    PlayerNode<Key,Value> *UpNode;
    int height;
    int NumberOfPlayerInSubTreeIncluded;

public:

    PlayerNode() : player(nullptr), NodeLeft(nullptr), NodeRight(nullptr), UpNode(nullptr), height(0), NumberOfPlayerInSubTreeIncluded(0){}

    explicit PlayerNode(const Value &value) : player(value), NodeLeft(nullptr), NodeRight(nullptr), UpNode(nullptr), height(0), NumberOfPlayerInSubTreeIncluded(1) {}

    ~PlayerNode();

    const PlayerNode<Key,Value> *GetMinNode() const;

    Value DataGet() const { return player; }

    void DataSet(const Value &new_data);

    void LeftNodeSet(PlayerNode<Key,Value> *left) { NodeLeft = left; }

    PlayerNode<Key,Value> *LeftNodeGet() const { return NodeLeft; }

    void UpNodeSet(PlayerNode<Key,Value> *Up) { UpNode = Up; }

    PlayerNode<Key,Value> *UpNodeGet() const { return UpNode; }

    int GetHeight() const { return this->height; }

    int NodeCounterSubTree() {return (this == nullptr) ? 0 : this->NumberOfPlayerInSubTreeIncluded;};

    void RightNodeSet(PlayerNode<Key,Value> *right) { NodeRight = right; }

    PlayerNode<Key,Value> *RightNodeGet() const { return NodeRight; }

    void UpdateHeight();

    int GetBalanceFactor() const;

};

template<class Key,class Value>
class RankAVLPlayerTree {

    PlayerNode<Key,Value> *root;

    void RLRotation(PlayerNode<Key,Value> *node);

    void RRRotation(PlayerNode<Key,Value> *node);

    void HelpForInsert(PlayerNode<Key,Value> *node, PlayerNode<Key,Value> *new_node);

    void RotationNeed(PlayerNode<Key,Value> *node);

public:

    explicit RankAVLPlayerTree() : root(nullptr) {}

    ~RankAVLPlayerTree() { delete root;};

    PlayerNode<Key,Value> *HelpForRemove(PlayerNode<Key,Value> *node, const Value& data);

    bool Insert(const Value &data);

    bool Remove(const Value &data);

    PlayerNode<Key,Value>* GetRoot() {return this->root;};

    PlayerNode<Key,Value>* findIndex(PlayerNode<Key, Value> *node, int index);

    PlayerNode<Key,Value> *Find(const Value &value) const;

    PlayerNode<Key,Value> *HelperForFind(PlayerNode<Key,Value> *node, const Value &value) const;

    PlayerNode<Key,Value> *FindMinValInTree(PlayerNode<Key,Value> *node);

    PlayerNode<Key,Value> *FindMaxValInTree(PlayerNode<Key,Value> *node);


};


template<class Key,class Value>
void PlayerNode<Key,Value>::UpdateHeight() {

    int left_height = 0;
    int right_height = 0;

    this->NumberOfPlayerInSubTreeIncluded = 1;

    if (NodeLeft == nullptr) {
        left_height = -1;
    }
    else
    {
        left_height = NodeLeft->GetHeight();
        this->NumberOfPlayerInSubTreeIncluded += NodeLeft->NumberOfPlayerInSubTreeIncluded;
    }

    if (NodeRight == nullptr)
    {
        right_height = -1;
    }
    else
    {
        right_height = NodeRight->GetHeight();
        this->NumberOfPlayerInSubTreeIncluded += NodeRight->NumberOfPlayerInSubTreeIncluded;

    }

    this->height = (std::max(left_height, right_height) + 1);
}


template<class Key,class Value>
int PlayerNode<Key,Value>::GetBalanceFactor() const {
    int lh = (NodeLeft == nullptr) ? -1 : NodeLeft->GetHeight();
    int rh = (NodeRight == nullptr) ? -1 : NodeRight->GetHeight();
    return lh - rh;
}

template<class Key,class Value>
PlayerNode<Key,Value>::~PlayerNode() {
    delete NodeLeft;
    delete NodeRight;
}

template<class Key,class Value>
void PlayerNode<Key,Value>::DataSet(const Value &new_data) {
    this->player = new_data;
}

template<class Key,class Value>
const PlayerNode<Key,Value> *PlayerNode<Key,Value>::GetMinNode() const {
    return (LeftNodeGet() == nullptr) ? this : LeftNodeGet()->GetMinNode();
}


template<class Key,class Value>
PlayerNode<Key,Value> *RankAVLPlayerTree<Key,Value>::Find(const Value &value) const {
    PlayerNode<Key,Value> *tmp = this->root;
    return HelperForFind(tmp, value);
}



template<class Key,class Value>
void RankAVLPlayerTree<Key,Value>::RLRotation(PlayerNode<Key,Value> *node) {
    if (node == nullptr) { return; }

    PlayerNode<Key,Value> *helper = node->RightNodeGet();


    PlayerNode<Key,Value> *helpers_left_node = helper->LeftNodeGet();
    node->RightNodeSet(helpers_left_node);
    helper->LeftNodeSet(node);

    if (node->UpNodeGet() == nullptr) {
        this->root = helper;
    }
    else
    {
        if (node->UpNodeGet()->LeftNodeGet() == node) {
            node->UpNodeGet()->LeftNodeSet(helper);
        }
        else
        {
            node->UpNodeGet()->RightNodeSet(helper);
        }
    }
    helper->UpNodeSet(node->UpNodeGet());
    node->UpNodeSet(helper);

    if (helpers_left_node != nullptr)
    {
        helpers_left_node->UpNodeSet(node);
    }
    node->UpdateHeight();
    helper->UpdateHeight();
}

template<class Key,class Value>
void RankAVLPlayerTree<Key,Value>::RRRotation(PlayerNode<Key,Value> *node) {
    if (node == nullptr) { return; }

    PlayerNode<Key,Value> *helper = node->LeftNodeGet();


    PlayerNode<Key,Value> *helpers_right_node = helper->RightNodeGet();
    node->LeftNodeSet(helpers_right_node);
    if (helpers_right_node != nullptr)
    {
        helpers_right_node->UpNodeSet(node);
    }
    helper->RightNodeSet(node);

    if (node->UpNodeGet() == nullptr) {
        this->root = helper;
    }
    else
    {
        if (node->UpNodeGet()->LeftNodeGet() == node) {
            node->UpNodeGet()->LeftNodeSet(helper);
        }
        else
        {
            node->UpNodeGet()->RightNodeSet(helper);
        }
    }
    helper->UpNodeSet(node->UpNodeGet());
    node->UpNodeSet(helper);

    node->UpdateHeight();
    helper->UpdateHeight();
}


template<class Key,class Value>
bool RankAVLPlayerTree<Key,Value>::Insert(const Value &data) {

    if (this->Find(data)) {
        return false;
    }

    auto *new_node = new PlayerNode<Key,Value>(data);

    if (new_node == nullptr) {
        throw std::bad_alloc();
    }

    if (this->root == nullptr)
    {
        this->root = new_node;
    }
    else
    {
        HelpForInsert(this->root, new_node);
    }

    return true;
}



template<class Key,class Value>
void RankAVLPlayerTree<Key,Value>::HelpForInsert(PlayerNode<Key,Value> *node, PlayerNode<Key,Value> *new_node) {
    if (Key()(node->DataGet(), new_node->DataGet()) == 1) {

        if (node->LeftNodeGet() == nullptr) {
            node->LeftNodeSet(new_node);
            new_node->UpNodeSet(node);
        }
        else
        {
            HelpForInsert(node->LeftNodeGet(), new_node);
        }
    }
    else
    {
        if (node->RightNodeGet() == nullptr)
        {
            node->RightNodeSet(new_node);
            new_node->UpNodeSet(node);
        }
        else
        {
            HelpForInsert(node->RightNodeGet(), new_node);
        }
    }
    RotationNeed(node);

}


template<class Key,class Value>
PlayerNode<Key,Value> *RankAVLPlayerTree<Key,Value>::FindMinValInTree(PlayerNode<Key,Value> *node) {
   return (node == nullptr or (node != nullptr and node->LeftNodeGet() == nullptr)) ? node : FindMinValInTree(node->LeftNodeGet());
}


template <class Key,class Value>
PlayerNode<Key,Value>* RankAVLPlayerTree<Key,Value>::HelpForRemove(PlayerNode<Key,Value>* node, const Value& data)
{
    PlayerNode<Key,Value>* removing = Find(data);
    PlayerNode<Key,Value>* helper_removing_parent_node = nullptr;

    if (removing->LeftNodeGet() && removing->RightNodeGet()) {
        PlayerNode<Key,Value>* new_root_needed = FindMinValInTree(removing->RightNodeGet());

        removing->DataSet(new_root_needed->DataGet());
        removing = new_root_needed;
        if (removing->RightNodeGet()) {
            helper_removing_parent_node = removing->RightNodeGet();
        }
    }

    else if (removing->LeftNodeGet())
    {
        helper_removing_parent_node = removing->LeftNodeGet();
    }
    else if (removing->RightNodeGet())
    {
        helper_removing_parent_node = removing->RightNodeGet();
    }

    PlayerNode<Key,Value>* removings_up_node = removing->UpNodeGet();
    if (removings_up_node == nullptr)
    {
        root = helper_removing_parent_node;
    }
    else if (removing->UpNodeGet()->LeftNodeGet() == removing)
    {
        removing->UpNodeGet()->LeftNodeSet(helper_removing_parent_node);
    }
    else
    {
        removing->UpNodeGet()->RightNodeSet(helper_removing_parent_node);
    }
    if (helper_removing_parent_node != nullptr) {
        helper_removing_parent_node->UpNodeSet(removings_up_node);
    }

    removing->LeftNodeSet(nullptr);
    removing->RightNodeSet(nullptr);
    delete removing;
    return removings_up_node;
}

template<class Key,class Value>
void RankAVLPlayerTree<Key,Value>::RotationNeed(PlayerNode<Key,Value> *node) {
    int balance_root = node->GetBalanceFactor();

    if (balance_root == 2 && (node->LeftNodeGet())->GetBalanceFactor() >= 0) {
        RRRotation(node);
    }
    else if (balance_root == 2 && (node->LeftNodeGet())->GetBalanceFactor() == -1)
    {
        RLRotation(node->LeftNodeGet());
        RRRotation(node);
        return;
    }
    else if (balance_root == -2 && (node->RightNodeGet())->GetBalanceFactor() == 1)
    {
        RRRotation(node->RightNodeGet());
        RLRotation(node);
        return;
    }
    else if (balance_root == -2 && (node->RightNodeGet())->GetBalanceFactor() <= 0)
    {
        RLRotation(node);
        return;
    }

    node->UpdateHeight();
}


template<class Key,class Value>
bool RankAVLPlayerTree<Key,Value>::Remove(const Value &data) {
    if (root == nullptr)
        return false;

    PlayerNode<Key,Value> *we_remove = this->Find(data);
    if (we_remove == nullptr)
    {
        return false;
    }

    PlayerNode<Key,Value> *parent_of_deleted = HelpForRemove(root, data);

    while (parent_of_deleted != nullptr) {
        RotationNeed(parent_of_deleted);
        parent_of_deleted = parent_of_deleted->UpNodeGet();
    }
    return true;

}



template<class Key,class Value>
PlayerNode<Key,Value> *RankAVLPlayerTree<Key,Value>::HelperForFind(PlayerNode<Key,Value> *node, const Value &value) const {
    if (node == nullptr) {
        return nullptr;
    }

    if (Key()(node->DataGet(), value) == 0)
    {
        return node;
    }
    else if (Key()(value, node->DataGet()) == 1)
    {
        return HelperForFind(node->RightNodeGet(), value);
    }
    else
    {

        return HelperForFind(node->LeftNodeGet(), value);
    }

}



template<class Key,class Value>
PlayerNode<Key,Value> *RankAVLPlayerTree<Key,Value>::FindMaxValInTree(PlayerNode<Key,Value> *node) {
    return (node->RightNodeGet() == nullptr && node->LeftNodeGet() == nullptr) ? node : FindMaxValInTree(node->RightNodeGet());
}



template<class Key, class Value>
PlayerNode<Key, Value> *RankAVLPlayerTree<Key, Value>::findIndex(PlayerNode<Key, Value> *node, int index)
{
    if (node == nullptr)
        return 0;

    if (node->LeftNodeGet()->NodeCounterSubTree() == index - 1)
    {
        return node;
    }

    else if (node->LeftNodeGet()->NodeCounterSubTree() > index - 1)
    {
        return findIndex(node->LeftNodeGet(), index);
    }

    else
    {
        return findIndex(node->RightNodeGet(), (index - node->LeftNodeGet()->NodeCounterSubTree() - 1));
    }

}



#endif //DS2_RANKAVLPLAYERTREE_H
