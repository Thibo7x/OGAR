#include "bot_yellow.h"
#include "entities.h"
#include "mapFunctions.h"

unsigned int action_over_sheep(rencontre *sheep)
{
	if (distance(dog->coord.X, dog->coord.Y, sheep->coord.X, sheep->coord.Y) < dog->R_action)
		return 1;
	else
		return 0;
}

coord borderCorrection(coord reach_point)
//NE MARCHE PAS FAUT L'AMELIORER SVP, permet à un chien de déplacer un mouton se trouvant proche d'une bordure
{
	coord objectif;
	if(dog->coord.X > MAP_SIZE_X-dog->R_action)
	{
		objectif.X = MAP_SIZE_X-51;
		objectif.Y = reach_point.Y;
	}
	else if(dog->coord.X < dog->R_action)
	{
		objectif.X = 51;
		objectif.Y = reach_point.Y;
	}
	else if(dog->coord.Y > MAP_SIZE_Y-dog->R_action)
	{
		objectif.X = reach_point.X;
		objectif.Y = MAP_SIZE_Y-51;
	}
	else if(dog->coord.Y < dog->R_action)
	{
		objectif.X = reach_point.X;
		objectif.Y = 51;
	}
	else
	{
		objectif.X = reach_point.X;
		objectif.Y = reach_point.Y;
	}
	return objectif;
}


coord circumvention(rencontre *sheep)
{
	coord objectif = borderCorrection(reach_point(sheep->coord, direction(100, 3000, sheep->coord.X, sheep->coord.Y)));
	// printf("%d\n", objectif.X);
	// printf("%d\n", objectif.Y);
	coord chemin;
	chemin.X = objectif.X;
	if (objectif.X != dog->coord.X)
	{
		// if (action_over_sheep(coord++)) ...
		chemin.Y = dog->coord.Y;
	}
	else// moveBot(wsi, sheep->coord.X, sheep->coord.Y);
	 // if(distance(dog->coord.X,dog->coord.Y,sheep->coord.X,sheep->coord.Y) < ( R_ACTION[dog->color]) ){
	 //  moveBot(wsi,dog->coord.X,dog->coord.Y);
		// dog->mode = 0;
	 // }
	{
		chemin.Y = objectif.Y;
	}
	return chemin;
}

coord bring_back_our_sheeps(rencontre *sheep)
{
	coord ret;
	if (distance(dog->coord.X, dog->coord.Y, 0, 3000) <= 900)
	{
		ret.X = 4500;
		ret.Y = 3000;
	}
	else
	{
		iii++;
		if(distance(dog->coord.X,dog->coord.Y,sheep->coord.X,sheep->coord.Y) < ( dog->R_action) )
		{
			ret.X = dog->coord.X;
			ret.Y = dog->coord.Y;
		}
		else
		{
			ret.X = sheep->coord.X;
			ret.Y = sheep->coord.Y;
		}
		if (iii == dog->R_action)
		{
		 iii = 0;
		 dog->mode = 0;
		}
	}
	return ret;
}
