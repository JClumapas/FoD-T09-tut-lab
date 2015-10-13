// hunter.c
// Implementation of your "Fury of Dracula" hunter AI

#include <stdlib.h>
#include <stdio.h>
#include "Game.h"
#include "HunterView.h"



void decideHunterMove(HunterView gameState)
{
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
         registerBestPlay("KL","Dead 1");
      }else if(round%2==1){
         registerBestPlay("GA","Dead 2");
      }
   }else{
      if(dead == 4){
         registerBestPlay("SZ","");
      }else if(dead == 5){
         registerBestPlay("KL","");
      }else{
         registerBestPlay("KL","");
      }
   } 
   //registerBestPlay("GE","I'm on holiday in Geneva");
}
