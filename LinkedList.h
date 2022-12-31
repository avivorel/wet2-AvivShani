//
// Created by darkm on 08/12/2022.
//

#ifndef SOLUTION_LINKEDLIST_H
#define SOLUTION_LINKEDLIST_H

template<typename T> class Node{
public:
    T data;
    int score;
    Node* next;
    Node* prev;
    ~Node() = default;
    Node(T data){
        this->data = data;
        next = NULL;
        prev = NULL;
        score = 0;
    }
    void setData(T newdata){
        data = newdata;
    }
    void addToScore(int to_add){
        score = score + to_add + 3;
    }
};

template<typename T>
class LinkedList {
private:
    Node<T> *head;
    Node<T> *tail;
public:
    LinkedList() {
        head = NULL;
        tail = NULL;
    }
    ~LinkedList() = default;
    void insert(T data) {
        Node<T> *newNode = new Node<T>(data);
        if (head == NULL) {
            head = newNode;
            tail = newNode;
        } else {
            tail->next = newNode;
            newNode->prev = tail;
            tail = newNode;
        }
    }
    Node<T>* getHead(){
        return this->head;
    }
    Node<T>* getTail(){
        return this->tail;
    }
    void remove(Node<T> *node) {
        if (node == nullptr){
            return;
        }
        // if the node to be removed is the head
        if (node == head) {
            head = node->next;
        } else {
            node->prev->next = node->next;
        }
        // if the node to be removed is the tail
        if (node == tail) {
            tail = node->prev;
        } else {
            node->next->prev = node->prev;
        }
        delete node;
    }
    void deleteList() {
        Node<T> *current = head;
        while (current != NULL) {
            Node<T> *next = current->next;
            delete current;
            current = next;
        }
        head = NULL;
        tail = NULL;
    }
};

#endif //SOLUTION_LINKEDLIST_H
