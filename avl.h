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
    void fixBalanceFactor(Node<K,D>* childVertice);
    void updateRoot(Node<K,D>* node);
    void fixRelations(Node<K,D>* parent, Node<K,D>* son);
    void rotateLL(Node<K,D>* node);
    void rotateLR(Node<K,D>* C);
    void rotateRL(Node<K,D>* C);
    void rotateRR(Node<K,D>* B);
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
    Node<K,D>* vertice = nullptr;
    try{
        vertice = this->find(key);
    }
    catch(const Avl<K,D>::KeyNotFound&){
        throw Avl<K,D>::KeyNotFound();
    }
    assert(vertice== nullptr);
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
void Avl<K,D>::fixBalanceFactor(Node<K,D>* childVertice){

    if(childVertice->getParent() == nullptr) return;
    Node<K,D>* parentVertice;
    while(childVertice){

        // Save old height and calc new height. If height hasn't changed - the tree is balanced
        int currentHeight = childVertice->getHeight();
        childVertice->calcHeight();
        int childBF = childVertice->calcBalanceFactor();

        // Calc balance factor of parent
        parentVertice = childVertice->getParent();

        // If current vertice height hasn't changed due to rotations and parent BF is legal - tree is balanced
        if(currentHeight == childVertice->getHeight() && childBF < 2 && childBF > -2){
            return;
        }

        //LL
        if(childBF == 2 && this->getBF(childVertice->getLeft())>=0){
            this->rotateLL(childVertice);
        }
        //LR
        else if(childBF == 2 && this->getBF(childVertice->getLeft())==-1){
            this->rotateLR(childVertice);
        }
        //RL
        else if(childBF==-2 && this->getBF(childVertice->getRight())==1){
            this->rotateRL(childVertice);
        }
        //RR
        else if(childBF==-2 && this->getBF(childVertice->getRight())<=0){
            this->rotateRR(childVertice);
        }
    }

}

template <class K, class D>
void Avl<K,D>::rotateLL(Node<K,D>* node){
    if(node == nullptr) return;
    Node<K,D>* parentNode = node->getParent();
    Node<K,D>* leftChild = node->getLeft();
    Node<K,D>* rightChild = leftChild->getRight();
    leftChild->setRight(node);
    node->setLeft(rightChild);
    if(parentNode != nullptr) {
        fixRelations(parentNode, leftChild);
    }
    if(rightChild != nullptr){
        fixRelations(node,rightChild);
    }
    leftChild->calcHeight();
    node->calcHeight();
}

template <class K, class D>
void Avl<K,D>::rotateLR(Node<K,D>* nodeC){
    Node<K,D>* nodeB = nodeC->getLeft();
    Node<K,D>* nodeA = nodeB->getRight();
    Node<K,D>* nodeRightA = nodeA->getRight();
    Node<K,D>* nodeLeftA = nodeA->getleft();

    // left rotation
    nodeB->setRight(nodeLeftA);
    nodeA->setLeft(nodeB);
    nodeC->setLeft(nodeA);

    // right rotation
    nodeC->setLeft(nodeRightA);
    nodeA->setLeft(nodeC);

    // fix parent-child relations for A,B,C
    if(nodeC->getParent() == nullptr) { nodeA->setParent(nullptr); }
    if(nodeLeftA != nullptr) fixRelations(nodeB, nodeLeftA);
    if(nodeRightA != nullptr) fixRelations(nodeC, nodeRightA);

    // Calc new height for vertices who's height have changed
    nodeA->calcHeight();
    nodeB->calcHeight();
    nodeC->calcHeight();
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

template <class K, class D>
void Avl<K,D>::fixRelations(Node<K,D>* parent, Node<K,D>* son){
    if (!parent || !son) throw Avl<K,D>::Error();
    if (parent->getKey()>son->getKey()) parent->setLeft(son);
    else parent->setRight(son);
    son->setParent(parent);
}

template <class K, class D>
Node<K,D>* Avl<K,D>::find(const K& key){
    if(this->root == nullptr) throw Avl<K,D>::KeyNotFound(); // avl is empty->
    Node<K,D>* currentNode = this->root;
    Node<K,D>* prevNode = this->root;
    while (currentNode){
        prevNode = currentNode;
        if(key > currentNode->getKey()){
            currentNode = currentNode->getRight();
        } else if(key == currentNode->getKey()){
            return currentNode;
        } else{
            currentNode = currentNode->getLeft();
        }
    }
    throw Avl<K,D>::KeyNotFound();
}

template <class K, class D>
int Avl<K,D>::getBF(Node<K,D>* node){
    if(node->getLeft()== nullptr && node->getRight()== nullptr) return 0; // leaf
    else if(node->getRight()==nullptr) return node->getLeft()->getHeight(); // no right son
    else if(node->getLeft()==nullptr) return -(node->getRight()->getHeight()); // // no left son
    return node->getLeft()->getHeight()-node->getRight()->getHeight();
}
template <class K, class D>
void Avl<K,D>::rotateRR(Node<K,D>* B){
    if(B == nullptr) return;
    Node<K,D>* BParent=B->getPapa();
    Node<K,D>* A=B->getRight();
    Node<K,D>* ALeft=A->getLeft();
    A->setLeft(B);
    B->setRight(ALeft);
    if (BParent!=nullptr)fixRelations(BParent,A);
    else A->setParent(nullptr);
    fixRelations(A,B);
    if (ALeft!= nullptr) fixRelations(B,ALeft);
    B->calcHeight();
    A->calcHeight();
}

template <class K, class D>
void Avl<K,D>::rotateRL(Node<K,D>* C){
    if(C == nullptr) return;

    Node<K,D>* CParent=C->getParent();
    Node<K,D>* B=C->getRight();
    Node<K,D>* A=B->getLeft();
    Node<K,D>* ALeft=A->getLeft();
    Node<K,D>* ARight=A->getRight();
    A->setLeft(C);
    C->setRight(ALeft);
    A->setRight(B);
    B->setLeft(ARight);
    if(CParent!= nullptr)fixRelations(CParent,A);
    else A->setParent(nullptr);
    if(ALeft!= nullptr)fixRelations(C,ALeft);
    fixRelations(A,B);
    fixRelations(A,C);
    if(ARight!= nullptr)fixRelations(B,ARight);
    C->calcHeight();
    B->calcHeight();
    A->calcHeight();
}




#endif //DTS2_EX1_AVL_H
