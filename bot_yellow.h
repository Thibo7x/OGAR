#ifndef BOT_YELLOW
#define BOT_YELLOW

#include "entities.h"
#include "mapFunctions.h"
#define MAP_SIZE_X 9000.0
#define MAP_SIZE_Y 6000.0
int iii;

unsigned int action_over_sheep(rencontre *sheep);
coord borderCorrection(coord reach_point);
coord circumvention(rencontre *sheep);
coord bring_back_our_sheeps(rencontre *sheep);

#endif
