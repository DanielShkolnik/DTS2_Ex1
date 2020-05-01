//
// Created by Eleanor on 27/04/2020.
//

#ifndef DTS2_EX1_ARTIST_H
#define DTS2_EX1_ARTIST_H

#include "node.h"
#include "song.h"

class Artist{
private:
    int artistID;
    int numOfSongs;
    Song** songArray;
    Node<int,Disc>** discNodeArray;
    int indexDiscArray;
public:
    Artist(int artistID,int numOfSongs):artistID(artistID),numOfSongs(numOfSongs),indexDiscArray(0){
        this->songArray = new Song*[numOfSongs]();
        for (int i = 0; i < this->numOfSongs; ++i) {
            songArray[i] = new Song(i, this->artistID);
        }
        this->discNodeArray = new Node<int,Disc>*[numOfSongs]();
    };
    ~Artist(){
        for (int i = 0; i < numOfSongs; ++i) {
            delete this->songArray[i];
        }
        delete[] songArray;
        delete [] discNodeArray;
    };
    Artist(const Artist& artist)= delete;
    Artist& operator=(const Artist& artist)= delete;
    void addCount(int songID){
        songArray[songID]->increasePopularity();
    }
    int getArtistID() const {
        return this->artistID;
    }

    void initNewDiscNode(Node<int,Disc>* disc){
        this->discNodeArray[this->indexDiscArray] = disc;
        this->indexDiscArray++;
    }

    void setDiscNode(int index,Node<int,Disc>* disc){
        this->discNodeArray[index] = disc;
    }

    Node<int,Disc>* getDiscNode(int index){
        return this->discNodeArray[index];
    }

    Song* getSong(int index){
        return this->songArray[index];
    }

    int getNumOfSongs() const {
        return this->numOfSongs;
    }
    int getLastDiscIndex(){
        return this->indexDiscArray-1;
    }

    class INVALID_INPUT{};
};


#endif //DTS2_EX1_ARTIST_H
