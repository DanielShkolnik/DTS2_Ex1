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
#include "exception"



class MusicManager{
private:
Avl<int,Artist> artistTree;
Node<int,Avl<int,Disc>>* bestHitsListStart;
Node<int,Avl<int,Disc>>* bestHitsListFinish;

public:
    MusicManager(){
        this->bestHitsListStart = new Node<int,Avl<int,Disc>>;
        this->bestHitsListFinish = this->bestHitsListStart;
    }
    ~MusicManager(){
        Node<int,Avl<int,Disc>>* current = this->bestHitsListStart;
        Node<int,Avl<int,Disc>>* prev = current;
        while(current!= nullptr){
            current= current->getNext();
            delete prev;
            prev = current;
        }
    }
    MusicManager(const MusicManager& musicManager) = delete;
    MusicManager& operator=(const MusicManager& musicManager) = delete;


    StatusType AddArtist(int artistID, int numOfSongs){

    }

    StatusType RemoveArtist(int artistID){
        if(artistID<=0) return INVALID_INPUT;
        try{
            Artist* artist= this->artistTree.find(artistID)->getData();
            Node<int,Avl<int,Disc>>* current = this->bestHitsListStart;
            while(current!= nullptr){
                try{
                    current->getData()->deleteVertice(artistID);
                }
                catch(Avl<int,Disc>::KeyNotFound& e){
                    continue;
                }
                if(current->getData()->isEmpty()){
                    if(current->getPrev()!= nullptr) current->getPrev()->setNext(current->getNext());
                    if(current->getNext()!= nullptr) current->getNext()->setPrev(current->getPrev());
                }
            }
            this->artistTree.deleteVertice(artistID);
            return SUCCESS;
        }
        catch(std::bad_alloc& e) {
            return ALLOCATION_ERROR;
        }
        catch(Avl<int,Artist>::KeyNotFound& e){
            return FAILURE;
        }
    }

    StatusType AddToSongCount(int artistID, int songID){
        if(artistID<=0 || songID<=0) return INVALID_INPUT;
        try{
            Artist* artist=this->artistTree.find(artistID)->getData();
            if(songID>=artist->getNumOfSongs()) return INVALID_INPUT;
            artist->addCount(songID);
        }
        catch(std::bad_alloc& e) {
            return ALLOCATION_ERROR;
        }
        catch(Avl<int,Artist>::KeyNotFound& e){
            return FAILURE;
        }
    }

    StatusType NumberOfStreams(int artistID, int songID, int *streams){

    }

    StatusType GetRecommendedSongs(int numOfSongs, int *artists, int *songs){

    }



};


#endif //DTS2_EX1_MUSICMANAGER_H
