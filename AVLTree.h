
template<class key, class value>
class AVLNode {

public:
    value data;
    AVLNode<key, value> *left_son;
    AVLNode<key, value> *right_son;
    AVLNode<key, value> *parent;

    int height;
    int numOfNodesBelow_include;

    AVLNode() : data(nullptr), left_son(nullptr), right_son(nullptr),parent(nullptr), height(0), numOfNodesBelow_include
    (0) {}

    explicit AVLNode(const value &data) : data(data), left_son(nullptr), right_son(nullptr), height(0)
            ,numOfNodesBelow_include(1) {}

    ~AVLNode();

    int nodesBelow() { return (this == nullptr) ? 0 : this->numOfNodesBelow_include; }

    int updateNodeHeight() ;

    value DataGet() { return  this->data; }

};

template<class key, class value>
AVLNode<key, value>::~AVLNode() {
    delete this->left_son;
    delete this->right_son;
}

template<class key, class value>
class AVLTree {

    AVLNode<key, value> *root;

    void rotateLeft(AVLNode<key, value> *node);

    void rotateRight(AVLNode<key, value> *node);

    void insertHelper(AVLNode<key, value> *node, AVLNode<key, value> *insert);

    void rotateByNeeded(AVLNode<key, value> *node);

public:

    AVLTree() : root(nullptr) {}

    ~AVLTree();

    AVLNode<key, value> *Find(const value &data) const;

    AVLNode<key, value> *findHelper(AVLNode<key, value> *node, const value &data) const;

    bool Insert(const value &data);

    bool Remove(const value &data);

    AVLNode<key, value>* removeHelper(AVLNode<key, value>* node, const value& data);

    int getNodeBF( AVLNode<key, value> *node) const;

    AVLNode<key, value> *GetRoot();

    AVLNode<key, value>* findMin(AVLNode<key, value>* root) const;

    AVLNode<key, value>* findIndex(AVLNode<key, value>* node, int index) const;


};

template<class key, class value>
AVLNode<key, value>* AVLTree<key,value>:: GetRoot()
{
    return this->root;
}

template<class key, class value>
int AVLNode<key,value>:: updateNodeHeight()
{
    int left = (left_son == nullptr) ? -1 : left_son->height;
    int right = (right_son == nullptr) ? -1 : right_son->height;
    this->numOfNodesBelow_include = 1;
    if (left_son != nullptr)
    {
        this->numOfNodesBelow_include += left_son->nodesBelow();
    }
    if (right_son != nullptr)
    {
        this->numOfNodesBelow_include += right_son->nodesBelow();
    }
    this->height = left > right ? left + 1 : right + 1;
}

template<class key, class value>
int AVLTree<key,value>:: getNodeBF ( AVLNode<key, value> *node) const
{
    int lh = (node->left_son == nullptr) ? -1 : node->left_son->height;
    int rh = (node->right_son == nullptr) ? -1 : node->right_son->height;
    return lh - rh;

}

template<class key, class value>
void AVLTree<key,value>:: rotateLeft (AVLNode<key, value>* node)
{
    if (node == nullptr)
    {
        return;
    }

    AVLNode<key, value>* temp1 = node->right_son;
    AVLNode<key, value>* temp2= temp1->left_son;

    temp1->left_son= node;
    node->right_son=temp2;

    node->updateNodeHeight();
    temp1->updateNodeHeight();
}

template<class key, class value>
void AVLTree<key,value>:: rotateRight (AVLNode<key, value>* node)
{
    AVLNode<key, value>* temp1 = node->left_son;
    AVLNode<key, value>* temp2= temp1->right_son;

    temp1->right_son= node;
    node->left_son= temp2;

    node->updateNodeHeight();
    temp1->updateNodeHeight();

}


template<class key, class value>
void AVLTree<key,value>::rotateByNeeded(AVLNode<key, value> *node)
{

    if (getNodeBF(node) == -2 && getNodeBF(node->right_son) <= 0)
    {
        rotateLeft(node);
        return;
    }

    if (getNodeBF(node)  == -2 && getNodeBF(node->right_son) == 1)
    {
        rotateRight(node->right_son);
        rotateLeft(node);
        return;
    }

    if (getNodeBF(node)  == 2 && getNodeBF(node->left_son) == -1)
    {
        rotateLeft(node->left_son);
        rotateRight(node);
        return;
    }

    if (getNodeBF(node) == 2 && getNodeBF(node->left_son) >= 0)
    {
        rotateRight(node);
        return;
    }

    node->updateNodeHeight();
}

template<class key, class value>
bool AVLTree<key,value>::Insert(const value &data)
{
    if (this->Find(data))
    {
        return false;
    }

    auto *new_node = new AVLNode<key,value>(data);

    if (new_node == nullptr) {
        throw std::bad_alloc();
    }

    if (this->root == nullptr)
    {
        this->root = new_node;
    }
    else
    {
        insertHelper(this->root, new_node);
    }

    return true;

}


template<class key, class value>
void AVLTree<key, value>::insertHelper(AVLNode<key, value> *node, AVLNode<key, value> *insert) {

    if (key()(node->data, insert->data) != 1)
    {
        if (node->right_son != nullptr)
        {
            insertHelper(node->right_son, insert);
        }
        else
        {
            node->right_son= insert;
        }
    }
    else
    {
        if (node->left_son != nullptr)
        {
            insertHelper(node->left_son, insert);
        }
        else
        {
            node->left_son=insert;
        }
    }

    rotateByNeeded(node);
}

template<class key, class value>
AVLNode<key, value> *AVLTree<key,value>::Find(const value &data) const
{
    return findHelper(this->root, data);
}


template<class key, class value>
AVLNode<key, value> *AVLTree<key,value>::findHelper(AVLNode<key, value>* node, const value& data) const
{
    if (node == nullptr)
    {
        return nullptr;
    }

    if (key()(data, node->data) == 1)
    {
        return findHelper(node->right_son, data);
    }

    if (key()(data, node->data) == 0)
    {
        return node;
    }

    return findHelper(node->left_son, data);
}


template<class key, class value>
bool AVLTree<key,value>::Remove(const value &data)
{
    if (root == nullptr)
        return false;

    AVLNode<key, value>* to_delete = this->Find(data);

    if (to_delete == nullptr)
    {
        return false;
    }

    removeHelper(this->root, data);
    return true;

}

template<class key, class value>
AVLNode<key, value>* AVLTree<key,value>::removeHelper(AVLNode<key, value>* node, const value& data)
{
    if (node == nullptr)
        return node;

    if ((key()(data, node->data)) == (-1))
    {
        //node->addNumOfNodesBelow(-1);
        node->left_son= (removeHelper(node->left_son, data)) ;
    }

    else if(key()(data, node->data) == 1)
    {
        //node->addNumOfNodesBelow(-1);
        node->right_son= (removeHelper(node->right_son, data));
    }

    else
    {
        if((node->left_son == nullptr) || (node->right_son == nullptr) )
        {
            AVLNode<key, value>* temp;

            if ((node->left_son == nullptr) && (node->right_son == nullptr))
            {
                temp = node;
                node = nullptr;
            }
            else
            {
                if ((node->left_son) != nullptr)
                {
                    temp= node->left_son;
                }

                else
                {
                    temp= node->right_son;
                }

                node->data= (temp->data);

            }

            delete temp;
        }

        else
        {
            AVLNode<key, value>* temp = findMin(node->right_son);
            node->data=(temp->data);
            node->right_son=removeHelper(node->right_son, temp->data);
        }
    }

    if (node == nullptr)
        return node;

    node->updateNodeHeight();
    rotateByNeeded(node);
    return node;
}


template<class key, class value>
AVLNode<key, value>* AVLTree<key,value>:: findMin(AVLNode<key, value>* root) const
{
    if (root == nullptr)
    {
        return nullptr;
    }

    AVLNode<key, value>* temp= root;

    while (temp->left_son != nullptr)
    {
        temp= temp->left_son;
    }

    return temp;
}

template<class key, class value>
AVLNode<key, value> *AVLTree<key, value>::findIndex(AVLNode<key, value> *node, int index) const
{
    if (node == nullptr)
        return 0;

    if (node->left_son->nodesBelow() == index - 1)
    {
        return node;
    }

    else if (node->left_son->nodesBelow() > index - 1)
    {
        return findIndex(node->left_son, index);
    }

    else
    {
        return findIndex(node->right_son, (index - node->left_son->nodesBelow() - 1));
    }

}

template<class key, class value>
AVLTree<key, value>::~AVLTree() {
    delete root;
}

