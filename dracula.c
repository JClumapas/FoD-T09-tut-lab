// dracula.c
// Implementation of your "Fury of Dracula" Dracula AI

#include <stdlib.h>
#include <stdio.h>
#include "Game.h"
#include "DracView.h"

void decideDraculaMove(DracView gameState)
{
	// TODO ...
	// Replace the line below by something better
	int numLocations;
	LocationID whereToGo[numLocations] = whereCanIGo(gameState, &numLocations, 1, 1);
	registerBestPlay("CD","Mwuhahahaha");
}
