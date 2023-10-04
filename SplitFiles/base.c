#include <stdlib.h>
#include "base.h"

GameParameters* get_game_parameters() {
    //Get a fresh GameParameters object from a reliable source
    //This should be kept up to date with changes in the GameParameters definition
    GameParameters* params = malloc(sizeof(GameParameters));
    params->ticks = 0;
    params->score = 0;
    params->slices = 0;
    params->gameOverFlag = 0;
    params->maxToast = 0;
    params->messageAddress = 0;
    params->message = NULL;
    params->messageSourceAddress = NULL;
    params->effect = NULL;
    params->breadBin = NULL;
    params->reputation = 1000; //Start with 1000 rep
    params->hotelOpen = 1;     //Open by default
    return params;
}