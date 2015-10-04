// HunterView.c ... HunterView ADT implementation

#include <stdlib.h>
#include <assert.h>
#include "Globals.h"
#include "Game.h"
#include "GameView.h"
#include "HunterView.h"
#include <string.h>
// #include "Map.h" ... if you decide to use the Map ADT
     
struct hunterView {
    GameView view;
    PlayerMessage *messages;
};

// Creates a new HunterView to summarise the current state of the game
HunterView newHunterView(char *pastPlays, PlayerMessage messages[])
{
    HunterView hunterView = malloc(sizeof(struct hunterView));
    assert(hunterView!=NULL);
    hunterView->view = newGameView(pastPlays,messages);

    int rounds = giveMeTheRound(hunterView);
    hunterView->messages = malloc(sizeof(PlayerMessage)*rounds);
    int i;
    for (i=0;i<rounds;i++) {
        strncpy(hunterView->messages[i], messages[i], MESSAGE_SIZE);
    }
    return hunterView;
}
     
     
// Frees all memory previously allocated for the HunterView toBeDeleted
void disposeHunterView(HunterView toBeDeleted)
{
    disposeGameView(toBeDeleted->view);
    free(toBeDeleted->messages);
    free(toBeDeleted);
}


//// Functions to return simple information about the current state of the game

// Get the current round
int giveMeTheRound(HunterView currentView)
{
    return getRound(currentView->view);
}

// Get the id of current player
PlayerID whoAmI(HunterView currentView)
{
    return getCurrentPlayer(currentView->view);
}

// Get the current score
int giveMeTheScore(HunterView currentView)
{
    return getScore(currentView->view);
}

// Get the current health points for a given player
int howHealthyIs(HunterView currentView, PlayerID player)
{
    return getHealth(currentView->view, player);
}

// Get the current location id of a given player
LocationID whereIs(HunterView currentView, PlayerID player)
{
    return getLocation(currentView->view, player);
}

//// Functions that return information about the history of the game

// Fills the trail array with the location ids of the last 6 turns
void giveMeTheTrail(HunterView currentView, PlayerID player,
                            LocationID trail[TRAIL_SIZE])
{
    return getHistory(currentView->view, player, trail);
}

//// Functions that query the map to find information about connectivity

// What are my possible next moves (locations)
LocationID *whereCanIgo(HunterView currentView, int *numLocations, int road, int rail, int sea)
{
    return whereCanTheyGo(currentView,
                               numLocations,
                               getCurrentPlayer(currentView->game),
                               road, rail, sea);
}

// What are the specified player's next possible moves
LocationID *whereCanTheyGo(HunterView currentView, int *numLocations,
                           PlayerID player, int road, int rail, int sea)
{
    int i, numValidLocations, index;
    LocationID forbidden;
    LocationID *validLocations;

    LocationID *locations = connectedLocations(currentView->game,
                               numLocations,
                               getLocation(currentView->game, player),
                               player,
                               getRound(currentView->game),
                               road, rail, sea);
    if(player == PLAYER_DRACULA){
        forbidden = ST_JOSEPH_AND_ST_MARYS;
    }

    numValidLocations = 0;
    for(i = 0; i < (*numLocations); i++){
        if(locations[i] != forbidden){
            numValidLocations++;
        }
    }

    index = 0;
    validLocations = malloc(sizeof(LocationID) * numValidLocations);
    for(i = 0; i < numValidLocations; i++){
        if(locations[i] != forbidden){
            validLocations[index] = locations[i];
            index++;
        }
    }

    free(locations);
    *numLocations = numValidLocations;
    return validLocations;
}
