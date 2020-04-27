//
// Created by danie on 26/04/2020.
//

#ifndef DTS2_EX1_NODE_H
#define DTS2_EX1_NODE_H

template <class K, class D>
class Node{
private:
    K key;
    D* data;
    Node<K,D>* parent;
    Node<K,D>* left;
    Node<K,D>* right;
    int height;

public:
    Node()= delete;

    // need to allocate space for data/paret/left/right?
    Node(K key, D* data, Node* parent):key(key),data(data),parent(parent),left(nullptr),right(nullptr),height(1){};
    // fix D'tor if needed
    ~Node() = default;
    Node(const Node&)= delete;
    Node operator=(const Node&)= delete;
    const K& getKey(){
        return this->key;
    }
    D* getData(){
        return this->data;
    }
    void setData(D* data){
        this->data=data;
    }
    Node<K,D>* getParent(){
    return this->parent;
    }
    void setParent(Node<K,D>* node){
        this->parent=node->parent;
    }
    Node<K,D>* getLeft(){
        return this->left;
    }
    void setLeft(Node<K,D>* node){
        this->left=node->left;
    }
    Node<K,D>* getRight(){
        return this->right;
    }
    void setRight(Node<K,D>* node){
        this->right=node->right;
    }
    int getHeight() const{
        return this->height;
    }
    bool isLeaf(){
        return  (this->right == nullptr && this->left == nullptr);
    }
    bool isRoot(){
        return this->parent== nullptr;
    }
    void calcHeight(){
        if(this->left== nullptr && this->right== nullptr)this->height = 1; //leaf
        else if(this->left== nullptr) this->height = this->right->height +1; // no left son
        else if(this->right== nullptr) this->height = this->left->height +1; // no right son
        else this->height= max(this->left->height,this->right->height) + 1; // max between sons
    }


    //List node methods
    Node(K key, D* data,Node* prev,Node* next):key(key),data(data),parent(nullptr),left(prev),right(next),height(1){}
    Node(K key, D* data):key(key),data(data),parent(nullptr),left(nullptr),right(nullptr),height(1){}
    void setPrev(Node* prev){
        this->left=prev;
    }
    Node* getPrev(){
        return this->left;
    }
    void setNext(Node* next){
        this->right=next;
    }
    Node* getNext(){
        return this->right;
    }

};


static int max(int a, int b){
    if (a>b) return a;
    return b;
}



#endif //DTS2_EX1_NODE_H
