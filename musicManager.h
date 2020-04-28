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
            disc->setRankPtr(this->bestHitsListStart);
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

            artist->addCount(songID);

            int popularity = song->getPopularity();

            if(rankNodeOld->getNext()!= nullptr && rankNodeOld->getNext()->getKey()==popularity){
                try{
                    rankNodeOld->getNext()->getData()->find(artistID)->getData()->addSong((long*)song);
                }
                catch(Avl<int,Disc>::KeyNotFound& e){
                    Disc* discNew = new Disc(artistID);
                    rankNodeOld->getNext()->getData()->insert(artistID,discNew);
                    discNew->setRankPtr(rankNodeOld->getNext());
                    discNew->addSong((long*)song);
                }
            }
            else{
                Node<int,Avl<int,Disc>>* rankNodeNew = new Node<int,Avl<int,Disc>>(popularity, new Avl<int,Disc>);
                rankNodeNew->setPrev(rankNodeOld);
                rankNodeNew->setNext(rankNodeOld->getNext());
                rankNodeOld->setNext(rankNodeNew);
                Disc* discNew = new Disc(artistID);
                rankNodeNew->getData()->insert(artistID,discNew);
                discNew->setRankPtr(rankNodeNew);
                discNew->addSong((long*)song);
            }

            if(rankNodeOld->getData()->isEmpty()){
                if(this->bestHitsListStart->getKey() == rankNodeOld->getKey()){
                    this->bestHitsListStart = rankNodeOld->getNext();
                }
                rankNodeOld->removeNode();
            }

            if(this->bestHitsListFinish->getNext()!= nullptr) this->bestHitsListFinish=this->bestHitsListFinish->getNext();

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


    void songInorder(Node<int,Song>* songNode, int* songs, int* songsIndex,int* counter){

        if(counter == 0){
            return;
        }

        // add to songs list
        *(songs+*songsIndex) = songNode->getData()->getSongID();

        // update index
        *songsIndex++;
        *counter--;
    }
    void discInorder(Node<int,Disc>* disc,int* artists,int* counter, int* artistIndex, int* songs){




    }

    class discPredicate{
    private:
        int* artists;
        int* counter;
        int* artistIndex;
        int* songs;
    public:
        void operator()(Node<int,Disc>* disc){
            // add to artists array
            *(artists+*artistIndex) = disc->getData()->getArtistID();

            // for each node in tree do inorder traverse on song tree + add to song array
            // get songTree root
            Node<int,Song>* song = (Node<int,Song>*)(disc->getData()->getSongTree()->getRoot());

            // init indices
            int i = 0; int* songIndex = &i;
            inorder<int,Node<int,Song>,void(Node<int,Song>* songNode, int* songs, int* songsIndex,int* counter)>(song, songInorder);

            // update indices
            *artists++;
        }
        explicit discPredicate(Node<int,Disc>*, int* artists,int* counter,int* artistIndex,int* songs):artists(artists),counter(counter),artistIndex(artistIndex),songs(songs){};
        discPredicate(const discPredicate& a) = delete;
    };

    StatusType GetRecommendedSongs(int numOfSongs, int *artists, int *songs){

        int i = 0; int* counter = &numOfSongs; int* artistIndex = &i;
        // set iterator to end of list & init counter
        Node<int,Avl<int,Disc>>* iter = this->bestHitsListFinish;

        // loop over node of list
        for (iter; iter != nullptr && *counter > 0; iter = iter->getNext()){

            // for each node do inorder traverse on disc tree
            Node<int,Disc>* discIter = iter->getData()->getRoot();

            discPredicate discPred(discIter,artists,counter,artistIndex,songs);

            inorder<int,Node<int,Disc>*,discPredicate>(discIter,discPred);

        }


        // finish when counter equal to zero
    }

};


#endif //DTS2_EX1_MUSICMANAGER_H
