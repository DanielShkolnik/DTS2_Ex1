//
// Created by danie on 27/04/2020.
//
#include <iostream>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "avl.h"

using  namespace std;

void print(Node<int,int>* node){
    std::cout << *(node->getData()) << std::endl;
}


int main(int argc, const char**argv) {
    //Test1
    Avl<int,int> avl;
    int a=1,c=5,d=4;
    int* b=&a;
    //*a=1; *b=5; *c=3; *d=2; *e=10; *f=15; *g=20; *h=7; *i=9; *j=4; *k=11;

    //avl.insert(*a,a);
    avl.insert(*b,b);
    /*
    avl.insert(*c,c);
    avl.insert(*d,d);
    avl.insert(*e,e);
    avl.insert(*f,f);
    avl.insert(*g,g);
    avl.insert(*h,h);
    avl.insert(*i,i);
    avl.insert(*j,j);
    avl.insert(*k,k);
*/
    Node<int,int>* root = avl.getRoot();
    inorder<int,int,void (Node<int,int>* node)>(root,print);


    return 0;
}
