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
    int numOfSongs;
    Node<int,Avl<int,Disc>>* rankPtr;
    Avl<int,Song>* songTree;

public:
    Disc() = delete;
    Disc(int artistID):artistID(artistID),rankPtr(nullptr){
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

    int getArtistID(){
        return this->artistID;
    }

    Node<int,Avl<int,Disc>>* getRankPtr(){
        return this->rankPtr;
    }

    void addSong(Song* song){
        this->songTree->insert(song->getSongID(),song);
        this->numOfSongs++;
    }

    void removeSong(int songID){
        this->songTree->deleteVertice(songID);
    }

    void setRankPtr(Node<int,Avl<int,Disc>>* rankPtr){
        this->rankPtr = rankPtr;
    }



};



#endif //DTS2_EX1_DISC_H
