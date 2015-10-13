// HunterView.c ... HunterView ADT implementation

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "Globals.h"
#include "Game.h"
#include "GameView.h"
#include "HunterView.h"
#include "Queue.h"
#include "Queue.c"
#include <string.h>
// #include "Map.h" ... if you decide to use the Map ADT
     
struct hunterView {
    GameView g;
    PlayerMessage *messages;
};

// Creates a new HunterView to summarise the current state of the game
HunterView newHunterView(char *pastPlays, PlayerMessage messages[])
{
    HunterView hunterView = malloc(sizeof(struct hunterView));
    assert(hunterView!=NULL);
    hunterView->g = newGameView(pastPlays,messages);

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
    disposeGameView(toBeDeleted->g);
    free(toBeDeleted->messages);
    free(toBeDeleted);
}


//// Functions to return simple information about the current state of the game

// Get the current round
int giveMeTheRound(HunterView currentView)
{
    return getRound(currentView->g);
}

// Get the id of current player
PlayerID whoAmI(HunterView currentView)
{
    return getCurrentPlayer(currentView->g);
}

// Get the current score
int giveMeTheScore(HunterView currentView)
{
    return getScore(currentView->g);
}

// Get the current health points for a given player
int howHealthyIs(HunterView currentView, PlayerID player)
{
    return getHealth(currentView->g, player);
}

// Get the current location id of a given player
LocationID whereIs(HunterView currentView, PlayerID player)
{
    return getLocation(currentView->g, player);
}

//// Functions that return information about the history of the game

// Fills the trail array with the location ids of the last 6 turns
void giveMeTheTrail(HunterView currentView, PlayerID player,
                            LocationID trail[TRAIL_SIZE])
{
    return getHistory(currentView->g, player, trail);
}

//// Functions that query the map to find information about connectivity

// What are my possible next moves (locations)
LocationID *whereCanIgo(HunterView currentView, int *numLocations, int road, int rail, int sea)
{
    return whereCanTheyGo(currentView,
                               numLocations,
                               getCurrentPlayer(currentView->g),
                               road, rail, sea);
}

// What are the specified player's next possible moves
LocationID *whereCanTheyGo(HunterView currentView, int *numLocations,
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

// find a path between two vertices using breadth-first traversal
int findPath(HunterView h, LocationID src, LocationID dest, int *path, int road, int rail, int sea)
{
	printf("finding path from %d to %d\n",src,dest);
	if(src==dest) {
		printf("trying to get to where you are\n");
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
		int *locs = connectedLocations(h->g, &num_locs,tmp_city, whoAmI(h), giveMeTheRound(h),road,rail,sea);
		
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
