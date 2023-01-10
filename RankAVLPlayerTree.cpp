
#ifndef DS2_RANKAVLPLAYERTREE_H
#define DS2_RANKAVLPLAYERTREE_H
#include <iostream>

template<class Key,class Value>
class PlayerNode {

    Value player;
    PlayerNode<Key,Value> *NodeLeft;
    PlayerNode<Key,Value> *NodeRight;
    //PlayerNode<Key,Value> *UpNode;
    int height;
    int NumberOfPlayerInSubTreeIncluded;

public:

    PlayerNode() : player(nullptr), NodeLeft(nullptr), NodeRight(nullptr), //UpNode(nullptr), height(0),
    NumberOfPlayerInSubTreeIncluded(0){}
    explicit PlayerNode(const Value &value) : player(value), NodeLeft(nullptr), NodeRight(nullptr), //UpNode(nullptr),
    height(0), NumberOfPlayerInSubTreeIncluded(1) {}
    ~PlayerNode();
    const PlayerNode<Key,Value> *GetMinNode() const;
    Value DataGet() const { return player; }
    void DataSet(const Value &new_data);
    void LeftNodeSet(PlayerNode<Key,Value> *left) { NodeLeft = left; }
    PlayerNode<Key,Value> *LeftNodeGet() const { return NodeLeft; }
    //void UpNodeSet(PlayerNode<Key,Value> *Up) { UpNode = Up; }
  //  PlayerNode<Key,Value> *UpNodeGet() const { return UpNode; }
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
    void HelpForInsert(PlayerNode<Key,Value> *root_to_insert, PlayerNode<Key,Value> *new_node);
    void RotationNeed(PlayerNode<Key,Value> *node);

public:

    explicit RankAVLPlayerTree() : root(nullptr) {}
    ~RankAVLPlayerTree() { delete root;};
    PlayerNode<Key,Value> *HelpForRemove(PlayerNode<Key,Value> *root_to_remove, const Value& val);
    bool Insert(const Value &data);
    bool Remove(const Value &data);
    PlayerNode<Key,Value>* GetRoot() {return this->root;};
    PlayerNode<Key,Value>* findIndex(PlayerNode<Key, Value> *node, int index);
    PlayerNode<Key,Value> *Find(const Value &value) const;
    PlayerNode<Key,Value> *HelperForFind(PlayerNode<Key,Value> *node, const Value &value) const;
    PlayerNode<Key,Value> *FindMinValInTree(PlayerNode<Key,Value> *node);

};


template<class Key,class Value>
void PlayerNode<Key,Value>::UpdateHeight()
{
    int left = (NodeLeft == nullptr) ? -1 : NodeLeft->GetHeight();
    int right = (NodeRight == nullptr) ? -1 : NodeRight->GetHeight();
    this->NumberOfPlayerInSubTreeIncluded = 1;
    if (NodeLeft != nullptr)
    {
        this->NumberOfPlayerInSubTreeIncluded += NodeLeft->NumberOfPlayerInSubTreeIncluded;
    }
    if (NodeRight != nullptr)
    {
        this->NumberOfPlayerInSubTreeIncluded += NodeRight->NumberOfPlayerInSubTreeIncluded;
    }
    this->height = left > right ? left + 1 : right + 1;
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

    if (node == nullptr)
    {
        return;
    }

    PlayerNode<Key,Value> *helper = node->RightNodeGet();
    PlayerNode<Key,Value> *helpers_left_node = helper->LeftNodeGet();
    node->RightNodeSet(helpers_left_node);
    helper->LeftNodeSet(node);

//    if (node->UpNodeGet() == nullptr)
//    {
//        this->root = helper;
//    }
//    else
//    {
//        if (node->UpNodeGet()->LeftNodeGet() == node) {
//            node->UpNodeGet()->LeftNodeSet(helper);
//        }
//        else
//        {
//            node->UpNodeGet()->RightNodeSet(helper);
//        }
//    }
//    helper->UpNodeSet(node->UpNodeGet());
//    node->UpNodeSet(helper);
//
//    if (helpers_left_node != nullptr)
//    {
//        helpers_left_node->UpNodeSet(node);
//    }
    node->UpdateHeight();
    helper->UpdateHeight();
}


template<class Key,class Value>
void RankAVLPlayerTree<Key,Value>::RRRotation(PlayerNode<Key,Value> *node) {
    if (node == nullptr)
    {
        return;
    }

    PlayerNode<Key,Value> *helper = node->LeftNodeGet();
    PlayerNode<Key,Value> *helpers_right_node = helper->RightNodeGet();
    node->LeftNodeSet(helpers_right_node);

//    if (helpers_right_node != nullptr)
//    {
//        helpers_right_node->UpNodeSet(node);
//    }
//    helper->RightNodeSet(node);
//
//    if (node->UpNodeGet() == nullptr)
//    {
//        this->root = helper;
//    }
//    else
//    {
//        if (node->UpNodeGet()->LeftNodeGet() == node) {
//            node->UpNodeGet()->LeftNodeSet(helper);
//        }
//        else
//        {
//            node->UpNodeGet()->RightNodeSet(helper);
//        }
//    }
//    helper->UpNodeSet(node->UpNodeGet());
//    node->UpNodeSet(helper);

    node->UpdateHeight();
    helper->UpdateHeight();
}


template<class Key,class Value>
bool RankAVLPlayerTree<Key,Value>::Insert(const Value &data) {

    if (this->Find(data))
    {
        return false;
    }

    auto *new_node = new PlayerNode<Key,Value>(data);

    if (new_node == nullptr)
    {
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
void RankAVLPlayerTree<Key,Value>::HelpForInsert(PlayerNode<Key,Value> *root_to_insert, PlayerNode<Key,Value> *new_node) {
    if (Key()(root_to_insert->DataGet(), new_node->DataGet()) == 1) {

        if (root_to_insert->LeftNodeGet() == nullptr) {
            root_to_insert->LeftNodeSet(new_node);
        //    new_node->UpNodeSet(root_to_insert);
        }
        else
        {
            HelpForInsert(root_to_insert->LeftNodeGet(), new_node);
        }
    }
    else
    {
        if (root_to_insert->RightNodeGet() == nullptr)
        {
            root_to_insert->RightNodeSet(new_node);
         //   new_node->UpNodeSet(root_to_insert);
        }
        else
        {
            HelpForInsert(root_to_insert->RightNodeGet(), new_node);
        }
    }
    RotationNeed(root_to_insert);

}


template<class Key,class Value>
PlayerNode<Key,Value> *RankAVLPlayerTree<Key,Value>::FindMinValInTree(PlayerNode<Key,Value> *node) {
    return (node == nullptr or (node != nullptr and node->LeftNodeGet() == nullptr)) ? node : FindMinValInTree(node->LeftNodeGet());
}


template <class Key,class Value>
PlayerNode<Key,Value>* RankAVLPlayerTree<Key,Value>::HelpForRemove(PlayerNode<Key,Value>* root_to_remove, const Value& val)
{
    if (root_to_remove == nullptr)
        return root_to_remove;

    if ((Key()(val, root_to_remove->DataGet())) == (-1))
    {
        //node->addNumOfNodesBelow(-1);
        root_to_remove->LeftNodeSet((HelpForRemove(root_to_remove->LeftNodeGet(), val)))  ;
    }

    else if(Key()(val, root_to_remove->DataGet()) == 1)
    {
        //node->addNumOfNodesBelow(-1);
        root_to_remove->RightNodeSet((HelpForRemove(root_to_remove->RightNodeGet(), val))) ;
    }

    else
    {
        if((root_to_remove->LeftNodeGet() == nullptr) || (root_to_remove->RightNodeGet() == nullptr) )
        {
            PlayerNode<Key, Value>* temp;

            if ((root_to_remove->LeftNodeGet() == nullptr) && (root_to_remove->RightNodeGet() == nullptr))
            {
                temp = root_to_remove;
                root_to_remove = nullptr;
            }
            else
            {
                if ((root_to_remove->LeftNodeGet()) != nullptr)
                {
                    temp= root_to_remove->LeftNodeGet();
                }

                else
                {
                    temp= root_to_remove->RightNodeGet();
                }

                root_to_remove->DataSet((temp->DataGet())) ;

            }

            delete temp;
        }

        else
        {
            PlayerNode<Key, Value>* temp = FindMinValInTree(root_to_remove->RightNodeGet());
            root_to_remove->DataSet(temp->DataGet());
            root_to_remove->RightNodeSet(HelpForRemove(root_to_remove->RightNodeGet(), temp->DataGet()));
        }
    }

    if (root_to_remove == nullptr)
        return root_to_remove;

    root_to_remove->UpdateHeight();
    RotationNeed(root_to_remove);
    return root_to_remove;
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

    PlayerNode<Key, Value>* to_delete = this->Find(data);

    if (to_delete == nullptr)
    {
        return false;
    }

   HelpForRemove(this->root, data);
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