#include "bot_yellow.h"
#include "entities.h"
#include "mapFunctions.h"
#include <string.h>
#include <math.h>
#define min(a,b) (a<=b?a:b)

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
	coord objectif = borderCorrection(reach_point(sheep->coord, direction(100, MAP_SIZE_Y/2, sheep->coord.X, sheep->coord.Y)));
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
	if (distance(dog->coord.X, dog->coord.Y, 0, MAP_SIZE_Y/2) <= MAP_SIZE_X/10)
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
//A tester
{
	int has_lower_ID = 1;
	rencontre* yellow_tester = voisins;
	if(dog->coord.X != MAP_SIZE_X/2 || dog->coord.Y != MAP_SIZE_Y/2)
		has_lower_ID = 0;
	while(yellow_tester->next != NULL && has_lower_ID == 1)
	{
		if(!memcmp(yellow_tester->couleur,"\xff\x0\xff",3))
		{
			if (yellow_tester->ID < dog->ID) {
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
		while ((blue_radar != NULL) || (!memcmp(blue_radar->couleur,"\x0\xff\x0",3)))
		{
			blue_radar = blue_radar->next;
		}
		if(blue_radar != NULL)
		{
			direction = direction(dog->coord.X,dog->coord.Y,blue_radar->coord.X,blue_radar->coord.Y);
			reach_point.X = ceil((direction.X)*(min(MAP_SIZE_X,MAP_SIZE_Y)));
			reach_point.Y = ceil((direction.Y)*(min(MAP_SIZE_X,MAP_SIZE_Y)));
		}
	}
	return reach_point;
}
