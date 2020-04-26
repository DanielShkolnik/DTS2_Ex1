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

public:
    Avl():root(nullptr){}
    ~Avl();
    Avl(const Avl& avl)= delete;
    Avl& operator=(const Avl& avl)= delete;
    void insert(const K& key, D* data);
    void delete_vertice(const K& key);
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
void Avl<K,D>::delete_vertice(const K& key){
    // left->right->right....
    Node<K,D>* vertice = this->find(key);
    if(vertice == nullptr) throw Avl<K,D>::KeyNotFound();

    // only one element
    if(nearest->isLeaf() && nearest->isRoot()){
        this->head = nullptr;
        return;
    }
        //leaf and not root => papa exists
    else if(nearest->isLeaf()){
        remove_from_papa(nearest);
        this->fix_BFs(nearest->getPapa()); // fix balance from leaf parent
        this->update_head(nearest->getPapa());
    }
        //no left son but not leaf => right son and papa exists
    else if(nearest->getLeft()== nullptr){
        if(nearest->getPapa() != nullptr) fix_relations(nearest->getPapa(),nearest->getRight());
        else nearest->getRight()->setPapa(nullptr);
        fix_BFs(nearest->getRight());
        this->update_head(nearest->getRight());
    }
        //find the left->right->right....son
    else{
        std::shared_ptr<Node<K,D>> current = nearest->getLeft();
        while (current->getRight()!= nullptr){
            current = current->getRight();
        }
        // path is only one to the left
        if(current == nearest->getLeft()){
            if (nearest->getPapa() != nullptr)fix_relations(nearest->getPapa(),current);
            else current->setPapa(nullptr); // if the nearest is the root.
            if(nearest->getRight() != nullptr) fix_relations(current,nearest->getRight());
            else current->setRight(nullptr);
            this->fix_BFs(current);
            this->update_head(current);
        }
        else{
            std::shared_ptr<Node<K,D>> changed_from = current->getPapa(); // save parent of leaf to fix balance from
            if(current->getLeft() != nullptr) fix_relations(current->getPapa(),current->getLeft());
            else current->getPapa()->setRight(nullptr);
            if(nearest->getPapa() != nullptr) fix_relations(nearest->getPapa(),current);
            else current->setPapa(nullptr); // if the nearest is the root
            if(nearest->getRight() != nullptr) fix_relations(current,nearest->getRight());
            else current->setRight(nullptr);
            fix_relations(current,nearest->getLeft());
            this->fix_BFs(changed_from);
            this->update_head(current);
        }

    }
}

// Gets the key and returns element with the nearest existing key
template <class K, class D>
Node<K,D> getNextAvailable(Node<K,D>& node){
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

    return this->head;
}

template <class K, class D>
void insert(const K& key, D* data){
    Node<K,D> nearest = getNextAvailable(key);
    if(nearest != nullptr){
        if (nearest->getKey()==key) throw Avl<K,D>::KeyExists();
    }
    Node<K,D> newNode = <Node<K,D>>(new Node<K,D>(key,data,nearest));
    if(nearest == nullptr){
        this->root = newNode;
        return;
    }
    if(key > nearest->getKey()) nearest.setRight();
    else if (key < nearest->getKey()) nearest.getLeft();
    this->fixBalanceFactor(newNode);
    this->update_head(newNode);
}


#endif //DTS2_EX1_AVL_H
