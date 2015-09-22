// GameView.c ... GameView ADT implementation

#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "Globals.h"
#include "Game.h"
#include "GameView.h"
#include "Map.h" //use for get connections
     
#define NUM_HUNTERS 4

struct gameView {
    Map europe;
    int numRounds;
    int hp[NUM_PLAYERS];
    int trail[NUM_PLAYERS][TRAIL_SIZE];
    int score;
};

static PlayerID letterToPlayerID(char l);
     

// Creates a new GameView to summarise the current state of the game
GameView newGameView(char *pastPlays, PlayerMessage messages[])
{
    //initializing gameView variables
    GameView gameView = malloc(sizeof(struct gameView));
    gameView->europe = newMap();
    gameView->numRounds = (((int)strlen(pastPlays)+1) / 40);
    gameView->score = GAME_START_SCORE;
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
    int count2 = 0;
    int curr = 0;
    PlayerID currPlayerID = 0;
    LocationID Location = 0;
    char currPlayer[1];
    char currLocation[3] = {'\0'};
    //int pastPlaySize = strlen(pastPlays);
    while (curr < pastPlaySize){
        //the following gets the current player and adds locations to their respective trail array
        numTurns = gameView->numRounds; //this is not correct
        count = 3;
        currPlayer[0] = pastPlays[curr];
        currPlayerID = letterToPlayerID(currPlayer);
        currLocation[0] = pastPlays[curr+1];
        currLocation[1] = pastPlays[curr+2];
        gameView->trail[currPlayerID][numTurns] = abbrevToID(currLocation);
        //clean this if i havent done so before its due

        //the following reads the characters after the first 3 to adjust the scores/hp
        if (currPlayerID != PLAYER_DRACULA){
            while (count < 7){
                if (gameView->hp[currPlayerID] < 0){
                    //player died and was sent to hospital (lactionID 59)(ST_JOSEPH_AND_ST_MARYS)
                    gameView->hp[currPlayerID] = 9;
                }
                if (pastPlays[curr+count] == 'T'){
                    //apply trap consequences
                    gameView->hp[currPlayerID] -= LIFE_LOSS_TRAP_ENCOUNTER;
                }else if (pastPlays[curr+count] == 'D'){
                    //apply consequences for encountering dracula
                    gameView->hp[currPlayerID] -= LIFE_LOSS_DRACULA_ENCOUNTER;
                    gameView->hp[PLAYER_DRACULA] -= LIFE_LOSS_HUNTER_ENCOUNTER;
                }else{
                    count = 7;
                    continue;
                }
                count++;
            }
        }else{
            gameView->score -= SCORE_LOSS_DRACULA_TURN;
            //only this case changes the score or hp
            if (pastPlays[curr+5] == 'V'){
                //apply immature vampire has matured
                gameView->score -= SCORE_LOSS_VAMPIRE_MATURES;
            }
        }

        //adjust score according to trails
        if (currPlayerID != PLAYER_DRACULA){
            i = 0;
            //checking for rests in the player's trail
            while (i < TRAIL_SIZE-1){
                if(trail[currPlayerID][i] == trail[currPlayerID][i] && trail[currPlayerID][i] != UNKNOWN_LOCATION){
                    gameView->hp[currPlayerID] += LIFE_GAIN_REST;
                    if (gameView->hp[currPlayerID] > 9){
                        //cannot exceed 9hp
                        gameView->hp[currPlayerID] = 9;
                    }
                }
                i++;
            }

        }else{
            i = 0;
            //checking for castle dracula or sea in the trail
            while (i < TRAIL_SIZE){
                if (gameView->trail[currPlayerID][i] == CASTLE_DRACULA){
                    gameView->hp[currPlayerID] += LIFE_GAIN_CASTLE_DRACULA;
                }else if (isSea(gameView->trail[currPlayerID][i])){
                    gameView->hp[currPlayerID] -= LIFE_LOSS_SEA;
                }
                i++;
            }
        }
        curr += 8;
    }

    return gameView;
}

static PlayerID letterToPlayerID(char l)
{
    PlayerID curr;
    if (l == 'G'){
        curr = PLAYER_LORD_GODALMING;
    }else if (l == 'S'){
        curr = PLAYER_DR_SEWARD;
    }else if (l == 'H'){
        curr = PLAYER_VAN_HELSING;
    }else if (l == 'M'){
        curr = PLAYER_MINA_HARKER;
    }else{
        curr = PLAYER_DRACULA;
    }
    return curr;
}
     
     
// Frees all memory previously allocated for the GameView toBeDeleted
void disposeGameView(GameView toBeDeleted)
{
    //COMPLETE THIS IMPLEMENTATION
    disposeMap(toBeDeleted->europe);
    free( toBeDeleted );
}


//// Functions to return simple information about the current state of the game

// Get the current round
Round getRound(GameView currentView)
{
    //printf("%s\n",currentView->pastPlays);
    return currentView->numRounds;
}

// Get the id of current player - ie whose turn is it?
PlayerID getCurrentPlayer(GameView currentView)
{
    //int playerID = (((int)strlen(currentView->pastPlays)+1)%40)/5;
    //printf("player:%d\n",playerID);
    //return (((int)strlen(currentView->pastPlays)+1)%40)/5;
    return ((currentView->numRounds-1)%5);
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
