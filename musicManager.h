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
#include <iostream>

class MusicManager{
private:
Avl<int,Artist> artistTree;
Node<int,Avl<int,Disc>>* bestHitsListStart;
Node<int,Avl<int,Disc>>* bestHitsListFinish;
int totalSongs;

public:
    MusicManager():totalSongs(0){
        this->bestHitsListStart = new Node<int,Avl<int,Disc>>(0, new Avl<int,Disc>);
        this->bestHitsListFinish = this->bestHitsListStart;
    }

   class DiscPredicateDelete{
    public:
        void operator()(Node<int,Disc>* discNode){
            // delete disc
            delete discNode->getData();
        }
        explicit DiscPredicateDelete() = default;
        DiscPredicateDelete(const DiscPredicateDelete& a) = delete;
    };

    class ArtistPredicate{
    public:
        void operator()(Node<int,Artist>* artistNode){
            // delete artist
            delete artistNode->getData();
        }
        explicit ArtistPredicate() = default;
        ArtistPredicate(const ArtistPredicate& a) = delete;
    };

    ~MusicManager(){
        Node<int,Avl<int,Disc>>* current = this->bestHitsListStart;
        Node<int,Avl<int,Disc>>* prev = current;
        while(current!= nullptr){
            current= current->getNext();

            // get the root of disc tree of the prev node in BestHitList
            Node<int,Disc>* discNode = prev->getData()->getRoot();

            // do inorder to free data in each node
            DiscPredicateDelete discDelete;

            postorder<int,Disc,DiscPredicateDelete>(discNode,discDelete);

            delete prev->getData();
            delete prev;
            prev = current;
        }

        // get artist tree root
        Node<int,Artist>* artistNode = this->artistTree.getRoot();
        // create inst of artist destructor
        ArtistPredicate artistPred;

        postorder<int,Artist,ArtistPredicate>(artistNode,artistPred);
    }
    MusicManager(const MusicManager& musicManager) = delete;
    MusicManager& operator=(const MusicManager& musicManager) = delete;


    StatusType AddArtist(int artistID, int numOfSongs){
        if(artistID<=0 || numOfSongs<=0){
            return INVALID_INPUT;
        }
        Artist* artist;
        try {

            // create artist node
            artist = new Artist(artistID,numOfSongs);

            // insert into tree
            this->artistTree.insert(artistID, artist);

            // create new disc
            Disc* disc = new Disc(artistID);

            // update artists song
            for(int i=0;i<artist->getNumOfSongs();i++){
                disc->addSong((artist->getSong(i))); //Need to fix
                artist->getSong(i)->setDisc(disc);
            }

            // insert disc into disc tree at rank 0 in bestHitsList
            this->bestHitsListStart->getData()->insert(artistID,disc);

            // update ptr to rank
            disc->setRankPtr(this->bestHitsListStart);
            this->totalSongs+=numOfSongs;
            return SUCCESS;
        }
        catch (std::bad_alloc& e) {
            return ALLOCATION_ERROR;
        }
        catch(const Avl<int,Artist>::KeyExists& e) {
            delete artist;
            return FAILURE;
        }
    }

    StatusType RemoveArtist(int artistID){
        if(artistID<=0) return INVALID_INPUT;
        try{
            Artist* artist= this->artistTree.find(artistID)->getData();
            int numOfSongs=artist->getNumOfSongs();
            Node<int,Avl<int,Disc>>* current = this->bestHitsListStart;
            //Delete from Artist Tree
            this->artistTree.deleteVertice(artistID);
            delete artist;
            while(current!= nullptr){
                try{
                    Disc* disc = current->getData()->find(artistID)->getData();
                    delete disc;
                    current->getData()->deleteVertice(artistID);
                }
                catch(Avl<int,Disc>::KeyNotFound& e){
                    continue;
                }

                if(current->getData()->isEmpty()){
                    current->removeNode();
                }
                current=current->getNext();
            }
            this->totalSongs-=numOfSongs;
            return SUCCESS;
        }
        catch(std::bad_alloc&) {
            return ALLOCATION_ERROR;
        }
        catch(Avl<int,Artist>::KeyNotFound&){
            return FAILURE;
        }
    }

    StatusType AddToSongCount(int artistID, int songID){
        if(artistID<=0 || songID<0) return INVALID_INPUT;
        try{
            Artist* artist=this->artistTree.find(artistID)->getData();
            if(songID >= artist->getNumOfSongs()) return INVALID_INPUT;
            Song* song = artist->getSong(songID);
            Disc* discOld = song->getDisc();
            Node<int,Avl<int,Disc>>* rankNodeOld = discOld->getRankPtr();

            discOld->removeSong(songID);

            if(discOld->getSongTree()->isEmpty()) rankNodeOld->getData()->deleteVertice(artistID);

            artist->addCount(songID);

            int popularity = song->getPopularity();

            if(rankNodeOld->getNext()!= nullptr && rankNodeOld->getNext()->getKey()==popularity){
                try{
                    rankNodeOld->getNext()->getData()->find(artistID)->getData()->addSong(song);
                }
                catch(Avl<int,Disc>::KeyNotFound& e){
                    Disc* discNew = new Disc(artistID);
                    rankNodeOld->getNext()->getData()->insert(artistID,discNew);
                    discNew->setRankPtr(rankNodeOld->getNext());
                    discNew->addSong(song);
                    song->setDisc(discNew);
                }
            }
            else{
                Node<int,Avl<int,Disc>>* rankNodeNew = new Node<int,Avl<int,Disc>>(popularity, new Avl<int,Disc>);
                rankNodeNew->setPrev(rankNodeOld);
                rankNodeNew->setNext(rankNodeOld->getNext());
                if(rankNodeOld->getNext()!= nullptr) rankNodeOld->getNext()->setPrev(rankNodeNew);
                rankNodeOld->setNext(rankNodeNew);
                Disc* discNew = new Disc(artistID);
                rankNodeNew->getData()->insert(artistID,discNew);
                discNew->setRankPtr(rankNodeNew);
                discNew->addSong(song);
                song->setDisc(discNew);
            }

            if(rankNodeOld->getData()->isEmpty()){
                if(this->bestHitsListStart->getKey() == rankNodeOld->getKey()){
                    this->bestHitsListStart = rankNodeOld->getNext();
                }
                if(this->bestHitsListFinish->getKey() == rankNodeOld->getKey()){
                    this->bestHitsListFinish=rankNodeOld->getPrev();
                }
                rankNodeOld->removeNode();
            }

            if(this->bestHitsListFinish->getNext()!= nullptr){
                this->bestHitsListFinish=this->bestHitsListFinish->getNext();

            }

            return SUCCESS;
        }
        catch(std::bad_alloc&) {
            return ALLOCATION_ERROR;
        }
        catch(Avl<int,Artist>::KeyNotFound&){
            return FAILURE;
        }
    }

    StatusType NumberOfStreams(int artistID, int songID, int *streams){
        if(artistID<=0 || songID<0) return INVALID_INPUT;
        try {
            *streams = this->artistTree.find(artistID)->getData()->getSong(songID)->getPopularity();
            return SUCCESS;
        }
        catch (std::bad_alloc& e) {
            return ALLOCATION_ERROR;
        }
        catch(const Avl<int,Artist>::KeyExists& e) {
            return FAILURE;
        }
    }

    class SongPredicate{
    private:
        int* artistsArray;
        int artistID;
        int* songsArray;
        int* index;
        int* counter;
    public:
        void operator()(Node<int,Song>* songNode){

            // finish when counter equal to zero
            if(*counter > 0) {

                // add to artists array
                artistsArray[*index] = artistID;

                // add to songs list
                songsArray[*index] = songNode->getData()->getSongID();

                // update index
                (*index) = (*index) + 1;
                (*counter) = (*counter) - 1;
            }
        }
        explicit SongPredicate(int* artistsArray,int artistID,int* songsArray, int* index,int* counter):artistsArray(artistsArray),artistID(artistID), songsArray(songsArray), index(index),counter(counter){};
        SongPredicate(const SongPredicate& a) = delete;
    };

    class DiscPredicate{
    private:
        int* artistsArray;
        int* counter;
        int* index;
        int* songsArray;
    public:
        // for each node in disc tree do inorder traverse on song tree + add song to song array & artist to artist array
        void operator()(Node<int,Disc>* disc){
            if(*counter > 0){
            // get songTree root
            Node<int,Song>* song = (disc->getData()->getSongTree()->getRoot());


            // create inst of predicate for song
            SongPredicate songPred(artistsArray,song->getData()->getArtistID(),songsArray,index,counter);

            // traverse song tree
            inorder<int,Song,SongPredicate>(song, songPred);
            }
        }
        explicit DiscPredicate(int* artistsArray,int* counter,int* index,int* songsArray):artistsArray(artistsArray),counter(counter),index(index),songsArray(songsArray){};
        DiscPredicate(const DiscPredicate& a) = delete;
    };

    StatusType GetRecommendedSongs(int numOfSongs, int *artists, int *songs){
        if(numOfSongs <= 0) return INVALID_INPUT;
        if(numOfSongs > this->totalSongs) return FAILURE;
        try {
            int i = 0, j=numOfSongs;
            int *counter = &j;
            int *index = &i;
            // set iterator to end of list & init counter
            Node<int, Avl<int, Disc>> *iter = this->bestHitsListFinish;

            // loop over nodes of list
            for (; (iter != nullptr && *counter > 0); iter = iter->getPrev()) {

                // get disc root
                Node<int, Disc> *discIter = iter->getData()->getRoot();

                // create inst of predicate for disc
                DiscPredicate discPred(artists, counter, index, songs);

                // traverse tree for current rank
                inorder<int, Disc, DiscPredicate>(discIter, discPred);
            }
            return SUCCESS;
        }
        catch (Artist::INVALID_INPUT& e) {
            return INVALID_INPUT;
        }
        catch(std::bad_alloc&) {
            return ALLOCATION_ERROR;
        }
        catch(Avl<int,Artist>::KeyExists& e) {
            return FAILURE;
        }
    }

};


#endif //DTS2_EX1_MUSICMANAGER_H
