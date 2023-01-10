
#ifndef DS2_RANKAVLPLAYERTREE_H
#define DS2_RANKAVLPLAYERTREE_H
#include <iostream>


template<class Key,class Value>
class TeamNode {

    Value team;
    TeamNode<Key,Value> *NodeLeft;
    TeamNode<Key,Value> *NodeRight;
    TeamNode<Key,Value> *UpNode;
    int height;
    int NumberOfTeamsUnderThisIncluded;

public:
    TeamNode() : team(nullptr), NodeLeft(nullptr), NodeRight(nullptr), UpNode(nullptr), height(0), NumberOfTeamsUnderThisIncluded(0){}
    explicit TeamNode(const Value &value) : team(value), NodeLeft(nullptr), NodeRight(nullptr), UpNode(nullptr), height(0), NumberOfTeamsUnderThisIncluded(1) {}
    ~TeamNode();
    const TeamNode<Key,Value> *GetMinNode() const;
    Value GetTeam() const { return team; }
    void SetTeam(const Value &new_data);
    void LeftNodeSet(TeamNode<Key,Value> *left) { NodeLeft = left; }
    TeamNode<Key,Value> *LeftNodeGet() const { return NodeLeft; }
    void UpNodeSet(TeamNode<Key,Value> *Up) { UpNode = Up; }
    TeamNode<Key,Value> *UpNodeGet() const { return UpNode; }
    int GetHeight() const { return this->height; }
    int NodeCounterSubTree() {return (this == nullptr) ? 0 : this->NumberOfTeamsUnderThisIncluded;};
    void RightNodeSet(TeamNode<Key,Value> *right) { NodeRight = right; }
    TeamNode<Key,Value> *RightNodeGet() const { return NodeRight; }
    void UpdateHeight();
    int GetBalanceFactor() const;
};

template<class Key,class Value>
void TeamNode<Key,Value>::UpdateHeight()
{
    int left = (NodeLeft == nullptr) ? -1 : NodeLeft->GetHeight();
    int right = (NodeRight == nullptr) ? -1 : NodeRight->GetHeight();
    this->NumberOfTeamsUnderThisIncluded = 1;
    if (NodeLeft != nullptr)
    {
        this->NumberOfTeamsUnderThisIncluded += NodeLeft->NumberOfTeamsUnderThisIncluded;
    }
    if (NodeRight != nullptr)
    {
        this->NumberOfTeamsUnderThisIncluded += NodeRight->NumberOfTeamsUnderThisIncluded;
    }
    this->height = left > right ? left + 1 : right + 1;
}


template<class Key,class Value>
int TeamNode<Key,Value>::GetBalanceFactor() const
{
    int lh = (NodeLeft == nullptr) ? -1 : NodeLeft->GetHeight();
    int rh = (NodeRight == nullptr) ? -1 : NodeRight->GetHeight();
    return lh - rh;
}

template<class Key,class Value>
TeamNode<Key,Value>::~TeamNode()
{
    delete NodeLeft;
    delete NodeRight;
}

template<class Key,class Value>
void TeamNode<Key,Value>::SetTeam(const Value &new_data)
{
    this->team = new_data;
}

template<class Key,class Value>
const TeamNode<Key,Value> *TeamNode<Key,Value>::GetMinNode() const
{
    if (this->LeftNodeGet() == nullptr)
        return this;

    TeamNode<Key,Value>* iterator = this;

    while (iterator->LeftNodeGet() != nullptr)
    {
        iterator=iterator->LeftNodeGet();
    }

    return iterator;
}

template<class Key,class Value>
class RankAVLTeamTree{

    TeamNode<Key,Value> *root;
    void LLRotation(TeamNode<Key,Value> *node);
    void RRRotation(TeamNode<Key,Value> *node);
    void RotationNeed(TeamNode<Key,Value> *node);

public:
    explicit RankAVLTeamTree() : root(nullptr) {}
    ~RankAVLTeamTree() { delete root;};
    TeamNode<Key,Value>* GetRoot() const {return this->root;};
    TeamNode<Key,Value>* Find(const Value &value) const;
    bool Insert(const Value &data);
    bool Remove(const Value &data);
    TeamNode<Key,Value>* findIndex(TeamNode<Key, Value> *node, int index);
    TeamNode<Key,Value> *FindMinValInTree(TeamNode<Key,Value> *node);

};

template<class Key,class Value>
void RankAVLTeamTree<Key,Value>::RotationNeed(TeamNode<Key,Value> *node)
{
    if (node->GetBalanceFactor() == 2)
    {
        if (node->LeftNodeGet()->GetBalanceFactor() == -1)
        {
            LLRotation(node->LeftNodeGet());
            RRRotation(node);
            return;
        }
        else if (node->LeftNodeGet()->GetBalanceFactor() >= 0)
        {
            RRRotation(node);
        }
    }

    else if (node->GetBalanceFactor() == -2)
    {
        if (node->RightNodeGet()->GetBalanceFactor()== 1)
        {
            RRRotation(node->RightNodeGet());
            LLRotation(node);
            return;
        }
        if (node->RightNodeGet()->GetBalanceFactor() <=0)
        {
            LLRotation(node);
            return;
        }
    }

    node->UpdateHeight();
}


template<class Key,class Value>
void RankAVLTeamTree<Key,Value>::LLRotation(TeamNode<Key,Value> *node)
{
    if (node == nullptr)
    {
        return;
    }

    TeamNode<Key,Value> *temp1 = node->RightNodeGet();
    TeamNode<Key,Value> *temp2 = temp1->LeftNodeGet();

    node->RightNodeSet(temp2);
    temp1->LeftNodeSet(node);

    if (node->UpNodeGet() == nullptr)
    {
        this->root= temp1;
    }

    else
    {
        (node->UpNodeGet()->LeftNodeGet() != node) ? (node->UpNodeGet()->RightNodeSet(temp1)) : (node->UpNodeGet()->LeftNodeSet(temp1));
    }

    temp1->UpNodeSet(node->UpNodeGet());
    node->UpNodeSet(temp1);

    if (temp2 != nullptr)
    {
        temp2->UpNodeSet(node);
    }

    node->UpdateHeight();
    temp1->UpdateHeight();
}

template<class Key,class Value>
void RankAVLTeamTree<Key,Value>::RRRotation(TeamNode<Key,Value> *node)
{
    if (node == nullptr)
    {
        return;
    }

    TeamNode<Key,Value> *temp1 = node->LeftNodeGet();
    TeamNode<Key,Value> *temp2 = temp1->RightNodeGet();
    node->LeftNodeSet(temp2);

    if (temp2 != nullptr)
    {
        temp2->UpNodeSet(node);
    }

    temp1->RightNodeSet(node);

    if (node->UpNodeGet() == nullptr)
    {
        this->root= temp1;
    }

    else
    {
        (node->UpNodeGet()->LeftNodeGet() != node) ? node->UpNodeGet()->RightNodeSet(temp1) :  node->UpNodeGet()->LeftNodeSet(temp1);
    }

    temp1->UpNodeSet(node->UpNodeGet());
    node->UpNodeSet(temp1);
    node->UpdateHeight();
    temp1->UpdateHeight();
}

template<class Key,class Value>
TeamNode<Key,Value> *RankAVLTeamTree<Key,Value>::Find(const Value &value) const
{
    if (this->root == nullptr)
        return nullptr;

    TeamNode<Key,Value>* iterator = GetRoot();

    while (iterator!= nullptr)
    {
        if (Key()(iterator->GetTeam(), value) == 0)
        {
            return iterator;
        }

        else if (Key()(value, iterator->GetTeam()) == -1)
        {
            iterator= iterator->LeftNodeGet();
        }

        else
        {
            iterator= iterator->RightNodeGet();
        }
    }

    return nullptr;

}

template<class Key,class Value>
bool RankAVLTeamTree<Key,Value>::Insert(const Value &data)
{
    try {
        auto *new_node = new TeamNode<Key, Value>(data);
        if (this->root == nullptr)
        {
            this->root = new_node;
            return true;
        }

        TeamNode<Key,Value>* iterator = GetRoot();

        while (iterator!= nullptr)
        {
            if (Key()(iterator->GetTeam(), new_node->GetTeam()) == 1)
            {
                if (iterator->LeftNodeGet() == nullptr)
                {
                    iterator->LeftNodeSet(new_node);
                    new_node->UpNodeSet(iterator);
                    break;
                }
                else
                {
                   iterator=iterator->LeftNodeGet();
                }
            }
            else
            {
                if (iterator->RightNodeGet() == nullptr)
                {
                    iterator->RightNodeSet(new_node);
                    new_node->UpNodeSet(iterator);
                    break;
                }
                else
                {
                   iterator=iterator->RightNodeGet();
                }
            }
        }

        if (iterator == nullptr)
        {
            return false;
        }

        TeamNode<Key,Value>* temp = iterator;

        while (temp->UpNodeGet() != nullptr)
        {
            temp->UpdateHeight();
            temp = temp->UpNodeGet();
        }

        while (iterator->UpNodeGet() != nullptr)
        {
            RotationNeed(iterator);
            iterator= iterator->UpNodeGet();
        }

    } catch (const std::bad_alloc &) { return false; }

}

template<class Key,class Value>
bool RankAVLTeamTree<Key,Value>::Remove(const Value &data)
{
    TeamNode<Key,Value>* removing_node = Find(data);
    TeamNode<Key,Value>* replacement = nullptr;

    if (removing_node == nullptr)
    {
        return false;
    }

    if (removing_node->RightNodeGet() != nullptr && removing_node->LeftNodeGet() != nullptr)
    {
        // have 2 children so need the replacement to be the min in thr rightsubtree
        TeamNode<Key,Value>* min = FindMinValInTree(removing_node->RightNodeGet());
        removing_node->SetTeam(min->GetTeam());
        removing_node = min;

        if (removing_node->RightNodeGet())
        {
            replacement = removing_node->RightNodeGet();
        }
    }

    // checking if has 1 child
    else if (removing_node->RightNodeGet() != nullptr)
    {
        replacement = removing_node->RightNodeGet();
    }

    else if (removing_node->LeftNodeGet() != nullptr)
    {
        replacement = removing_node->LeftNodeGet();
    }

    TeamNode<Key,Value>* parent_of_removing= removing_node->UpNodeGet();

    if (parent_of_removing == nullptr)
    {
        // if the removing is the root
        this->root= replacement;
    }

    else
    {
        (removing_node->UpNodeGet()->LeftNodeGet() == removing_node) ? (removing_node->UpNodeGet()->LeftNodeSet
        (replacement)) : (removing_node->UpNodeGet()->RightNodeSet(replacement));
    }

    if (replacement!= nullptr)
    {
        replacement->UpNodeSet(parent_of_removing);
    }

    TeamNode<Key,Value>* temp = parent_of_removing;

    // fixing the tree

    while (temp!= nullptr && temp->UpNodeGet() != nullptr)
    {
        temp->UpdateHeight();
        temp = temp->UpNodeGet();
    }

    temp = parent_of_removing;

    while (temp!= nullptr && temp->UpNodeGet() != nullptr)
    {
        RotationNeed(temp);
        temp= temp->UpNodeGet();
    }

    removing_node->LeftNodeSet(nullptr);
    removing_node->RightNodeSet(nullptr);
    delete removing_node;
    return true;

}

template<class Key, class Value>
TeamNode<Key, Value> *RankAVLTeamTree<Key, Value>::findIndex(TeamNode<Key, Value> *node, int index)
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

template<class Key, class Value>
TeamNode<Key, Value> *RankAVLTeamTree<Key, Value>::FindMinValInTree(TeamNode<Key, Value> *node)
{
    if (node == nullptr)
        return nullptr;

    TeamNode<Key,Value>* iterator = node;

    while (iterator->LeftNodeGet() != nullptr)
    {
        iterator=iterator->LeftNodeGet();
    }

    return iterator;
}

#endif //DS2_RANKAVLPLAYERTREE_H
