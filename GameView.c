// GameView.c ... GameView ADT implementation

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "Globals.h"
#include "Game.h"
#include "GameView.h"
#include "Map.h" //use for get connections
     
#define NUM_HUNTERS 4
#define MAX_CONNECTIONS 100
#define SIZE_OF_PLAYS 7

struct gameView {
    Map europe;
    int numTurns;
    int numRounds;
    int hp[NUM_PLAYERS];
    LocationID trail[NUM_PLAYERS][TRAIL_SIZE];
    int score;
    char *pastPlays; // past plays
};

//not sure if they still need to be declared when they are defined before used
static void addToTrail(GameView currentView, PlayerID currPlayerID, LocationID currLocation);
static PlayerID letterToPlayerID(char l);
static LocationID handleDoubleBack(GameView gameView, PlayerID currPlayer, LocationID loc, int *index);
     
//adds the last seen location to the start of the array and pushes the rest along
static void addToTrail(GameView currentView, PlayerID currPlayerID, LocationID currLocation)
{
    int count = TRAIL_SIZE-1;
    while (count > 0){
        currentView->trail[currPlayerID][count] = currentView->trail[currPlayerID][count-1];
        count--;
    }
    currentView->trail[currPlayerID][0] = currLocation;
}

//converts first letter of pastplays to its corresponding playerID
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

static LocationID handleDoubleBack(GameView gameView, PlayerID currPlayerID, LocationID loc, int *index){
    LocationID location = loc;
    switch (location){
        case DOUBLE_BACK_1:
        location = gameView->trail[currPlayerID][1];
        *index = 1;
        break;
        case DOUBLE_BACK_2:
        location = gameView->trail[currPlayerID][2];
        *index = 2;
        break;
        case DOUBLE_BACK_3:
        location = gameView->trail[currPlayerID][3];
        *index = 3;
        break;
        case DOUBLE_BACK_4:
        location = gameView->trail[currPlayerID][4];
        *index = 4;
        break;
        case DOUBLE_BACK_5:
        location = gameView->trail[currPlayerID][5];
        *index = 5;
        break;
        default:
        break;
    }
    return location;
}

// Creates a new GameView to summarise the current state of the game
GameView newGameView(char *pastPlays, PlayerMessage messages[])
{
    //initializing gameView variables
    GameView gameView = malloc(sizeof(struct gameView));
    gameView->europe = newMap();
    gameView->score = GAME_START_SCORE;
    gameView->pastPlays = pastPlays;
    int i = 0;
    int j = 0;
    while (i<NUM_PLAYERS){
        while(j<TRAIL_SIZE){
            gameView->trail[i][j] = UNKNOWN_LOCATION;
            j++;
        }
        j=0;
        i++;
    }
    i = 0;
    while (i < NUM_HUNTERS){
        gameView->hp[i] = GAME_START_HUNTER_LIFE_POINTS;
        i++;
    }
    gameView->hp[PLAYER_DRACULA] = GAME_START_BLOOD_POINTS;

    //adjusts the data to current state
    if(pastPlays == NULL){
        return gameView;
    }
    int count = 3;
    int count2 = 0;
    int curr = 0;
    PlayerID currPlayerID = 0;
    LocationID location = 0;
    char currPlayer[1];
    char currLocation[3] = {'\0'};
    int pastPlaySize = strlen(pastPlays);
    while (curr < pastPlaySize){
        //prints messages just for fun
        printf("%s\n",messages[count2]);
        //the following gets the current player and adds locations to their respective trail array
        count = 3;
        currPlayer[0] = pastPlays[curr];
        currPlayerID = letterToPlayerID(*currPlayer);
        //adjusts round number
        if (currPlayerID == PLAYER_LORD_GODALMING){
            gameView->numRounds++;
        }
        currLocation[0] = pastPlays[curr+1];
        currLocation[1] = pastPlays[curr+2];
        location = abbrevToID(currLocation);
        addToTrail(gameView,currPlayerID,location);
        //clean this if i havent done so before its due

        //the following reads the characters after the first 3 to adjust the scores/hp
        if (currPlayerID != PLAYER_DRACULA){
            while (count < SIZE_OF_PLAYS){
                if (gameView->hp[currPlayerID] < 0){
                    //player died and was sent to hospital (locationID 59)(ST_JOSEPH_AND_ST_MARYS)
                    gameView->score -= SCORE_LOSS_HUNTER_HOSPITAL;
                    gameView->hp[currPlayerID] = 9;
                    //give the hunter a rest and dont apply more damage
                    break;
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
        //Dracula's turn
        }else{
            gameView->score -= SCORE_LOSS_DRACULA_TURN;
            //only this case changes the score or hp
            if (pastPlays[curr+5] == 'V'){
                //apply immature vampire has matured
                gameView->score -= SCORE_LOSS_VAMPIRE_MATURES;
            }
        }

        //adjust score and hp according to trails
        if (currPlayerID != PLAYER_DRACULA){
            if (gameView->numRounds > 1){
                //check if hunter rested
                if (location == gameView->trail[currPlayerID][1]){
                    gameView->hp[currPlayerID] += LIFE_GAIN_REST;
                    //hp cannot exceed 9
                    gameView->hp[currPlayerID] = 9;
                }
            }
        }else{
            int index = 0;
            //check for doublebacks
            if (location >= HIDE && location <= DOUBLE_BACK_5){
                    location = handleDoubleBack(gameView, currPlayerID, location, &index);
                //if it was a hide or double backed into a hide
                if (location == HIDE){location = gameView->trail[currPlayerID][index+1];}
            }
            //if exact location is known check if its a sea
            if (location <= MAX_MAP_LOCATION){
                if (isSea(location)){
                    gameView->hp[currPlayerID] -= LIFE_LOSS_SEA;
                //if dracula hid into a double back
                }else if (location >= DOUBLE_BACK_1 && location <= DOUBLE_BACK_5){
                    location = handleDoubleBack(gameView, currPlayerID, location, &index);
                    if (location <= MAX_MAP_LOCATION){
                      if (isSea(location)){
                          gameView->hp[currPlayerID] -= LIFE_LOSS_SEA;
                      }
                    }
                }
            }
            //check for Castle dracula or teleporting to it after hide and double backs were tracked down
            if (location == CASTLE_DRACULA || location == TELEPORT){
                 gameView->hp[currPlayerID] += LIFE_GAIN_CASTLE_DRACULA;
            //decrease dracula's hp if he is at an unknown sea
            }else if (location == SEA_UNKNOWN){
                 gameView->hp[currPlayerID] -= LIFE_LOSS_SEA;
            }
        }
        count2++;
        curr += 8;
    }

    return gameView;
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
    currentView->numRounds = (((int)strlen(currentView->pastPlays)+1) / 40);
    return currentView->numRounds;
}

// Get the id of current player - ie whose turn is it?
PlayerID getCurrentPlayer(GameView currentView)
{
    //printf("player:%d\n",(int)strlen(currentView->pastPlays));
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
   
    int read = (getRound(currentView))*40 + player*8; // Get the players string
    if(read > (int)strlen(currentView->pastPlays)){
      read -= 40;
    }
      
    //read = read - 39;
    if(read<0) // if no data
      return -1;
    // get location abreviationc
    char abbrev[2]; 
    abbrev[0] = currentView->pastPlays[read+1];
    abbrev[1] = currentView->pastPlays[read+2];
    //printf("%s\n
    int ID = abbrevToID(abbrev);
    //printf("Loc:%s %d\n",abbrev,ID);
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
    LocationID *locations = malloc(NUM_MAP_LOCATIONS*sizeof(LocationID));
    int numLoc = adjacentLocations((int)from, -1, (int)player, (int)round, road, rail, sea, locations, -1);
    int i,e;
    int found;
    LocationID conectedLocs[numLoc];
    // remove duplicates and backtracks for drac
    for(i=0; i<numLoc; i++){
      found = 0;
      for(e=0; e<*numLocations && !found; e++){
         if(locations[i] == conectedLocs[e])
            found = 1;
       }
       if(!found){
         conectedLocs[*numLocations] = locations[i];
         *numLocations+=1;
       }
    }
      
    numLoc = conectedLocs[0]; //keep comiler happy
    numLoc = locations[0];
    
    //return conectedLocs; 
    return NULL; //keep comiler happy
}
