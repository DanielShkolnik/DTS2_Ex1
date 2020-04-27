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
    std::cout << *(node->getData()) << "-";
}



int main(int argc, const char**argv) {

    // Test1 - Insersion
/*Avl<int,int> avl;
int a=1,cc=5,dd=2,ee=3,ff=10,gg=19,hh=16,ii=20,jj=30,kk=4;
int* b=&a;
int* c=&cc;
int* d=&dd;
int* e=&ee;
int* f=&ff;
int* g=&gg;
int* h=&hh;
int* i=&ii;
int* j=&jj;
int* k=&kk;



avl.insert(*b,b);
avl.insert(*c,c);
avl.insert(*d,d);
try {
avl.insert(*b,b);
} catch (const Avl<int,int>::KeyExists&){
cout << "Test key exist!" << endl;
}
avl.insert(*e,e);
avl.insert(*f,f);
avl.insert(*g,g);
avl.insert(*h,h);
avl.insert(*i,i);
avl.insert(*j,j);
avl.insert(*k,k);

Node<int,int>* root = avl.getRoot();

cout <<  "preorder" << endl;
preorder<int,int,void (Node<int,int>* node)>(root,print);
cout << endl;
cout << "inorder" << endl;
inorder<int,int,void (Node<int,int>* node)>(root,print);
*/
    // Test2 - LL
    //cout << endl;
    cout << "Test LL!" << endl;
    Avl<int,int> avl2;
    int x=15,y=10,z=5,w=12,v=17,u=20, t=11,s=13,q=14,r=50,a=30,b=7,c=4,d=8;
    int* xx=&x; int* yy=&y; int* zz=&z;int* ww=&w;int* vv=&v; int* uu=&u;int* tt=&t; int*ss = &s; int* qq=&q; int*rr = &r; int* aa=&a;int* bb=&b; int*cc = &c; int* dd=&d;
    avl2.insert(*xx,xx);
    avl2.insert(*yy,yy);
    avl2.insert(*zz,zz);
    avl2.insert(*ww,ww);
    avl2.insert(*vv,vv);
    avl2.insert(*uu,uu);
    avl2.insert(*tt,tt);
    avl2.insert(*ss,ss);
    avl2.insert(*qq,qq);

    avl2.deleteVertice(*vv);

    avl2.insert(*rr,rr);
    avl2.insert(*aa,aa);

    //RR
    avl2.deleteVertice(*qq);
    avl2.deleteVertice(*xx);
    //LL
    avl2.deleteVertice(*rr);
    avl2.deleteVertice(*aa);

    avl2.insert(*rr,rr);
    avl2.insert(*aa,aa);
    avl2.insert(*xx,xx);
    avl2.insert(*vv,vv);


    avl2.deleteVertice(*rr);
    avl2.deleteVertice(*aa);


    avl2.insert(*bb,bb);
    avl2.insert(*cc,cc);
    avl2.insert(*dd,dd);

    avl2.deleteVertice(*zz);
    avl2.deleteVertice(*cc);


    //cout << "tree before deletion" << endl;
    //preorder<int,int,void (Node<int,int>* node)>(avl2.getRoot(),print);
    //cout << endl;




    cout << "tree after deletion" << endl;

    cout <<  "preorder" << endl;
    preorder<int,int,void (Node<int,int>* node)>(avl2.getRoot(),print);
    cout << endl;
    cout << "inorder" << endl;
    inorder<int,int,void (Node<int,int>* node)>(avl2.getRoot(),print);


    return 0;
}
