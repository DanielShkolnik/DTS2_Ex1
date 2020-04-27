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
public:
    Artist(int artistID,int numOfSongs,Song** songs){
        if(artistID<=0 || numOfSongs<=0){
            throw INVALID_INPUT();
        }
        for (int i = 0; i < numOfSongs; ++i) {
            Node<int,Song>* song = new Node<int,Song>();
            songArray[i] = song;
        }

    };
    ~Artist();
    Artist(const Artist& avl)= delete;
    Artist& operator=(const Artist& avl)= delete;

    class INVALID_INPUT{};
    class ALLOCATION_ERROR{};
    class FAILURE{};
    class SUCCESS{};

    /*void insert(const K& key, D* data);
    void deleteVertice(const K& key);
    Node<K,D>* find(const K& key);*/
};

#endif //DTS2_EX1_ARTIST_H
