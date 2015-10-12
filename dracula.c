// dracula.c
// Implementation of your "Fury of Dracula" Dracula AI

#include <stdlib.h>
#include <stdio.h>
#include "Game.h"
#include "DracView.h"
#include "places.h"


void decideDraculaMove(DracView gameState)
{	
	//enter teleport move first
	registerBestPlay("TP","Teleporting");
	//add possible moves
	int numLocs;
	LocationID *moves = whereCanIGo(gameState,*numLocs,TRUE,TRUE);
	LocationID bestMove = moves[0];
	//check for legality, if times out, then teleport
	while (LegalMove(gameState,bestMove)==0){
		int r = (rand()%(numLocs));
		bestMove=moves[r];
		r++;
	}
	switch (LegalMove(gameState,bestMove)){
		//standard move
		case 1: 
			registerBestPlay(idToAbbrev(bestMove),"IKnowWhatImDoing");
			break;
		//hide
		case 2:
			registerBestPlay("HI","IKnowWhatImDoing");
			break;
		//double back
		case 3:
			registerBestPlay("D1","BBBBBBB");
			break;
		case 4:
			registerBestPlay("D2","BBBBBBB");
			break;
		case 5:
			registerBestPlay("D3","BBBBBBB");
			break;
		case 6:
			registerBestPlay("D4","BBBBBBB");
			break;
		case 7:
			registerBestPlay("D5","BBBBBBB");
			break;
	}
	
}

int LegalMove(DracView gameState, LocationID move){
	LocationID trail[TRAIL_SIZE];
	giveMeTheTrail(gameState,PLAYER_DRACULA,trail);
	for(i=0,i<TRAIL_SIZE,i++){
		if(strcmp(&move,&trail[i])==0){
			if(i==0){
				if (hideInTrail(gameState, trail)==1){
					return 0;
				} else {
					return 2;
				}
			} else if (doubleBackInTrail(gameState,trail)==1){
				return 0;
			} else {
				return (i+2);
			}
		}
		return 1;
	}
}
