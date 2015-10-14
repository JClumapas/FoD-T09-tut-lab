// hunter.c
// Implementation of your "Fury of Dracula" hunter AI

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "Game.h"
#include "HunterView.h"



void decideHunterMove(HunterView gameState)
{
   //printf("------------------------------------------");
   int playerID = whoAmI(gameState);
   if(playerID == 0){
       int round = giveMeTheRound(gameState);
       LocationID trail[TRAIL_SIZE];
       giveMeTheTrail(gameState, whoAmI(gameState), trail);
       int dead = 0;
       int i;

       for(i=4; i<6; i++){
          if(trail[i] == nameToID("JM"))
             dead = i;
       }
       
       if(dead){
          if(round%2==0){
             registerBestPlay("KL","Camping 1");
          }else if(round%2==1){
             registerBestPlay("BC","Camping 2");
          }
       }else{
          if(dead == 4){
             registerBestPlay("SZ","Just dead");
          }else if(dead == 5){
             registerBestPlay("KL","Recovery 1");
          }else{
             registerBestPlay("KL","Recovery 2");
          }
       } 
   }else{
        fprintf(stderr, "player 1-4\n");
        srand((unsigned) time(NULL));
        if(giveMeTheRound(gameState) == 0){
            registerBestPlay("MU","Let the games Begin");
        }else{
            int numLocations = 0;
            int *places; 
            char message[15];
            fprintf(stderr, "Level 1\n");
            places = whereCanIgo(gameState, &numLocations, 1, 1, 0);
            fprintf(stderr, "Free\nnumLoc:%d\n", numLocations);
            registerBestPlay(idToAbbrev(places[1]),message); 
            //if(places[0])
            //    printf("a");
            //sprintf(message, "%s", idToAbbrev(places[1]));
            //registerBestPlay("MU",""); 
        }
   }
   //printf("\n\n\n\n");
   //egisterBestPlay("GE","I'm on holiday in Geneva");
}
