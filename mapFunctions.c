#include <math.h>
#include "mapFunctions.h"
#include "entities.h"

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
	reach_point.X = sheep.X + round((direction.X) * ENTITY_SIZE/4);
	reach_point.Y = sheep.Y + round((direction.Y) * ENTITY_SIZE/4);
	return reach_point;
}


int is_in_zone(rencontre *sheep)
{
	if(distance(sheep->coord.X,sheep->coord.Y,CENTER_PEN_X,CENTER_PEN_Y) <= MAP_SIZE_X/10)
		return 1;
	else
		return 0;
}

int is_in_opposite_zone(rencontre *sheep)
{
	if(distance(sheep->coord.X,sheep->coord.Y,MAP_SIZE_X-CENTER_PEN_X,CENTER_PEN_Y) <= MAP_SIZE_X/10)
		return 1;
	else
		return 0;
}

coord circumvention(rencontre *sheep)
{
	coord objectif = reach_point(sheep->coord, direction(CENTER_PEN_X, CENTER_PEN_Y, sheep->coord.X, sheep->coord.Y));
	coord chemin;
	chemin = objectif;


	//Si le mouton est sur le bord, le décolle du bord
	if(chemin.X > MAP_SIZE_X-50)
	{
		chemin.X = MAP_SIZE_X;
		chemin.Y = sheep->coord.Y;
	}
	if(chemin.X < 50)
	{
		chemin.X = 0;
		chemin.Y = sheep->coord.Y;
	}
	if(chemin.Y > MAP_SIZE_Y-50)
	{
		chemin.X = sheep->coord.X;
		chemin.Y = MAP_SIZE_Y;
	}
	if(chemin.Y < 50)
	{
		chemin.X = sheep->coord.X;
		chemin.Y = 0;
	}

	return chemin;
}

coord bring_back_our_sheeps(rencontre *sheep)
{
	coord ret;
	ret = reach_point(sheep->coord, direction(CENTER_PEN_X, CENTER_PEN_Y, sheep->coord.X, sheep->coord.Y));
	return ret;
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

//renvoie 0 si la distance entre 2 points dépasse 3, renvoie 1 sinon , isOk
int checkpoint(coord point,coordF target)
{
  int radius = distance(point.X,point.Y,(int)(target.X),(int)(target.Y));
  if (radius < 3)
    return 1;
  else
    return 0;
}
