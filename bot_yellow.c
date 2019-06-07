#include "bot_yellow.h"
#include "entities.h"
#include "mapFunctions.h"
#include <stdio.h>
#include <math.h>
#include <string.h>

coord intel_yellow(rencontre *voisins)
{
	rencontre *sheep_viseur = voisins;
	coord obj;
	obj.X = 0;
	obj.Y = 0;

	sheep_viseur = voisins;
	while (sheep_viseur != NULL)
	{
		if (memcmp(sheep_viseur->couleur,"\xe6\xf0\xf0",3))
			sheep_viseur = sheep_viseur->next;
		else
			break;
	}

	if (sheep_viseur != NULL)
	{
		coord co = circumvention(sheep_viseur);
		int radius = (int) ceil(distance(co.X, co.Y, dog->coord.X, dog->coord.Y));
		if ((radius > 2) && dog->mode == 0)
		{
			obj.X = co.X;
			obj.Y = co.Y;
		}
		else
		{
			if (dog->mode == 0)
			{
				dog->mode = 1;
			}
			coord go = bring_back_our_sheeps(sheep_viseur);
			obj.X = go.X;
			obj.Y = go.Y;
		}
	}
	else
	{
		dog->mode = 0;
		obj.X = 4500;
		obj.Y = 3000;
	}
	return obj;
}

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
		objectif.X = MAP_SIZE_X;
		objectif.Y = reach_point.Y;
	}
	else if(dog->coord.X < dog->R_action)
	{
		objectif.X = 0;
		objectif.Y = reach_point.Y;
	}
	else if(dog->coord.Y > MAP_SIZE_Y-dog->R_action)
	{
		objectif.X = reach_point.X;
		objectif.Y = MAP_SIZE_Y;
	}
	else if(dog->coord.Y < dog->R_action)
	{
		objectif.X = reach_point.X;
		objectif.Y = 0;
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
	int radiusX = (int)distance(objectif.X, dog->coord.Y, dog->coord.X, dog->coord.Y);
	coord chemin;
	chemin.X = objectif.X;
	if (radiusX > 2)
	{
		chemin.Y = dog->coord.Y;
	}
	else
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

