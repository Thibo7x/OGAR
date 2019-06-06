#ifndef MAPFUNCTIONS
#define MAPFUNCTIONS

#include "entities.h"
#include "global.h"

typedef struct node_view_borders view;
struct node_view_borders
{
	 float left;
	 float top;
	 float right;
	 float bottom;
};

typedef struct node_effect_borders effect;
struct node_effect_borders
{
	 float left;
	 float top;
	 float right;
	 float bottom;
};

int distance(int coordX1, int coordY1, int coordX2, int coordY2);
unsigned int action_over_sheep(rencontre *sheep);
coordF direction(int coordX1, int coordY1, int coordX2, int coordY2);
coord reach_point(coord sheep, coordF direction);
void correction(effect* border);

#endif
