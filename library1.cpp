//
// Created by danie on 29/04/2020.
//
#include "library1.h"
#include "musicManager.h"

void *Init(){
    MusicManager *DS = new MusicManager();
    return (void*)DS;
}

StatusType AddArtist(void *DS, int artistID, int numOfSongs){
    if(DS == nullptr) return INVALID_INPUT;
    return ((MusicManager*)DS)->AddArtist(artistID,numOfSongs);
}

StatusType RemoveArtist(void *DS, int artistID){
    if(DS == nullptr) return INVALID_INPUT;
    return ((MusicManager*)DS)->RemoveArtist(artistID);
}

StatusType AddToSongCount(void *DS, int artistID, int songID){
    if(DS == nullptr) return INVALID_INPUT;
    return ((MusicManager*)DS)->AddToSongCount(artistID,songID);
}

StatusType NumberOfStreams(void *DS, int artistID, int songID, int *streams){
    if(DS == nullptr) return INVALID_INPUT;
    return ((MusicManager*)DS)->NumberOfStreams(artistID,songID,streams);
}

StatusType GetRecommendedSongs(void *DS, int numOfSongs, int *artists, int *songs);

void Quit(void** DS);
