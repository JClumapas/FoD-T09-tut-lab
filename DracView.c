// DracView.c ... DracView ADT implementation

#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "Globals.h"
#include "Game.h"
#include "GameView.h"
#include "DracView.h"
#include "Map.h" //... if you decide to use the Map ADT

#define NUM_MAP_LOCATION 100
     
struct dracView {
    GameView g;
    int numTraps[NUM_MAP_LOCATION];
    int numImVampires[NUM_MAP_LOCATION];
    int hello;
};
     

// Creates a new DracView to summarise the current state of the game
DracView newDracView(char *pastPlays, PlayerMessage messages[])
{
    //REPLACE THIS WITH YOUR OWN IMPLEMENTATION
    DracView dracView = malloc(sizeof(struct dracView));
    dracView->g = newGameView(pastPlays, messages);
    dracView->hello = 42;
    int i = 0;
    while (i < NUM_MAP_LOCATION){
        dracView->numTraps[i] = 0;
        dracView->numImVampires[i] = 0;
        i++;
    }
    int curr = 32;
    LocationID location = 0;
    char currLocation[3] = {'\0'};
    int pastPlaySize = strlen(pastPlays);
    while (curr < pastPlaySize){
        currLocation[0] = pastPlays[curr+1];
        currLocation[1] = pastPlays[curr+2];
        location = abbrevToID(currLocation);
        if (pastPlays[curr+3] == 'T'){
            dracView->numTraps[location]++;
        }
        if (pastPlays[curr+4] == 'V'){
            dracView->numImVampires[location]++;
        }
        curr += 40;
    }
    return dracView;
}
     
     
// Frees all memory previously allocated for the DracView toBeDeleted
void disposeDracView(DracView toBeDeleted)
{
    //COMPLETE THIS IMPLEMENTATION
    disposeGameView(toBeDeleted->g);
    free( toBeDeleted );
}


//// Functions to return simple information about the current state of the game

// Get the current round
Round giveMeTheRound(DracView currentView)
{
    return getRound(currentView->g);
}

// Get the current score
int giveMeTheScore(DracView currentView)
{
    return getScore(currentView->g);
}

// Get the current health points for a given player
int howHealthyIs(DracView currentView, PlayerID player)
{
    return getHealth(currentView->g, player);
}

// Get the current location id of a given player
LocationID whereIs(DracView currentView, PlayerID player)
{
    //REPLACE THIS WITH YOUR OWN IMPLEMENTATION
    //not yet correct
    //This should be correct if the pastPlays string given has exact locations not hie or double back etc
    //else this needs to be taken care of
    return getLocation(currentView->g, player);
}

// Get the most recent move of a given player
void lastMove(DracView currentView, PlayerID player,
                 LocationID *start, LocationID *end)
{
    LocationID trail[TRAIL_SIZE];
    getHistory(currentView->g, player, trail);
    *start = trail[1];
    *end = trail[0];
}

// Find out what minions are placed at the specified location
void whatsThere(DracView currentView, LocationID where,
                         int *numTraps, int *numVamps)
{
    *numTraps = currentView->numTraps[where];
    *numVamps = currentView->numImVampires[where];
    return;
}

//// Functions that return information about the history of the game

// Fills the trail array with the location ids of the last 6 turns
void giveMeTheTrail(DracView currentView, PlayerID player,
                            LocationID trail[TRAIL_SIZE])
{
    getHistory(currentView->g, player, trail);
}

//// Functions that query the map to find information about connectivity

// What are my (Dracula's) possible next moves (locations)
LocationID *whereCanIgo(DracView currentView, int *numLocations, int road, int sea)
{
    /*assert(g != NULL);
    int numPath = 0; //number of paths
    int seaID = -1;
    int start = whereIs(currentView, DRAC)

    VList n = g->connections[start];
    while (n != NULL) {
        if(idToType(n->v) == SEA)
            seaID = n->v;
        if(n->v == end){
            if((sea && n->type == SEA) || (road && n->type == ROAD))
               type[i] = n->type; 
            numPath++; 
        }
        n = n->next;
      }

    if(seaID != -1 && sea){
         n = g->connections[end];
         while( n != NULL){
             if(n->v == seaID){
                 type[i] = BOAT;
                 numPath++;            
             }
             n = n->next;
         }
    }*/
    return NULL;
}

// What are the specified player's next possible moves
LocationID *whereCanTheyGo(DracView currentView, int *numLocations,
                           PlayerID player, int road, int rail, int sea)
{
    //REPLACE THIS WITH YOUR OWN IMPLEMENTATION
    return NULL;
}
