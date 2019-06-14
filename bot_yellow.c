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
	rencontre *yellow_viseur = voisins;
	rencontre *sheep_vise;
	coord obj;
	int radius;
	obj.X = 0;
	obj.Y = 0;

	sheep_viseur = find_voisin_by_color((unsigned char *)"\xe6\xf0\xf0", voisins);
	blue_viseur = find_voisin_by_color((unsigned char *)"\x0\x0\xff",voisins);
	yellow_viseur = find_voisin_by_color((unsigned char *)"\xff\xff\x0",voisins);
	//printf("MODE : %d\n", dog->mode);
	switch (dog->mode) {

		case 0:
			// Ordre
			obj.X = MAP_SIZE_X/2;
			obj.Y = MAP_SIZE_Y/2;

			// Sorties
			if(sheep_viseur != NULL)
			{
				//if (!is_in_zone(sheep_viseur) && PAS_PRIS)
				if (!is_in_zone(sheep_viseur))
				{
					ID_suivi = sheep_viseur->ID;
					dog->mode = 2;
				}
			}
			if((blue_viseur != NULL) && (dog->coord.X == MAP_SIZE_X/2) && (dog->coord.Y == MAP_SIZE_Y/2) && has_lower_ID_center(voisins))
			{
				dog->mode = 3;
			}
		break;

		case 1:
			// Ordre
			sheep_vise = rechercherListeChainee(sheep_viseur, ID_suivi);

			if (sheep_vise == NULL) // On a perdu le mouton
			{
				dog->mode = 0;
				obj.X = MAP_SIZE_X/2;
				obj.Y = MAP_SIZE_Y/2;
				break;
			}
			else
			{
				obj = bring_back_our_sheeps(sheep_vise);
				iii++;
			}

			// Sorties
			if(distance(dog->coord.X, dog->coord.Y, 0, MAP_SIZE_Y/2) <= MAP_SIZE_X/10 || sheep_viseur == NULL)
			{
				dog->mode = 0;
			}

			if(iii == 0.75*dog->R_action)
			{
				iii = 0;
				dog->mode = 2;
			}
		break;

		case 2:
			//Ordre
			if(yellow_viseur != NULL && sheep_viseur != NULL)
			{
				//|| distance(yellow_viseur->coord.X,yellow_viseur->coord.Y,dog->coord.X,dog->coord.Y) == 0
				if (distance(yellow_viseur->coord.X,yellow_viseur->coord.Y,sheep_viseur->coord.X,sheep_viseur->coord.Y) < distance(dog->coord.X,dog->coord.Y,sheep_viseur->coord.X,sheep_viseur->coord.Y) )
				{
					dog->mode = 0;
					obj.X = MAP_SIZE_X/2;
					obj.Y = MAP_SIZE_Y/2;
					break;
				}
			}
			sheep_viseur = rechercherListeChainee(voisins, ID_suivi);
			// if(ID_suivi != sheep_viseur->ID) // MAJ ID_suivi
			// 	ID_suivi = sheep_viseur->ID;
			if (sheep_viseur == NULL)
			{
				obj.X = MAP_SIZE_X/2;
				obj.Y = MAP_SIZE_Y/2;
				dog->mode = 0;
				break;
			}

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
			if(blue_viseur != NULL && distance(blue_viseur->coord.X,blue_viseur->coord.Y,MAP_SIZE_X/2,MAP_SIZE_Y/2) < 3)
			{
				blue_dog = 1;
			}
			if(blue_dog)
			{
				blue_direction = follow_blue_dog(voisins,blue_viseur);
 			 	obj = blue_direction;
			}
			else
			{
				obj.X = MAP_SIZE_X/2;
				obj.Y = MAP_SIZE_Y/2;
				blue_dog = 0;
			}
			//Sorties
			if(distance(dog->coord.X,dog->coord.Y,MAP_SIZE_X/2,MAP_SIZE_Y/2) >= 300)
				dog->mode = 4;
		break;

		case 4:
			//Ordre
			blue_dog = 0;
			obj = blue_direction;
			//Sorties
			if(sheep_viseur != NULL)
			{
				ID_suivi = sheep_viseur->ID;
				dog->mode = 2;
			}
			if(dog->coord.X < 50 || dog->coord.Y < 50 || dog->coord.X > MAP_SIZE_X-50 || dog->coord.Y > MAP_SIZE_Y-50)
				dog->mode = 0;
		break;
		default:
		dog->mode = 0;
		break;
	}
	return obj;
}

int action_over_sheep(rencontre *sheep, rencontre *dog_tested) //
{
	if (distance(dog_tested->coord.X, dog_tested->coord.Y, sheep->coord.X, sheep->coord.Y) < dog->R_action)
	{
		if (!has_lower_ID(dog_tested))
			return 1;
		else
			return 0;
	}
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
	if(distance(dog->coord.X,dog->coord.Y,sheep->coord.X,sheep->coord.Y) < ( dog->R_action /2) )
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

int has_lower_ID_center(rencontre* voisins)
//Teste si parmis les chiens jaunes groupés au centre, le chien à l'ID le plus bas
{
	int has_lower_ID = 1;
	rencontre* yellow_tester = voisins;
	if(yellow_tester == NULL)
		return has_lower_ID;
	while((yellow_tester->next != NULL) && (has_lower_ID == 1))
	{
		if(!memcmp(yellow_tester->couleur,"\xff\xff\x0",3) && distance(MAP_SIZE_X/2,MAP_SIZE_Y/2, voisins->coord.X,voisins->coord.Y))
		{
			if(yellow_tester->ID < dog->ID && distance(yellow_tester->coord.X,yellow_tester->coord.Y,MAP_SIZE_X/2,MAP_SIZE_Y/4) < 3)
				has_lower_ID = 0;
		}
		yellow_tester = yellow_tester->next;
	}
	return has_lower_ID;
}

int has_lower_ID(rencontre* tested)
//Teste si le chien à l'ID le plus bas
{
	int has_lower_ID = 1;

	if(tested == NULL)
		return has_lower_ID;

	if(((int)tested->ID) < ((int)dog->ID))
		has_lower_ID = 0;

	return has_lower_ID;
}


coord follow_blue_dog(rencontre* voisins, rencontre* blue_radar)
//Renvoie les coordonnées à suivre pour atteindre la brebis indiquée par le chien bleu
{
	coordF sheep_direction;
	coord reach_point;
	if(!has_lower_ID_center(voisins))
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
