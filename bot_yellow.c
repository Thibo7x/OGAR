#include "bot_yellow.h"
#include "entities.h"
#include "mapFunctions.h"
#include "chainedListFunctions.h"
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#define min(a,b) (a<=b?a:b)
#define max(a,b) (((a)>(b))?(a):(b))

//Ne pas éviter les moutons jaunes qui ne sont pas à nous
coord intel_yellow(rencontre *voisins)
{
	rencontre *sheep_viseur = voisins;
	rencontre *blue_viseur = voisins;
	rencontre *yellow_viseur = voisins;
	rencontre *sheep_vise;
	coordF random_direction;
	coord obj;
	srand(time(NULL));
	int radius;

	sheep_viseur = find_voisin_by_color((unsigned char *)"\xe6\xf0\xf0", voisins);
	blue_viseur = find_voisin_by_color((unsigned char *)"\x0\x0\xff",voisins);
	yellow_viseur = find_voisin_by_color((unsigned char *)"\xff\xff\x0",voisins);
	//printf("MODE : %d\n", dog->mode);
	if(((distance(dog->coord.X,dog->coord.Y, CENTER_PEN_X,CENTER_PEN_Y) <= MAP_SIZE_X/10 || distance(dog->coord.X,dog->coord.Y, MAP_SIZE_X-CENTER_PEN_X,CENTER_PEN_Y) <= MAP_SIZE_X/10) && mode == 2) || ((distance(dog->coord.X,dog->coord.Y, CENTER_PEN_X,CENTER_PEN_Y) <= MAP_SIZE_X/10) && mode == 1))
	{
		dog->mode = 0;
		obj.X = MAP_SIZE_X/2;
		obj.Y = MAP_SIZE_Y/2;
		return obj;
	}
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
			if((blue_viseur != NULL) && distance(dog->coord.X,dog->coord.Y,MAP_SIZE_X/2,MAP_SIZE_Y/2) < 3 && has_lower_ID_center(voisins))
			{
				dog->mode = 3;
				break;
			}
			if(three_in_the_center(voisins) && (dog->coord.X == MAP_SIZE_X/2) && (dog->coord.Y == MAP_SIZE_Y/2) && has_lower_ID_center(voisins))
				dog->mode = 3;
			// if(has_lower_ID_center(voisins) && three_in_the_center(voisins) && distance(dog->coord.X,dog->coord.Y,MAP_SIZE_X/2,MAP_SIZE_Y/2) < 3)
			// {
			// 	dog->mode = 4;
			// 	random_direction();
			// }
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
			if(distance(dog->coord.X, dog->coord.Y, CENTER_PEN_X, CENTER_PEN_Y) <= MAP_SIZE_X/10 || sheep_viseur == NULL || distance(dog->coord.X, dog->coord.Y, MAP_SIZE_X-CENTER_PEN_X, CENTER_PEN_Y) <= MAP_SIZE_X/10 )
			{
				dog->mode = 0;
			}
			if(sheep_viseur != NULL)
			{
				while(yellow_viseur != NULL)
				{
					if(distance(yellow_viseur->coord.X,yellow_viseur->coord.Y,sheep_viseur->coord.X,sheep_viseur->coord.Y) < distance(dog->coord.X,dog->coord.Y,sheep_viseur->coord.X,sheep_viseur->coord.Y ))
					{
						dog->mode = 0;
						obj.X = MAP_SIZE_X/2;
						obj.Y = MAP_SIZE_Y/2;
						break;
					}
					yellow_viseur = yellow_viseur->next;
				}
			}
			if(iii == 0.25*dog->R_action)
			{
				iii = 0;
				dog->mode = 2;
			}
		break;

		case 2:
			//Ordre
			if((sheep_viseur != NULL) && yellow_viseur != NULL)
			{
				if(distance(yellow_viseur->coord.X,yellow_viseur->coord.Y,sheep_viseur->coord.X,sheep_viseur->coord.Y) < distance(dog->coord.X,dog->coord.Y,sheep_viseur->coord.X,sheep_viseur->coord.Y ) )
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
			if (radius <= 20) // Il pousse le mouton s'il est bien positionné
			{
				dog->mode = 1;
			}

		break;

		case 3:
			//Ordre
			if(blue_viseur != NULL)
			{
 			 	obj = blue_viseur->coord;
			}
			else if(three_in_the_center(voisins) && (dog->coord.X == MAP_SIZE_X/2) && (dog->coord.Y == MAP_SIZE_Y/2) && has_lower_ID_center(voisins))
			{
				if(mode == 1)
				{
					random_direction.X = (float)(rand() % 100000) / 100000.0 * pow(-1,(int)rand()%2);
					random_direction.Y = (float)(rand() % 100000) / 100000.0 * pow(-1,(int)rand()%2);
					obj.X = round(MAP_SIZE_X/2.0 + (random_direction.X)*200.0);
					obj.Y = round(MAP_SIZE_Y/2.0 + (random_direction.Y)*200.0);
				}
				if(mode == 2)
				{
					if(CENTER_PEN_X == 0.0) obj.X = 9000 - 0.5*dog->R_action;
				  if(CENTER_PEN_X == 9000.0) obj.X = 0.5*dog->R_action;
					if(rand()%2)
						obj.Y = CENTER_PEN_Y - 1150;
					else
						obj.Y = CENTER_PEN_Y + 1150;
				}
			}
			else obj = follow_blue_dog(voisins);
			//Sorties
			if((distance(dog->coord.X,dog->coord.Y,MAP_SIZE_X/2,MAP_SIZE_Y/2) >= 200) || (sheep_viseur != NULL))
				dog->mode = 4;
		break;

		case 4:
			//Ordre
			obj = follow_blue_dog(voisins);
			if(sheep_viseur != NULL)
			{
				while(yellow_viseur != NULL)
				{
					if(distance(yellow_viseur->coord.X,yellow_viseur->coord.Y,sheep_viseur->coord.X,sheep_viseur->coord.Y) < distance(dog->coord.X,dog->coord.Y,sheep_viseur->coord.X,sheep_viseur->coord.Y ) )
					{
						obj.X = MAP_SIZE_X/2;
						obj.Y = MAP_SIZE_Y/2;
						dog->mode = 0;
						break;
					}
					yellow_viseur = yellow_viseur->next;
				}
			}
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


coord follow_blue_dog(rencontre* voisins)
//Renvoie les coordonnées à suivre pour atteindre la brebis indiquée par le chien bleu
{
	coordF sheep_direction;
	coord reach_point;
	sheep_direction = direction(MAP_SIZE_X/2,MAP_SIZE_Y/2,dog->coord.X,dog->coord.Y);
	reach_point.X = round(dog->coord.X + (sheep_direction.X)*50);
	reach_point.Y = round(dog->coord.Y + (sheep_direction.Y)*50);
	if (reach_point.X>MAP_SIZE_X)
		reach_point.X = MAP_SIZE_X;
	if (reach_point.X<0)
		reach_point.X = 0;
	if (reach_point.Y>MAP_SIZE_Y)
		reach_point.Y = MAP_SIZE_Y;
	if (reach_point.Y<0)
		reach_point.Y = 0;
	return reach_point;
}

int has_lower_ID_center(rencontre* voisins)
{
	int has_lower_ID = 1;
	rencontre* test = voisins;
	while(test != NULL)
	{
		if(!memcmp(test->couleur,"\xff\xff\x0",3))
		{
			if(test->ID < dog->ID)
				return 0;
		}
		test = test->next;
	}
	return has_lower_ID;
}

int three_in_the_center(rencontre* voisins)
{
	rencontre* yellow = voisins;
	int yellow_counter = 1;
	while(yellow != NULL)
	{
		if(!memcmp(yellow->couleur,"\xff\xff\x0",3) && !strcmp(dog->nickname, yellow->nickname))
		{
				yellow_counter++;
		}
		yellow = yellow->next;
	}
	if(yellow_counter > 1)
		return 1;
	return 0;
}
