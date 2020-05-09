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

// Ranked linked list to keep songs by number of listenings
Node<int,Avl<int,Disc>>* bestHitsListStart;
Node<int,Avl<int,Disc>>* bestHitsListFinish;
int totalSongs;

public:
    MusicManager():totalSongs(0){
        this->bestHitsListStart = new Node<int,Avl<int,Disc>>(0, new Avl<int,Disc>);
        this->bestHitsListFinish = this->bestHitsListStart;
    }

    /* Function used by MusicManager destructor to iterate over disc tree and delete all of it's nodes */
   class DiscPredicateDelete{
    public:
        void operator()(Node<int,Disc>* discNode){
            // delete disc
            delete discNode->getData();
        }
        explicit DiscPredicateDelete() = default;
        DiscPredicateDelete(const DiscPredicateDelete& a) = delete;
    };

    /* Function used by MusicManager destructor to iterate over artist tree and delete all of it's nodes */
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

        /* Go through all nodes of ranked linked list  */
        while(current!= nullptr){
            current= current->getNext();

            // get the root of disc tree of the prev node in BestHitList
            Node<int,Disc>* discNode = prev->getData()->getRoot();

            // do postorder to free data (disc) in each node
            DiscPredicateDelete discDelete;

            postorder<int,Disc,DiscPredicateDelete>(discNode,discDelete);

            // Delete node of linked list
            delete prev->getData();
            prev->removeNode();
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

            // if there is rank 0 at BsetHitsList
            if(this->bestHitsListStart->getKey()==0){
                this->bestHitsListStart->getData()->insert(artistID,disc);
                disc->setRankPtr(this->bestHitsListStart);
            }

            // if there isn't rank 0 at BsetHitsList - need to create one and insert it into list
            else{
                Node<int,Avl<int,Disc>>* rankNodeNew = new Node<int,Avl<int,Disc>>(0, new Avl<int,Disc>);
                rankNodeNew->setPrev(nullptr);
                rankNodeNew->setNext(this->bestHitsListStart);
                this->bestHitsListStart->setPrev(rankNodeNew);
                this->bestHitsListStart=rankNodeNew;
                if(this->bestHitsListFinish == nullptr) this->bestHitsListFinish=this->bestHitsListStart;

                // insert disc into disc tree of newly created node in rank list, with ank 0 & update disc rank pointer
                rankNodeNew->getData()->insert(artistID,disc);

                disc->setRankPtr(rankNodeNew);
            }

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

    //Predicate on Song which updates each song DiscPtr to nullptr
    class SongPredicateDestroy{
    public:
        void operator()(Node<int,Song>* songNode){
            // delete artist
            songNode->getData()->setDisc(nullptr);
        }
        explicit SongPredicateDestroy() = default;
        SongPredicateDestroy(const SongPredicateDestroy& a) = delete;
    };



    StatusType RemoveArtist(int artistID){
        if(artistID<=0) return INVALID_INPUT;
        try{
            Artist* artist= this->artistTree.find(artistID)->getData();
            int numOfSongs=artist->getNumOfSongs();
            Node<int,Avl<int,Disc>>* currentNode;
            for(int i=0; i<numOfSongs; i++){

                Disc* disc = artist->getSong(i)->getDisc();
                if(disc== nullptr) continue;

                Node<int,Song>* songNode = disc->getSongTree()->getRoot();
                SongPredicateDestroy songPred;
                postorder<int,Song,SongPredicateDestroy>(songNode,songPred);

                currentNode = disc->getRankPtr();
                delete disc;
                currentNode->getData()->deleteVertice(artistID);

                if(currentNode->getData()->isEmpty()){
                    if(this->bestHitsListStart->getKey() == currentNode->getKey()){
                        this->bestHitsListStart = currentNode->getNext();
                    }
                    if(this->bestHitsListFinish->getKey() == currentNode->getKey()){
                        this->bestHitsListFinish = currentNode->getPrev();
                    }
                    delete currentNode->getData();
                    currentNode->removeNode();
                }


            }
            //Delete from Artist Tree
            this->artistTree.deleteVertice(artistID);
            delete artist;

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

            if(discOld->getSongTree()->isEmpty()){
                delete discOld;
                rankNodeOld->getData()->deleteVertice(artistID);
            }

            artist->addCount(songID);

            int popularity = song->getPopularity();

            if(rankNodeOld->getNext()!= nullptr && rankNodeOld->getNext()->getKey()==popularity){
                try{
                    Disc* disc=rankNodeOld->getNext()->getData()->find(artistID)->getData();
                    disc->addSong(song);
                    song->setDisc(disc);
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
                    this->bestHitsListFinish = rankNodeOld->getNext();
                }

                delete rankNodeOld->getData();
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
            Artist* artist= this->artistTree.find(artistID)->getData();
            if(artist->getNumOfSongs()<=songID) return INVALID_INPUT;
            *streams = artist->getSong(songID)->getPopularity();
            return SUCCESS;
        }
        catch (std::bad_alloc& e) {
            return ALLOCATION_ERROR;
        }
        catch(const Avl<int,Artist>::KeyNotFound& e) {
            return FAILURE;
        }
    }
    //Predicate on Song which inserts each song songID and artistID to the suitable array
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
            else return;
        }
        explicit SongPredicate(int* artistsArray,int artistID,int* songsArray, int* index,int* counter):artistsArray(artistsArray),artistID(artistID), songsArray(songsArray), index(index),counter(counter){};
        SongPredicate(const SongPredicate& a) = delete;
    };

    //Predicate on Disc which do inorder traversal on all the songs of the Disc
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
            else return;
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
