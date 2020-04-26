//
// Created by danie on 26/04/2020.
//

#ifndef DTS2_EX1_NODE_H
#define DTS2_EX1_NODE_H

template <class K, class D>
class Node{
private:
    K key;
    D& data;
    Node<K,D>& parent;
    Node<K,D>& left;
    Node<K,D>& right;
    int height;

public:
    Node()= delete;
    Node(K key, D& data, Node& parent):key(key),data(data),parent(parent),height(1){};
    ~Node() = default;
    Node(const Node&)= delete;
    Node operator=(const Node&)= delete;
    Node<K,D>& getParent(){
    return this->parent;
    }
    void setParent(Node<K,D>& node){
        this->parent=node->parent;
    }
    Node<K,D>& getLeft(){
        return this->left;
    }
    void setLeft(Node<K,D>& node){
        this->left=node->left;
    }
    Node<K,D>& getRight(){
        return this->right;
    }
    void setRight(Node<K,D>& node){
        this->right=node->right;
    }
    int getHeight() const{
        return this->height;
    }



}

#endif //DTS2_EX1_NODE_H
