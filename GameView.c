// GameView.c ... GameView ADT implementation

#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "Globals.h"
#include "Game.h"
#include "GameView.h"
// #include "Map.h" ... if you decide to use the Map ADT
     
#define NUM_HUNTERS 4

struct gameView {
    int hp[NUM_PLAYERS];
    int trail[NUM_PLAYERS][TRAIL_SIZE];
    int score;
    char *pastPlays; // past plays 
};
     

// Creates a new GameView to summarise the current state of the game
GameView newGameView(char *pastPlays, PlayerMessage messages[])
{
    //initializing gameView variables
    GameView gameView = malloc(sizeof(struct gameView));
    gameView->score = GAME_START_SCORE;
    gameView->pastPlays = pastPlays;
    int i = 0;
    int j = 0;
    while (i<NUM_PLAYERS){
        while(j<TRAIL_SIZE){
            gameView->trail[i][j] = UNKNOWN_LOCATION;
            j++;
        }
        i++;
    }
    i = 0;
    while (i < NUM_HUNTERS){
        gameView->hp[i] = GAME_START_HUNTER_LIFE_POINTS;
        i++;
    }
    gameView->hp[PLAYER_DRACULA] = GAME_START_BLOOD_POINTS;

    //adjust to current state
    //here we want to analyse pastPlays string and adjust 
    //the values as we read information from the string
    if(pastPlays == NULL){
        return gameView;
    }
    //the following can probably be cleaned up
    int count = 3;
    int curr = 0;
    int currPlayerID = 0;
    int Location = 0;
    char currPlayer[1];
    char currLocation[2];
    int pastPlaySize = strlen(pastPlays);
    int numTurns = (pastPlaySize/7)/5; //fix maths for this to get how many times the player has taken a trun
    while (curr < pastPlaySize){
        //the following gets the current player and adds locations to their respective trail array
        count = 3;
        currPlayer[0] = pastPlays[curr];
        currPlayerID = letterToPlayerID(currPlayer);
        currLocation[0] = pastPlays[curr+1];
        currLocation[1] = pastPlays[curr+2];
        gameView->trail[currPlayerID][numTurns];
        //clean this if i havent done so before its due

        //the following reads the characters after the first 3 to adjust the scores/hp
        if (currPlayerID != PLAYER_DRACULA){
            while (count < 7){
                if (pastPlays[curr+count] == 'T'){
                    //apply trap consequences
                }else if (pastPlays[curr+count] == 'V'){
                    //apply consequences for meeting immature vampire
                }else if (pastPlays[curr+count] == 'D'){
                    //apply consequences for encountering dracula
                }else{
                    count = 7;
                    continue;
                }
                count++;
            }
        }else{
            if (pastPlays[curr+3] == 'T'){
                //apply trap by dracula
            }
            if (pastPlays[curr+4] == 'V'){
                //apply immature vampire was placed
            }
            if (pastPlays[curr+5] == 'M'){
                //apply trap has worn out
            }else if (pastPlays[curr+5] == 'V'){
                //apply immature vampire has matured
            }
        }
    }

    return gameView;
}
     
     
// Frees all memory previously allocated for the GameView toBeDeleted
void disposeGameView(GameView toBeDeleted)
{
    //COMPLETE THIS IMPLEMENTATION
    free( toBeDeleted );
}


//// Functions to return simple information about the current state of the game

// Get the current round
Round getRound(GameView currentView)
{
    //printf("%s\n",currentView->pastPlays);
    return (((int)strlen(currentView->pastPlays)+1) / 40);
}

// Get the id of current player - ie whose turn is it?
PlayerID getCurrentPlayer(GameView currentView)
{
    //int playerID = (((int)strlen(currentView->pastPlays)+1)%40)/5;
    //printf("player:%d\n",playerID);
    return (((int)strlen(currentView->pastPlays)+1)%40)/5;
}

// Get the current score
int getScore(GameView currentView)
{
    return currentView->score;
}

// Get the current health points for a given player
int getHealth(GameView currentView, PlayerID player)
{
    return currentView->hp[player];
}

// Get the current location id of a given player
LocationID getLocation(GameView currentView, PlayerID player)
{
    int read = (getRound(currentView)-1)*40 + player*8; // Get the players string
    if(read<0) // if no data
      return -1;
    // get location abreviation
    char abbrev[2]; 
    abbrev[0] = currentView->pastPlays[read+1];
    abbrev[1] = currentView->pastPlays[read+2];
    // printf("Loc:%s\n",abbrev);
    int ID = abbrevToID(abbrev);
    if(ID != NOWHERE) // if this fails drac is in an unknown location
      return ID;
    else if(abbrev[0] == 'C') // unknown city
      return CITY_UNKNOWN;
    else if(abbrev[0] == 'S') // unknown sea
      return SEA_UNKNOWN;
    return -1;// Something went wrong :(
}

//// Functions that return information about the history of the game

// Fills the trail array with the location ids of the last 6 turns
void getHistory(GameView currentView, PlayerID player,
                            LocationID trail[TRAIL_SIZE])
{
    int i = 0;
    while (i < TRAIL_SIZE){
        trail[i] = currentView->trail[player][i];
        i++;
    }
}

//// Functions that query the map to find information about connectivity

// Returns an array of LocationIDs for all directly connected locations

LocationID *connectedLocations(GameView currentView, int *numLocations,
                               LocationID from, PlayerID player, Round round,
                               int road, int rail, int sea)
{
    //REPLACE THIS WITH YOUR OWN IMPLEMENTATION
    return NULL;
}
