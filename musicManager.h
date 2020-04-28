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
        this->bestHitsListStart = new Node<int,Avl<int,Disc>>(0, new Avl<int,Disc>);
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

        try {
            // create artist node
            Artist* artist = new Artist(artistID,numOfSongs);

            // insert into tree
            this->artistTree.insert(artistID, artist);

            // create new disc
            Disc* disc = new Disc(artistID);

            // update artists song
            for(int i=0;i<artist->getNumOfSongs();i++){
                disc->addSong((long*)(artist->getSongNode(i)->getData())); //Need to fix
                artist->getSongNode(i)->getData()->setDisc(disc);
            }

            // insert disc into disc tree at rank 0 in bestHitsList
            this->bestHitsListStart->getData()->insert(artistID,disc);

            // update ptr to rank
            disc->updateRank(this->bestHitsListStart);
        } catch (const Artist::ALLOCATION_ERROR&) {
            throw static_cast<StatusType>(-2);
        } catch (const Artist::INVALID_INPUT&) {
            throw static_cast<StatusType>(-3);
        }  catch(const Avl<int,Artist>::KeyExists&) {
            throw static_cast<StatusType>(-1);
        }
        return static_cast<StatusType>(0);
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
            if(songID >= artist->getNumOfSongs()) return INVALID_INPUT;
            Song* song = artist->getSongNode(songID)->getData();
            Disc* discOld = song->getDisc();
            Node<int,Avl<int,Disc>>* rankNodeOld = discOld->getRankPtr();

            discOld->removeSong(songID);

            if(discOld->getSongTree()->isEmpty()) rankNodeOld->getData()->deleteVertice(artistID);

            if(rankNodeOld->getData()->isEmpty()){
                //check bestHitsListStart
                rankNodeOld->removeNode();
            }

            artist->addCount(songID);

            int popularity = song->getPopularity();

            if(rankNodeOld->getNext()!= nullptr && rankNode->getNext()->getKey()==popularity){
                try{
                    rankNode->getNext()->getData()->find(artistID)->getData()->addSong();
                }
            }
            artist->getSongNode(songID)->getData()->getDisc()->getRankPtr()->getNext()->getKey();
        }
        catch(std::bad_alloc& e) {
            return ALLOCATION_ERROR;
        }
        catch(Avl<int,Artist>::KeyNotFound& e){
            return FAILURE;
        }
    }

    StatusType NumberOfStreams(int artistID, int songID, int *streams){
        try {
            *streams = this->artistTree.find(artistID)->getData()->getSongNode(songID)->getData()->getPopularity();
        } catch (const Artist::ALLOCATION_ERROR&) {
            throw static_cast<StatusType>(-2);
        } catch (const Artist::INVALID_INPUT&) {
            throw static_cast<StatusType>(-3);
        }  catch(const Avl<int,Artist>::KeyExists&) {
            throw static_cast<StatusType>(-1);
        }
    }

    void discInorder(Node<int,Disc>* disc,int* artists, int artistIndex, int* songs){

            // add to artists array
            *(artists+artistIndex) = disc->getData()->getArtistID();

            // for each node in tree do inorder traverse on song tree + add to song array

    }

    StatusType GetRecommendedSongs(int numOfSongs, int *artists, int *songs){

        int i = 0, counter = 0;
        // set iterator to end of list & init counter
        Node<int,Avl<int,Disc>>* iter = this->bestHitsListFinish;

        // loop over node of list
        for (iter; iter != nullptr && counter>=0; iter = iter->getNext(), i++, counter--){
            // for each node do inorder traverse on disc tree
            Node<int,Disc>* discIter = iter->getData()->getRoot();

            inorder<int,Avl<int,Disc>,void(Avl<int,Disc>*,int*,int*)>(discRoot->getRoot(),discInorder(discRoot,artists,songs));

        }


        // finish when counter equal to zero
    }

};


#endif //DTS2_EX1_MUSICMANAGER_H
