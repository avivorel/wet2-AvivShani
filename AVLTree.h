
template<class key, class value>
class AVLNode {
    value data;
    AVLNode<key, value> *left_son;
    AVLNode<key, value> *right_son;
    int height;
    int numOfNodesBelow_include;

public:

    AVLNode() : data(nullptr), left_son(nullptr), right_son(nullptr), height(0), numOfNodesBelow_include(1) {}

    explicit AVLNode(const value &data) : data(data), left_son(nullptr), right_son(nullptr), height(0)
    , numOfNodesBelow_include(1) {}

    ~AVLNode();

    value getValue() const { return data; }

    void setValue(const value &new_data)  { this->getValue()= new_data;}

    void setLeft(AVLNode<key, value> *left) { left_son = left; }

    AVLNode<key, value> *getLeft() const { return left_son; }

    void setRight(AVLNode<key, value> *right) { right_son = right; }

    AVLNode<key, value> *getRight() const { return right_son; }

    void setHeight(int new_height) { this->height = new_height; }

    int getHeight() const { return this->height; }

    int nodesBelow() const { return this->numOfNodesBelow_include; }

    void addNumOfNodesBelow(int n)  { this->numOfNodesBelow_include+= n ; }

    void setNumOfNodesBelow(int n)  { this->numOfNodesBelow_include= n ; }

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

    bool insertHelper(AVLNode<key, value> *node, const value &data);

    void rotateByNeeded(AVLNode<key, value> *node);

public:

    AVLTree() : root(nullptr) {}

    ~AVLTree();

    AVLNode<key, value> *find(const value &data) const;

    AVLNode<key, value> *findHelper(AVLNode<key, value> *node, const value &data) const;

    bool insert(const value &data);

    bool remove(const value &data);

    AVLNode<key, value>* removeHelper(AVLNode<key, value>* node, const value& data);

    int nodeHeight( AVLNode<key, value> *node) const;

    int getNodeBF( AVLNode<key, value> *node) const;

    AVLNode<key, value> *getRoot();

    AVLNode<key, value>* findMax(AVLNode<key, value>* root) const;

    AVLNode<key, value>* findMin(AVLNode<key, value>* root) const;

    AVLNode<key, value>* findIndex(AVLNode<key, value>* node, int index) const;


};

template<class key, class value>
AVLNode<key, value>* AVLTree<key,value>:: getRoot()
{
    return this->root;
}

template<class key, class value>
int AVLTree<key,value>:: nodeHeight( AVLNode<key, value> *node) const
{
    if (node->getRight() != nullptr && node->getLeft() != nullptr)
    {
        if (node->getRight()->getHeight() > node->getLeft()->getHeight())
            return node->getRight()->getHeight() + 1;

        else
            return node->getLeft()->getHeight() + 1;
    }

    if (node->getRight() != nullptr)
    {
        return node->getRight()->getHeight() + 1;
    }

    if (node->getLeft() != nullptr)
    {
        return node->getLeft()->getHeight() + 1;
    }

    return 0;
}

template<class key, class value>
int AVLTree<key,value>:: getNodeBF ( AVLNode<key, value> *node) const
{
    int leftheight;
    int rightheight;
    if (node->getLeft() == nullptr){
        leftheight = -1;
    }
    else
    {
        leftheight = node->getLeft()->getHeight();
    }
    if(node->getRight() == nullptr){
        rightheight = -1;
    }
    else{
        rightheight = node->getRight()->getHeight();
    }
    return leftheight - rightheight;

}

template<class key, class value>
void AVLTree<key,value>:: rotateLeft (AVLNode<key, value>* node)
{
    AVLNode<key, value>* temp1 = node->getRight();
    AVLNode<key, value>* temp2= temp1->getLeft();

    node->setNumOfNodesBelow(node->getLeft()->nodesBelow() + temp2->nodesBelow() + 1);
    temp1->setNumOfNodesBelow(temp1->getRight()->nodesBelow() + node->nodesBelow() + 1);

    temp1->setLeft(node);
    node->setRight(temp2);

    if (nodeHeight(node->getLeft()) > nodeHeight(node->getRight()))
    {
        node->setHeight(nodeHeight(node->getLeft()) + 1);
    }

    else
    {
        node->setHeight(nodeHeight(node->getRight()) + 1);
    }

    if (nodeHeight(temp1->getLeft()) > nodeHeight(temp1->getRight()))
    {
        temp1->setHeight(nodeHeight(temp1->getLeft() + 1));
    }

    else
    {
        temp1->setHeight(nodeHeight(temp1->getRight() + 1));
    }
}

template<class key, class value>
void AVLTree<key,value>:: rotateRight (AVLNode<key, value>* node)
{
    AVLNode<key, value>* temp1 = node->getLeft();
    AVLNode<key, value>* temp2= temp1->getRight();

    node->setNumOfNodesBelow(temp2->nodesBelow() + node->getRight()->nodesBelow() + 1);
    temp1->setNumOfNodesBelow(temp1->getLeft()->nodesBelow() + node->nodesBelow() + 1);

    temp1->setRight(node);
    node->setLeft(temp2);

    if (nodeHeight(node->getLeft()) > nodeHeight(node->getRight()))
    {
        node->setHeight(nodeHeight(node->getLeft()) + 1);
    }

    else
    {
        node->setHeight(nodeHeight(node->getRight()) + 1);
    }

    if (nodeHeight(temp1->getLeft()) > nodeHeight(temp1->getRight()))
    {
        temp1->setHeight(nodeHeight(temp1->getLeft() + 1));
    }

    else
    {
        temp1->setHeight(nodeHeight(temp1->getRight() + 1));
    }
}


template<class key, class value>
void AVLTree<key,value>::rotateByNeeded(AVLNode<key, value> *node)
{

    if (getNodeBF(node) == -2 && getNodeBF(node->getRight()) <= 0)
    {
        rotateLeft(node);
        return;
    }

    if (getNodeBF(node)  == -2 && getNodeBF(node->getRight()) == 1)
    {
        rotateRight(node->getRight());
        rotateLeft(node);
        return;
    }

    if (getNodeBF(node)  == 2 && getNodeBF(node->getLeft()) == -1)
    {
        rotateLeft(node->getLeft());
        rotateRight(node);
        return;
    }

    if (getNodeBF(node) == 2 && getNodeBF(node->getLeft()) >= 0)
    {
        rotateRight(node);
        return;
    }

    node->setHeight(nodeHeight(node));
}

template<class key, class value>
bool AVLTree<key,value>::insert(const value &data)
{
    if (this->root == nullptr)
    {
        auto* newNode = new  AVLNode<key, value>(data);

        if (newNode == nullptr)
        {
            return false;
        }

        this->root = newNode;
        return true;
    }

    if (this->find(data))
    {
        return false;
    }

   return insertHelper(this->root, data);

}


template<class key, class value>
bool AVLTree<key, value>::insertHelper(AVLNode<key, value> *node, const value &data) {

    if (key()(node->getValue(), data) != 1)
    {
        if (node->getRight() != nullptr)
        {
            node->addNumOfNodesBelow(1);
            insertHelper(node->getRight(), data);
        }
        else
        {
            auto* newNode = new  AVLNode<key, value> (data);

            if (newNode == nullptr)
            {
                return false;
            }

            node->setRight(newNode);
        }
    }
    else
    {
        if (node->getLeft() != nullptr)
        {
            node->addNumOfNodesBelow(1);
            insertHelper(node->getLeft(), data);
        }
        else
        {
            auto* newNode = new  AVLNode<key, value>(data);

            if (newNode == nullptr)
            {
                return false;
            }

            node->setLeft(newNode);
        }
    }

    rotateByNeeded(node);
    return true;
}

template<class key, class value>
AVLNode<key, value> *AVLTree<key,value>::find(const value &data) const
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

    if (key()(data, node->getValue()) == 1)
    {
        return findHelper(node->getRight(), data);
    }

    if (key()(data, node->getValue()) == 0)
    {
        return node;
    }

    return findHelper(node->getLeft(), data);
}


template<class key, class value>
bool AVLTree<key,value>::remove(const value &data)
{
    if (root == nullptr)
        return false;

    AVLNode<key, value>* to_delete = this->find(data);

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

    if (key()(data, node->getValue()) == (-1))
    {
        node->addNumOfNodesBelow(-1);
        node->setLeft(removeHelper(node->getLeft(), data)) ;
    }

    if(key()(data, node->getValue()) == 1)
    {
        node->addNumOfNodesBelow(-1);
        node->setRight(removeHelper(node->getRight(), data));
    }

    if (key()(data, node->getValue()) == 0)
    {
        if((node->getLeft() == nullptr) || (node->getRight() == nullptr) )
        {
            AVLNode<key, value>* temp;

            if ((node->getLeft() == nullptr) && (node->getRight() == nullptr))
            {
                temp = node;
                node = nullptr;
            }
            else
            {
                if (node->getLeft() != nullptr)
                {
                    temp= node->getLeft();
                }

                else
                {
                    temp= node->getRight();
                }

                node->setValue(temp->getValue());

            }

            delete temp;
        }

        else
        {
            AVLNode<key, value>* temp = findMin(node->getRight());
            node->setValue(temp->getValue());
            node->setRight(removeHelper(node->getRight(), temp->getValue()));
        }
    }

    if (node == nullptr)
        return node;

    node->setHeight(nodeHeight(node));
    rotateByNeeded(node);
    return node;
}

template<class key, class value>
AVLNode<key, value>* AVLTree<key,value>:: findMax(AVLNode<key, value>* root) const
{
    if (root == nullptr)
    {
        return nullptr;
    }

    AVLNode<key, value>* temp= root;

    while (temp->getRight() != nullptr)
    {
        temp= temp->getRight();
    }

    return temp;
}

template<class key, class value>
AVLNode<key, value>* AVLTree<key,value>:: findMin(AVLNode<key, value>* root) const
{
    if (root == nullptr)
    {
        return nullptr;
    }

    AVLNode<key, value>* temp= root;

    while (temp->getLeft() != nullptr)
    {
        temp= temp->getLeft();
    }

    return temp;
}

template<class key, class value>
AVLNode<key, value> *AVLTree<key, value>::findIndex(AVLNode<key, value> *node, int index) const
{
    if (node->getLeft()->nodesBelow() == index-1)
    {
        return node;
    }

    if (node->getLeft()->nodesBelow() > index-1)
    {
        findIndex(node->getLeft(), index);
    }
    if (node->getLeft()->nodesBelow() < index-1)
    {
        findIndex(node->getRight(), (index - node->getLeft()->nodesBelow() -1));
    }
}

