#include <math.h>
#include "mapFunctions.h"

/****************************************************************************************************************************/
int distance(int coordX1, int coordY1, int coordX2, int coordY2)
{
	return sqrt((coordX2-coordX1)*(coordX2-coordX1)+(coordY2-coordY1)*(coordY2-coordY1));
}

coordF direction(int coordX1, int coordY1, int coordX2, int coordY2)
{
	coordF dir;
	dir.X = ((float) (coordX2 - coordX1)) / ((float) distance(coordX1, coordY1, coordX2, coordY2));
	dir.Y = ((float) (coordY2 - coordY1)) / ((float) distance(coordX1, coordY1, coordX2, coordY2));
	return dir;
}

coord reach_point(coord sheep, coordF direction)
{
	coord reach_point;
	reach_point.X = sheep.X + ceil((direction.X)*(dog->R_action + ENTITY_SIZE));
	reach_point.Y = sheep.Y + ceil((direction.Y)*(dog->R_action + ENTITY_SIZE));
	return reach_point;
}


int is_in_zone(rencontre *sheep)
{
	if(distance(sheep->coord.X,sheep->coord.Y,CENTER_PEN_X,CENTER_PEN_Y) <= MAP_SIZE_X/10)
		return 1;
	else
		return 0;
}

// //correction de plage d'effet si on se trouve près du plafond ou près du mur de gauche.
// //Pas encore effectif
// void correction(effect* border)
// {
// 	if (border->left < dog->R_action)
// 		border->left = dog->R_action;
// 	if (border->top < dog->R_action)
// 		border->top = dog->R_action;
// }
