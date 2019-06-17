#ifndef BOTPURPLE
#define BOTPURPLE

#include "entities.h"
#include "mapFunctions.h"
#include "chainedListFunctions.h"

int saved_rank;
coordF target_top;
coordF target_bottom;
float points[4][2];

void test_split_purpel(void);
void split_purpel(void);
int checkpoint(coord point,coordF target);
coord spotting_purple(void);
coord intel_purple(rencontre *voisins);


int A_REMPLACER_X;
int A_REMPLACER_Y;


#endif
