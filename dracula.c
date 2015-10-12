// dracula.c
// Implementation of your "Fury of Dracula" Dracula AI

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "Game.h"
#include "DracView.h"
#include "Places.h"

static int LegalMove(DracView gameState, LocationID move);

void decideDraculaMove(DracView gameState)
{	
	printf ("hi\n");
	//enter teleport move first
	if(LegalMove(gameState, CASTLE_DRACULA))
		registerBestPlay("TP","Teleporting");
	//add possible moves
	int numLocs, r;
	LocationID *moves = whereCanIgo(gameState,&numLocs,TRUE,TRUE);
	r = (rand()%(numLocs));
	LocationID bestMove = moves[r];
	//check for legality, if times out, then teleport
	while (LegalMove(gameState,bestMove)==0){
		r = (rand()%(numLocs));
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

static int LegalMove(DracView gameState, LocationID move){
	LocationID trail[TRAIL_SIZE];
	giveMeTheTrail(gameState,PLAYER_DRACULA,trail);
	int i;
	for(i=0;i<TRAIL_SIZE;i++){
		if(move == trail[i]){
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
		//return 1;
	}
	return 1;
}
