#ifndef DS2_UNIONFIND_H
#define DS2_UNIONFIND_H

#include "AVLTree.h"

class UnionFind
{

private:

    int k;
    AVLNode<int>** arr_elements;
    AVLNode<int>** arr_groups;

public:
    //remember to make private
    int* groups_size;

    explicit UnionFind(int k);

    int MakeSet(int a);

    int Find(int a);

    int Union(int a, int b);

    ~UnionFind();
};


#endif //DS2_UNIONFIND_H
