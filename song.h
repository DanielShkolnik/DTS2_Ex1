//
// Created by Eleanor on 27/04/2020.
//

#ifndef DTS2_EX1_SONG_H
#define DTS2_EX1_SONG_H

class Song{
private:
    int songID;
    int artistID;
    int popularity;
public:
    Song(int songID, int artistID):songID(songID),artistID(artistID),popularity(0){};
    ~Song() = default;
    Song(const Song& avl) = delete;
    Song& operator=(const Song& avl) = delete;
    int getSongID(){
        return this->songID;
    }
    int getArtistID(){
        this->artistID;
    }
    void increasePopularity(){
        this->popularity++;
    }
};
#endif //DTS2_EX1_SONG_H
