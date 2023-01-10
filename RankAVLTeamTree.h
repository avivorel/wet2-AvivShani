
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
class RankAVLTeamTree {

    TeamNode<Key,Value> *root;

    void RLRotation(TeamNode<Key,Value> *node);

    void RRRotation(TeamNode<Key,Value> *node);

    void HelpForInsert(TeamNode<Key,Value> *root_to_insert, TeamNode<Key,Value> *new_node);

    void RotationNeed(TeamNode<Key,Value> *node);

public:

    explicit RankAVLTeamTree() : root(nullptr) {}

    ~RankAVLTeamTree() { delete root;};

    TeamNode<Key,Value> *HelpForRemove(TeamNode<Key,Value> *root_to_remove, const Value& val);

    bool Insert(const Value &data);

    bool Remove(const Value &data);

    TeamNode<Key,Value>* GetRoot() {return this->root;};

    TeamNode<Key,Value>* findIndex(TeamNode<Key, Value> *node, int index);

    TeamNode<Key,Value> *Find(const Value &value) const;

    TeamNode<Key,Value> *HelperForFind(TeamNode<Key,Value> *node, const Value &value) const;

    TeamNode<Key,Value> *FindMinValInTree(TeamNode<Key,Value> *node);



};


template<class Key,class Value>
void TeamNode<Key,Value>::UpdateHeight() {
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
int TeamNode<Key,Value>::GetBalanceFactor() const {
    int lh = (NodeLeft == nullptr) ? -1 : NodeLeft->GetHeight();
    int rh = (NodeRight == nullptr) ? -1 : NodeRight->GetHeight();
    return lh - rh;
}

template<class Key,class Value>
TeamNode<Key,Value>::~TeamNode() {
    delete NodeLeft;
    delete NodeRight;
}

template<class Key,class Value>
void TeamNode<Key,Value>::SetTeam(const Value &new_data) {
    this->team = new_data;
}

template<class Key,class Value>
const TeamNode<Key,Value> *TeamNode<Key,Value>::GetMinNode() const {
    return (LeftNodeGet() == nullptr) ? this : LeftNodeGet()->GetMinNode();
}


template<class Key,class Value>
TeamNode<Key,Value> *RankAVLTeamTree<Key,Value>::Find(const Value &value) const {
    TeamNode<Key,Value> *tmp = this->root;
    return HelperForFind(tmp, value);
}



template<class Key,class Value>
void RankAVLTeamTree<Key,Value>::RLRotation(TeamNode<Key,Value> *node) {
    if (node == nullptr) { return; }

    TeamNode<Key,Value> *helper = node->RightNodeGet();


    TeamNode<Key,Value> *helpers_left_node = helper->LeftNodeGet();
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
void RankAVLTeamTree<Key,Value>::RRRotation(TeamNode<Key,Value> *node) {
    if (node == nullptr) { return; }

    TeamNode<Key,Value> *helper = node->LeftNodeGet();


    TeamNode<Key,Value> *helpers_right_node = helper->RightNodeGet();
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
bool RankAVLTeamTree<Key,Value>::Insert(const Value &data) {

    if (this->Find(data)) {
        return false;
    }

    auto *new_node = new TeamNode<Key,Value>(data);

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
void RankAVLTeamTree<Key,Value>::HelpForInsert(TeamNode<Key,Value> *root_to_insert, TeamNode<Key,Value> *new_node) {
    if (Key()(root_to_insert->GetTeam(), new_node->GetTeam()) == 1) {

        if (root_to_insert->LeftNodeGet() == nullptr) {
            root_to_insert->LeftNodeSet(new_node);
            new_node->UpNodeSet(root_to_insert);
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
            new_node->UpNodeSet(root_to_insert);
        }
        else
        {
            HelpForInsert(root_to_insert->RightNodeGet(), new_node);
        }
    }
    RotationNeed(root_to_insert);

}


template<class Key,class Value>
TeamNode<Key,Value> *RankAVLTeamTree<Key,Value>::FindMinValInTree(TeamNode<Key,Value> *node) {
   return (node == nullptr or (node != nullptr and node->LeftNodeGet() == nullptr)) ? node : FindMinValInTree(node->LeftNodeGet());
}


template <class Key,class Value>
TeamNode<Key,Value>* RankAVLTeamTree<Key,Value>::HelpForRemove(TeamNode<Key,Value>* root_to_remove, const Value& val)
{
    TeamNode<Key,Value>* removing = Find(val);
    TeamNode<Key,Value>* helper_removing_parent_node = nullptr;

    if (removing->LeftNodeGet() && removing->RightNodeGet()) {
        TeamNode<Key,Value>* new_root_needed = FindMinValInTree(removing->RightNodeGet());

        removing->SetTeam(new_root_needed->GetTeam());
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

    TeamNode<Key,Value>* removings_up_node = removing->UpNodeGet();
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
void RankAVLTeamTree<Key,Value>::RotationNeed(TeamNode<Key,Value> *node) {
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
bool RankAVLTeamTree<Key,Value>::Remove(const Value &data) {
    if (root == nullptr)
        return false;

    TeamNode<Key,Value> *we_remove = this->Find(data);
    if (we_remove == nullptr)
    {
        return false;
    }

    TeamNode<Key,Value> *UpDeleted = HelpForRemove(root, data);

    while (UpDeleted != nullptr) {
        RotationNeed(UpDeleted);
        UpDeleted = UpDeleted->UpNodeGet();
    }
    return true;

}



template<class Key,class Value>
TeamNode<Key,Value> *RankAVLTeamTree<Key,Value>::HelperForFind(TeamNode<Key,Value> *node, const Value &value) const {
    if (node == nullptr) {
        return nullptr;
    }

    if (Key()(node->GetTeam(), value) == 0)
    {
        return node;
    }
    else if (Key()(value, node->GetTeam()) == 1)
    {
        return HelperForFind(node->RightNodeGet(), value);
    }
    else
    {

        return HelperForFind(node->LeftNodeGet(), value);
    }

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



#endif //DS2_RANKAVLPLAYERTREE_H
