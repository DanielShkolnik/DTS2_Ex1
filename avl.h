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
    Node<K,D> getNextAvailable(Node<K,D>& node);
    void removeFromParent(Node<K,D>* node);
    bool isRightSon(Node<K,D>* node);
    bool isLeftSon(Node<K,D>* node);


public:
    Avl():root(nullptr){}
    ~Avl();
    Avl(const Avl& avl)= delete;
    Avl& operator=(const Avl& avl)= delete;
    void insert(const K& key, D* data);
    void deleteVertice(const K& key);
    Node<K,D>* find(const K& key);

    class Error{};
    class KeyExists{};
    class KeyNotFound{};

};

template <class K, class D>
Avl<K,D>::~Avl(){
    delete this->root;
}

template <class K, class D>
void Avl<K,D>::deleteVertice(const K& key){

    // left->right->right....
    Node<K,D>* vertice = this->find(key);
    if(vertice == nullptr) throw Avl<K,D>::KeyNotFound();

    // only one element
    if(vertice->isLeaf() && vertice->isRoot()){
        delete vertice;
        this->root = nullptr;
        return;
    }

    //leaf and not root => parent exists
    else if(vertice->isLeaf()){
        removeFromPaparent(vertice);
        this->fixBalanceFactor(vertice->getParent()); // fix balance from leaf parent
        this->updateRoot(vertice->getParent());
    }
    //no left son but not leaf => right son
    else if(vertice->getLeft()== nullptr){
        if(vertice->getParent() != nullptr) fixRelations(vertice->getParent(),vertice->getRight());
        else vertice->getRight()->setParent(nullptr);
        fix_BFs(vertice->getRight());
        this->updateRoot(vertice->getRight());
    }
        //find the left->right->right....son
    else{
        Node<K,D>* current = vertice->getLeft();
        while (current->getRight()!= nullptr){
            current = current->getRight();
        }
        // path is only one to the left
        if(current == vertice->getLeft()){
            if (vertice->getParent() != nullptr)fixRelations(vertice->getParent(),current);
            else current->setParent(nullptr); // if the vertice is the root.
            if(vertice->getRight() != nullptr) fixRelations(current,vertice->getRight());
            else current->setRight(nullptr);
            this->fixBalanceFactor(current);
            this->updateRoot(current);
        }
        else{
            Node<K,D>* changedFrom = current->getParent(); // save parent of leaf to fix balance from
            if(current->getLeft() != nullptr) fixRelations(current->getPapa(),current->getLeft());
            else current->getParent()->setRight(nullptr);
            if(vertice->getParent() != nullptr) fixRelations(vertice->getPapa(),current);
            else current->getParent(nullptr); // if the nearest is the root
            if(vertice->getRight() != nullptr) fixRelations(current,vertice->getRight());
            else current->setRight(nullptr);
            fixRelations(current,vertice->getLeft());
            this->fix_BFs(changedFrom);
            this->updateRoot(current);
        }

    }
    delete vertice;
}

// Gets the key and returns element with the nearest existing key
template <class K, class D>
Node<K,D> Avl<K,D>::getNextAvailable(Node<K,D>& node){
    // Avl is empty
    if(this->root == nullptr) return nullptr;

    // Create iterator
    Node<K,D> iter = this->root;

    // Start recursive search on right sub-tree
    if(node->key = iter->key){
        return iter;
    }

    // Start recursive search on right sub-tree
    if(node->key > iter->key){
        getNextAvailable(iter->getRight());
    }

    // Start recursive search on left sub-tree
    if(node->key < iter->key){
        getNextAvailable(iter->getLeft());
    }

    return this->root;
}

template <class K, class D>
void Avl<K,D>::insert(const K& key, D* data){
    Node<K,D> nearest = getNextAvailable(key);
    if(nearest != nullptr){
        if (nearest->getKey()==key) throw Avl<K,D>::KeyExists();
    }
    Node<K,D> newNode = new Node<K,D>(key,data,nearest);
    if(nearest == nullptr){
        this->root = newNode;
        return;
    }
    if(key > nearest->getKey()) nearest.setRight();
    else if (key < nearest->getKey()) nearest.getLeft();
    this->fixBalanceFactor(newNode);
    this->update_head(newNode);
}


template <class K, class D>
void Avl<K,D>::removeFromParent(Node<K,D>* node){
    if(node->isRoot()) return;
    if(isLeftSon(node)) node->getParent()->setLeft(nullptr);
    else node->getParent()->setRight(nullptr);
}

template <class K, class D>
bool Avl<K,D>::isLeftSon(Node<K,D>* node){
    if(node->getParent()->getLeft()== nullptr) return false;
    return node->getParent()->getLeft()->getKey()==node->getKey();
}

template <class K, class D>
bool Avl<K,D>::isRightSon(Node<K,D>* node){
    if(node->getParent()->getRight()== nullptr) return false;
    return node->getParent()->getRight()->getKey()==node->getKey();
}

template <class K, class D>
void Avl<K,D>::updateRoot(Node<K,D>* node){
    while(node->getPapa() != nullptr){
        node = node->getPapa();
    }
    this->root = node;
}

#endif //DTS2_EX1_AVL_H
