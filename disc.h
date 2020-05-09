//
// Created by danie on 28/04/2020.
//

#ifndef DTS2_EX1_DISC_H
#define DTS2_EX1_DISC_H

#include "avl.h"
#include "song.h"

class Disc{
private:
    int artistID;
    Node<int,Avl<int,Disc>>* rankPtr;
    Avl<int,Song>* songTree;

public:
    Disc() = delete;
    explicit Disc(int artistID):artistID(artistID),rankPtr(nullptr){
        this->songTree = new Avl<int,Song>();
    };
    ~Disc(){
        delete this->songTree;
    }
    Disc(const Disc& disc) = delete;
    Disc& operator=(const Disc& disc) = delete;

    Avl<int,Song>* getSongTree(){
        return this->songTree;
    }

    int getArtistID() const {
        return this->artistID;
    }

    Node<int,Avl<int,Disc>>* getRankPtr(){
        return this->rankPtr;
    }

    void addSong(Song* song){
        this->songTree->insert(song->getSongID(),song);
    }

    void removeSong(int songID){
        this->songTree->deleteVertice(songID);
    }

    /* pointer to the element of the doubly linked list which is ordered by number of streams.
     * Each disc holds song tree with songs which belong to the same artist.
     * all the songs in the disc also have the same number of streams. this number defines the disc's popularity.
     * Each disc is saved in a node of a disc tree which is pointed by an element of the rank list, with the same rank
     * as the popularity of the disc.
     * */
    void setRankPtr(Node<int,Avl<int,Disc>>* rankPtrToSet){
        this->rankPtr = rankPtrToSet;
    }




};



#endif //DTS2_EX1_DISC_H
