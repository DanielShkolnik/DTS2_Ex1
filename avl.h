//
// Created by danie on 26/04/2020.
//

#ifndef DTS2_EX1_AVL_H
#define DTS2_EX1_AVL_H

#include "node.h"


template <class K, class D>
class Avl{
private:
    Node<K,D> root;
    void fixBalanceFactor(Node<K,D>* leaf);
    void updateRoot(Node<K,D>* root);
    void fixRelations(Node<K,D>* v1, Node<K,D>* v2);
    void rotateLL(Node<K,D>* node);
    void rotateLR(Node<K,D>* node);
    void rotateRL(Node<K,D>* node);
    void rotateRR(Node<K,D>* node);
    int getBF(Node<K,D>* node);

public:
    Avl():root(nullptr){}
    ~Avl();
    Avl(const Avl& avl)= delete;
    Avl& operator=(const Avl& avl)= delete;
    void insert(const K& key, D* data);
    void delete_vertice(const K& key);
    D* find(const K& key);

    class Error{};
    class KeyExists{};
    class KeyNotFound{};

};

~Avl(){

}



#endif //DTS2_EX1_AVL_H
