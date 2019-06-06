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

coord circumvention(rencontre *sheep)
{
	coord objectif = reach_point(sheep->coord, direction(100, 3000, sheep->coord.X, sheep->coord.Y));
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
