#ifndef BOT_YELLOW
#define BOT_YELLOW

#include "entities.h"
#include "mapFunctions.h"

int iii;
int enclos;
unsigned char ID_suivi;
coord coord_blue_dog;
coord blue_direction;
rencontre *find_voisin_by_color(unsigned char* color, rencontre* voisins);
coord intel_yellow(rencontre *voisins);
int action_over_sheep(rencontre *sheep, rencontre *dog_tested);
int has_lower_ID(rencontre* voisins);
int has_lower_ID_center(rencontre* voisins);
coord follow_blue_dog(rencontre* voisins);
int has_lower_ID_center(rencontre* voisins);
int three_in_the_center(rencontre* voisins);
#endif
