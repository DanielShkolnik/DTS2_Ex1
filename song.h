//
// Created by Eleanor on 27/04/2020.
//

#ifndef DTS2_EX1_SONG_H
#define DTS2_EX1_SONG_H

#include "disc.h"

class Song{
private:
    int songID;
    int artistID;
    int popularity;
    Disc* disc;
public:
    Song(int songID, int artistID):songID(songID),artistID(artistID),popularity(0),disc(nullptr){};
    ~Song() = default;
    Song(const Song& song) = delete;
    Song& operator=(const Song& song) = delete;
    int getSongID(){
        return this->songID;
    }
    int getArtistID(){
        this->artistID;
    }
    void increasePopularity(){
        this->popularity++;
    }

    void setDisc(Disc* disc){
        this->disc=disc;
    }

    Disc* getDisc(){
        return this->disc;
    }

};
#endif //DTS2_EX1_SONG_H
