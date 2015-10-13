// DracView.c ... DracView ADT implementation

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "Globals.h"
#include "Game.h"
#include "GameView.h"
#include "DracView.h"
#include "Places.h"
#include "Queue.h"
#include "Map.h" //... if you decide to use the Map ADT

#define NUM_MAP_LOCATION 100
     
struct dracView {
    GameView g;
    int numTraps[NUM_MAP_LOCATION];
    int numImVampires[NUM_MAP_LOCATION];
};
     

// Creates a new DracView to summarise the current state of the game
DracView newDracView(char *pastPlays, PlayerMessage messages[])
{
    //initialising values of the struct
    DracView dracView = malloc(sizeof(struct dracView));
    dracView->g = newGameView(pastPlays, messages);
    int i = 0;
    while (i < NUM_MAP_LOCATION){
        dracView->numTraps[i] = 0;
        dracView->numImVampires[i] = 0;
        i++;
    }
    //traverses through dracula's actions and adds counters for traps and vampires
    //in the location where he placed them
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
    return whereCanTheyGo(currentView,
                               numLocations,
                               getCurrentPlayer(currentView->g),
                               road, FALSE, sea);
}

// What are the specified player's next possible moves
LocationID *whereCanTheyGo(DracView currentView, int *numLocations,
                           PlayerID player, int road, int rail, int sea)
{
    int i, numValidLocations, index;
    LocationID forbidden;
    LocationID *validLocations;

    LocationID *locations = connectedLocations(currentView->g,
                               numLocations,
                               getLocation(currentView->g, player),
                               player,
                               getRound(currentView->g),
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

int doubleBackInTrail(DracView g, LocationID *trail){
    int i;
    for(i=0;i<TRAIL_SIZE;i++){
        if (trail[i]==DOUBLE_BACK_1||
            trail[i]==DOUBLE_BACK_2||
            trail[i]==DOUBLE_BACK_3||
            trail[i]==DOUBLE_BACK_4||
            trail[i]==DOUBLE_BACK_5){
            return TRUE;
        }
    }
    return FALSE;
}

int hideInTrail(DracView g, LocationID *trail){
    int i;
    for(i=0;i<TRAIL_SIZE;i++){
        if (trail[i]==HIDE){
            return TRUE;
        }
    }
    return FALSE;
}

int findPath(DracView d, LocationID src, LocationID dest, int *path, int road, int sea)
{
	printf("finding path from %d to %d\n",src,dest);
	if(src==dest) {
		printf("nice try\n");
		path[1] = dest;
		return 1;
	}
	int tmp_city = src;
	// Temporary store of path_distance for calculations
	int tmp_distance = 0;
	int path_distance = 0;

	// Array of visited cities, if not visited 0, else 1
	int visited[NUM_MAP_LOCATIONS] = {0};

	// Stores index of the previous city, default value -1
	int prev[NUM_MAP_LOCATIONS] = {[0 ... (NUM_MAP_LOCATIONS-1)] = -1};

	Queue cityQ = newQueue();
	QueueJoin(cityQ, src);

	// While Queue is not empty and the tmp_city is not the destination city (e.g. when path to destination city from src is found)
	while (QueueIsEmpty(cityQ) == 0 && tmp_city != dest) {
		tmp_city = QueueLeave(cityQ);
		
		int num_locs;
		int *locs = connectedLocations(d->g, &num_locs,tmp_city, PLAYER_DRACULA, giveMeTheRound(d),road,FALSE,sea);
		
		int i;
		for (i=0;i<num_locs;i++) {
			
		
			if (!visited[locs[i]]) {
				QueueJoin(cityQ, locs[i]);
				prev[locs[i]] = tmp_city;
				visited[locs[i]] = 1;
			}
		}

		if (tmp_city == dest) {
			prev[locs[i]] = tmp_city;

			// Calculating size of path
			int index = locs[i];
			while (index != src) {
				index = prev[index];
				path_distance++;
			}
		
			// Building path array, storing destination first
			tmp_distance = path_distance-1;
			path[tmp_distance] = dest;
			tmp_distance--;

			// Storing rest of array
			index = prev[dest];
			while (tmp_distance >= 0) {
				path[tmp_distance] = index;
				index = prev[index];
				tmp_distance--;
			}
			break;
		}
	}
	
	
	printf("path->");
	int j;
	for(j=0;j<path_distance;j++) {
		printf("%d->",path[j]);
	}
	printf("x\n");
	
	return path_distance;
}


