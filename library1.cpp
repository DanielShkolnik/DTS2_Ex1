//
// Created by danie on 29/04/2020.
//
#include "library1.h"
#include "musicManager.h"

void *Init(){
    MusicManager *DS = new MusicManager();
    return
}

StatusType AddArtist(void *DS, int artistID, int numOfSongs);

StatusType RemoveArtist(void *DS, int artistID);

StatusType AddToSongCount(void *DS, int artistID, int songID);

StatusType NumberOfStreams(void *DS, int artistID, int songID, int *streams);

StatusType GetRecommendedSongs(void *DS, int numOfSongs, int *artists, int *songs);

void Quit(void** DS);
