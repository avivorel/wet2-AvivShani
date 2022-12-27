#include "UnionFind.h"

UnionFind::UnionFind(int k): k(k),arr_groups(nullptr), arr_elements(nullptr), groups_size(nullptr){
    this->arr_groups = new AVLNode<int>* [k];
    this->arr_elements = new AVLNode<int>* [k];
    this->groups_size = new int[k];
    for (int i = 0; i < k; ++i) {
        groups_size[i] =0;
    }
}

int UnionFind::MakeSet(int a) {

    auto* temp_node = new AVLNode<int>(a);
    arr_groups[a-1] =  temp_node;
    arr_elements[a-1] = temp_node;
    groups_size[a-1]++;
    return a;
}

int UnionFind::Find(int a) {
    AVLNode<int>* my_node = arr_elements[a - 1];
    AVLNode<int>* my_grandma = my_node;

    //finding the parent
    while(my_grandma->GetParent() != nullptr)
    {
        my_grandma = my_grandma->GetParent();
    }

    //updating the path to grandma
    AVLNode<int>* temp_node = my_node;
    while(temp_node->GetValue() != my_grandma->GetValue())
    {
        AVLNode<int>* temp_temp_node = temp_node;
        temp_node->SetParent(my_grandma);
        temp_node = temp_temp_node->GetParent();
    }

    return my_grandma->GetValue();
}

int UnionFind::Union(int a, int b) {

    AVLNode<int>* grandma_a = arr_groups[Find(a) - 1];
    AVLNode<int>* grandma_b = arr_groups[Find(b) - 1];
    int a_size = groups_size[grandma_a->GetValue()-1];
    int b_size = groups_size[grandma_b->GetValue()-1];

    if (a_size >= b_size)
    {
        grandma_b->SetParent(grandma_a);
        groups_size[grandma_b->GetValue()-1] = 0 ;
        arr_groups[grandma_b->GetValue()-1] = nullptr;
        groups_size[grandma_a->GetValue()-1] = a_size+b_size;
        return grandma_a->GetValue();
    }
    else
    {
        grandma_a->SetParent(grandma_b);
        groups_size[grandma_a->GetValue()-1] = 0 ;
        arr_groups[grandma_a->GetValue()-1] = nullptr;
        groups_size[grandma_b->GetValue()-1] = a_size+b_size;
        return grandma_b->GetValue();
    }
    return 0; //will never get here

}

UnionFind::~UnionFind() {
    for (int i = 0; i < this->k; i++) {
        delete  arr_elements[i];
         arr_groups[i] = nullptr;
    }
    delete [] this->arr_elements;
    delete [] this->arr_groups;
    delete [] this->groups_size;
}
