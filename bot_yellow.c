#include "bot_yellow.h"
#include "entities.h"
#include "mapFunctions.h"
#include "chainedListFunctions.h"
#include <stdio.h>
#include <math.h>
#include <string.h>
#define min(a,b) (a<=b?a:b)

coord intel_yellow(rencontre *voisins)
{
	rencontre *sheep_viseur = voisins;
	rencontre *blue_viseur = voisins;
	coord obj;
	int radius;
	obj.X = 0;
	obj.Y = 0;

	sheep_viseur = find_voisin_by_color((unsigned char *)"\xe6\xf0\xf0", voisins);
	blue_viseur = find_voisin_by_color((unsigned char *)"\x0\x0\xff",voisins);
	switch (dog->mode) {

		case 0:
			// Ordre
			obj.X = MAP_SIZE_X/2;
			obj.Y = MAP_SIZE_Y/2;

			// Sorties
			if(sheep_viseur != NULL && distance(dog->coord.X, dog->coord.Y, 0, MAP_SIZE_Y/2) > (MAP_SIZE_X/10+dog->view.X))
				dog->mode = 2;
			if((blue_viseur != NULL) && (dog->coord.X == MAP_SIZE_X/2) && (dog->coord.Y == MAP_SIZE_Y/2))
			{
				coord_blue_dog = follow_blue_dog(voisins, blue_viseur);
				dog->mode = 3;
			}
		break;

		case 1:
			// Ordre
			obj = bring_back_our_sheeps(rechercherListeChainee(sheep_viseur, ID_suivi));
			iii++;

			// Sorties
			if (distance(dog->coord.X, dog->coord.Y, 0, MAP_SIZE_Y/2) <= MAP_SIZE_X/10)
			{
				dog->mode = 0;
			}

			if (iii == dog->R_action/2)
			{
				iii = 0;
				dog->mode = 2;
			}
		break;

		case 2:
			//Ordre
			if (ID_suivi != sheep_viseur->ID) // MAJ ID_suivi
				ID_suivi = sheep_viseur->ID;
			obj = circumvention(sheep_viseur);

			//Sorties
			radius = (int) ceil(distance(obj.X, obj.Y, dog->coord.X, dog->coord.Y));
			if (radius <= 2) // Il pousse le mouton s'il est bien positionné
			{
				dog->mode = 1;
			}
		break;

		case 3:
			//Ordre
			if(distance(blue_viseur->coord.X,blue_viseur->coord.Y,MAP_SIZE_X/2,MAP_SIZE_Y/2) < 3)
			 blue_dog = 1;
			if(blue_dog)
			{
				blue_direction = follow_blue_dog(voisins,blue_viseur);
 				obj = blue_direction;
			}
			//Sorties
			if(distance(dog->coord.X,dog->coord.Y,MAP_SIZE_X/2,MAP_SIZE_Y/2) >= 250)
			{
				dog->mode = 4;
				blue_dog = 0;
			}

		break;

		case 4:
			//Ordre
			obj = blue_direction;
			//Sorties
			if(sheep_viseur != NULL)
				dog->mode = 2;
		break;
		default:
		dog->mode = 0;
		break;
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

coord circumvention(rencontre *sheep)
{
	int decalage = 0;
	coord objectif = reach_point(sheep->coord, direction(100, MAP_SIZE_Y/2, sheep->coord.X, sheep->coord.Y));
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

	//Si le mouton est sur le bord, le décolle du bord
	if(chemin.X > MAP_SIZE_X-dog->R_action)
	{
		chemin.X = MAP_SIZE_X;
		chemin.Y = sheep->coord.Y;
		decalage = 1;
	}
	if(chemin.X < dog->R_action)
	{
		chemin.X = 0;
		chemin.Y = sheep->coord.Y;
		decalage = 1;
	}
	if(chemin.Y > MAP_SIZE_Y-dog->R_action)
	{
		chemin.X = sheep->coord.X;
		chemin.Y = MAP_SIZE_Y;
		decalage = 1;
	}
	if(chemin.Y < dog->R_action)
	{
		chemin.X = sheep->coord.X;
		chemin.Y = 0;
		decalage = 1;
	}
	if(decalage && distance(dog->coord.X,dog->coord.Y,sheep->coord.X,sheep->coord.Y) >= dog->R_action)
	{
		chemin.X = dog->coord.X;
		chemin.Y = dog->coord.Y;
	}
	return chemin;
}

coord bring_back_our_sheeps(rencontre *sheep)
{
	coord ret;
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
	return ret;
}

int has_lower_ID(rencontre* voisins)
//Teste si parmis les chiens jaunes groupés au centre, le chien à l'ID le plus bas
{
	int has_lower_ID = 1;
	rencontre* yellow_tester = voisins;
	if(yellow_tester == NULL)
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

coord follow_blue_dog(rencontre* voisins, rencontre* blue_radar)
//Renvoie les coordonnées à suivre pour atteindre la brebis indiquée par le chien bleu
{
	coordF sheep_direction;
	coord reach_point;
	if(!has_lower_ID(voisins))
	{
		reach_point.X = MAP_SIZE_X/2;
		reach_point.Y = MAP_SIZE_Y/2;
		dog->mode = 0;
	}
	else
	{
		if(distance(dog->coord.X,dog->coord.Y,MAP_SIZE_X/2,MAP_SIZE_Y/2) < 250)
		{
			reach_point = blue_radar->coord;
		}
		if(distance(dog->coord.X,dog->coord.Y,MAP_SIZE_X/2,MAP_SIZE_Y/2) >= 250)
		{
			sheep_direction = direction(MAP_SIZE_X/2,MAP_SIZE_Y/2,blue_radar->coord.X,blue_radar->coord.Y);
			reach_point.X = MAP_SIZE_X/2 + ceil((sheep_direction.X)*((int)(min(MAP_SIZE_X, MAP_SIZE_Y))));
			reach_point.Y = MAP_SIZE_Y/2 + ceil((sheep_direction.Y)*((int)(min(MAP_SIZE_X, MAP_SIZE_Y))));
			if (reach_point.X>MAP_SIZE_X)
				reach_point.X = MAP_SIZE_X;
			if (reach_point.X<0)
				reach_point.X = 0;
			if (reach_point.Y>MAP_SIZE_Y)
				reach_point.Y = MAP_SIZE_Y;
			if (reach_point.Y<0)
				reach_point.Y = 0;
		}
	}
	return reach_point;
}
	// coordF sheep_direction;
	// coord reach_point;
	// if(has_lower_ID(voisins))
	// {
	// sheep_direction = direction(dog->coord.X,dog->coord.Y,blue_radar->coord.X,blue_radar->coord.Y);
	// reach_point.X = blue_radar->coord.X + ceil((sheep_direction.X)*((int)(min(MAP_SIZE_X, MAP_SIZE_Y))));
	// reach_point.Y = blue_radar->coord.Y + ceil((sheep_direction.Y)*((int)(min(MAP_SIZE_X, MAP_SIZE_Y))));
	// 	if (reach_point.X>MAP_SIZE_X)
	// 		reach_point.X = MAP_SIZE_X;
	// 	if (reach_point.X<0)
	// 		reach_point.X = 0;
	// 	if (reach_point.Y>MAP_SIZE_Y)
	// 		reach_point.Y = MAP_SIZE_Y;
	// 	if (reach_point.Y<0)
	// 		reach_point.Y = 0;
	// }
	// else
	// {
	// 	reach_point.X = MAP_SIZE_X/2;
	// 	reach_point.Y = MAP_SIZE_Y/2;
	// 	dog->mode = 0;
	// }
	// return reach_point;
