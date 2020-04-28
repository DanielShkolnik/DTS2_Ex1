//
// Created by danie on 28/04/2020.
//

#ifndef DTS2_EX1_MUSICMANAGER_H
#define DTS2_EX1_MUSICMANAGER_H

#include "avl.h"
#include "node.h"
#include "artist.h"
#include "disc.h"
#include "song.h"
#include "library1.h"



class MusicManager{
private:
Avl<int,Artist> artistTree;
Node<int,Avl<int,Disc>> bestHitsListStart;
Node<int,Avl<int,Disc>> bestHitsListFinish;

public:
    MusicManager() = default;
    ~MusicManager() = default;
    MusicManager(const MusicManager& musicManager) = delete;
    MusicManager& operator=(const MusicManager& musicManager) = delete;


    StatusType AddArtist(int artistID, int numOfSongs){

    }

    StatusType RemoveArtist(int artistID){

    }

    StatusType AddToSongCount(int artistID, int songID){

    }

    StatusType NumberOfStreams(int artistID, int songID, int *streams){

    }

    StatusType GetRecommendedSongs(int numOfSongs, int *artists, int *songs){

    }



};


#endif //DTS2_EX1_MUSICMANAGER_H
