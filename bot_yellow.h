#ifndef BOT_YELLOW
#define BOT_YELLOW

#include "entities.h"
#include "mapFunctions.h"
int iii;

coord intel_yellow(rencontre *voisins);
unsigned int action_over_sheep(rencontre *sheep);
coord borderCorrection(coord reach_point);
coord circumvention(rencontre *sheep);
coord bring_back_our_sheeps(rencontre *sheep);

#endif
