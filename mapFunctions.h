#ifndef MAPFUNCTIONS
#define MAPFUNCTIONS

#include "entities.h"
#include "global.h"
double MAP_SIZE_X;
double MAP_SIZE_Y;
double CENTER_PEN_X;
double CENTER_PEN_Y;
int mode;
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
coordF direction(int coordX1, int coordY1, int coordX2, int coordY2);
coord reach_point(coord sheep, coordF direction);
int is_in_zone(rencontre *sheep);
int is_in_opposite_zone(rencontre *sheep);
coord circumvention(rencontre *sheep);
coord bring_back_our_sheeps(rencontre *sheep);
int checkpoint(coord point,coordF target);

#endif
