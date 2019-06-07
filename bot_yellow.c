#include "bot_yellow.h"
#include "entities.h"
#include "mapFunctions.h"
#include <stdio.h>
#include <math.h>
#include <string.h>
#define min(a,b) (a<=b?a:b)

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
		{
			ID_suivi = sheep_viseur->ID;
			break;
		}
	}

	if (sheep_viseur != NULL)
	{
		if (ID_suivi)
		{
			sheep_viseur = voisins;
			while (sheep_viseur->ID != ID_suivi)
			{
				sheep_viseur = sheep_viseur->next;
			}
		}
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
		coord go = follow_blue_dog(voisins);
		obj.X = go.X;
		obj.Y = go.Y;
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
	coord objectif = borderCorrection(reach_point(sheep->coord, direction(100, MAP_SIZE_Y/2, sheep->coord.X, sheep->coord.Y)));
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
	if (distance(dog->coord.X, dog->coord.Y, 0, MAP_SIZE_Y/2) <= MAP_SIZE_X/10)
	{
		enclos = 1;
	}
	if (distance(dog->coord.X, dog->coord.Y, 0, MAP_SIZE_Y/2) >= MAP_SIZE_X/8)
	{
		enclos = 0;
	}

	if (enclos)
	{
		ret.X = MAP_SIZE_X/2;
		ret.Y = MAP_SIZE_Y/2;
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

int has_lower_ID(rencontre* voisins)
//Teste si parmis les chiens jaunes groupés au centre, le chien à l'ID le plus bas
{
	int has_lower_ID = 1;
	rencontre* yellow_tester = voisins;
	if((dog->coord.X != MAP_SIZE_X/2) || (dog->coord.Y != MAP_SIZE_Y/2) || (yellow_tester == NULL))
		return 0;
	while(yellow_tester->next != NULL && has_lower_ID == 1)
	{
		if(!memcmp(yellow_tester->couleur,"\xff\x0\xff",3))
		{
			if ((unsigned int)yellow_tester->ID < (unsigned int)dog->ID) {
				has_lower_ID = 0;
			}
			else
				yellow_tester = yellow_tester->next;
		}
	}
	return has_lower_ID;
}

coord follow_blue_dog(rencontre* voisins)
//Renvoie les coordonnées à suivre pour atteindre la brebis indiquée par le chien bleu
//A tester
{
	rencontre* blue_radar = voisins;
	coordF sheep_direction;
	coord reach_point;
	if(has_lower_ID(voisins))
	{
		printf("id le plus bas\n");
		while ((blue_radar->next != NULL) || (!memcmp(blue_radar->couleur,"\x0\xff\x0",3)))
		{
			blue_radar = blue_radar->next;
		}
		if(blue_radar != NULL)
		{
			sheep_direction = direction(dog->coord.X,dog->coord.Y,blue_radar->coord.X,blue_radar->coord.Y);
			reach_point.X = ceil((sheep_direction.X)*(min(MAP_SIZE_X,MAP_SIZE_Y)));
			reach_point.Y = ceil((sheep_direction.Y)*(min(MAP_SIZE_X,MAP_SIZE_Y)));
		}
	}
	else
	{
		reach_point.X = MAP_SIZE_X/2;
		reach_point.Y = MAP_SIZE_Y/2;
	}
	return reach_point;
}
