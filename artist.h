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
    Node<int,Song>** songArray;
public:
    Artist(int artistID,int numOfSongs,Song** songs){
        if(artistID<=0 || numOfSongs<=0){
            throw INVALID_INPUT();
        }
        Node<int,Song>** songArray = new Node<int,Song>*[numOfSongs]();
        for (int i = 0; i < numOfSongs; ++i) {
            songArray[i] = new Node<int,Song>(i, new Song(i, this->artistID));
        }
    };
    ~Artist(){
        for (int i = 0; i < numOfSongs; ++i) {
            delete this->songArray[i];
        }
        delete[] songArray;
    };
    Artist(const Artist& avl)= delete;
    Artist& operator=(const Artist& avl)= delete;
    void addCount(int songID){
        songArray[songID]->getData()->increasePopularity();
    }
    int getArtistID(){
        return this->artistID;
    }
    void operator()(Node<int,Song>* songArray, Node<int,Song> songPtr);
    class INVALID_INPUT{};
    class ALLOCATION_ERROR{};
    class FAILURE{};
    class SUCCESS{};
};


#endif //DTS2_EX1_ARTIST_H
