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
   if(howHealthyIs(gameState,playerID)==0){
        registerBestPlay("JM","Dead");
   }else if(playerID == 0){
       int round = giveMeTheRound(gameState);
       LocationID trail[TRAIL_SIZE];
       giveMeTheTrail(gameState, whoAmI(gameState), trail);
       int dead = 0;
       int i;

       for(i=0; i<3; i++){
          if(trail[i] == nameToID("JM"))
             dead = i;
          fprintf(stderr, "Dead%d\n",dead);
       }
       
       if(dead){
          if(round%2==0){
             registerBestPlay("KL","Camping 1");
          }else if(round%2==1){
             registerBestPlay("BC","Camping 2");
          }
       }else{
          if(dead == 0){
             registerBestPlay("SZ","Recovery 1");
          }else if(dead == 1){
             registerBestPlay("KL","Recovery 2");
          }else{
             registerBestPlay("KL","Recovery 3");
          }
       } 
   }else{
        fprintf(stderr, "\n\nHunters\n");
        srand((unsigned) time(NULL));
        if(giveMeTheRound(gameState) == 0){
            registerBestPlay("MU","Let the games Begin");
        }else{
            int numLocations = 0;
            int *places; 
            char message[15];
            int i;
            fprintf(stderr,"huntFrom:%s\n",idToName(whereIs(gameState, playerID)));
            fprintf(stderr, "\n\n\n");
            places = whereCanIgo(gameState, &numLocations, 1, 1, 0);
            fprintf(stderr, "Free\nnumLoc:%d\n", numLocations);
            for(i=0; i<numLocations; i++){
                fprintf(stderr, "%s,",idToName(places[i]));
            }
            
            srand(time(NULL));
            registerBestPlay(idToAbbrev(places[rand()%numLocations]),message); 
            //if(places[0])
            //    printf("a");
            sprintf(message, "\n\n");
            //registerBestPlay("MU",""); 
        }
        
   }
   //printf("\n\n\n\n");
   //egisterBestPlay("GE","I'm on holiday in Geneva");
}
