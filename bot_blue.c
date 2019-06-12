#include "bot_blue.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "mapFunctions.h"
#include "chainedListFunctions.h"
//obtenir les dimensions d'une case et le nombre selon la longueur/largeur , is Ok
void split(void)
{
  map.long_length = 0.0;
  map.column = 0;
  map.large_length = 0.0;
  map.line = 0;
  //On étudie sur un demi terrain , pour s'assurer de ne jamais avoir un bandeau centré sur le milieu du terrain
  float long_max = MAP_SIZE_X;
  float large_max = MAP_SIZE_Y/2;
  float number = 1.0;

  //on cherche à avoir le minimum de carrées , d'une taille max de 2000

  while ((large_max/number > 2000.0 || long_max/number > 2000.0) || (map.long_length == 0.0 || map.large_length == 0.0))
  {
    if (large_max/number < 2000.0 && map.large_length == 0.0)
      {
        map.large_length = large_max/number;
        map.line = 2*(int)number;
      }
    else if (long_max/number < 2000.0 && map.long_length == 0.0)
      {
        map.long_length = long_max/number;
        map.column = (int)number;
      }
    number++;
  }
}

//obtenir le rank de la case en fonction de sa colonne/ligne  , is Ok
int get_rank_with_axes(int column , int line)
{
  int rank = (line-1)*map.column + column;
  return rank;
}


//obtenir le centre d'un rectangle en fonction de sa position sur le cadrillage ; is Ok
void get_center(coordF* center, int column , int line)
{
  int rank = get_rank_with_axes(column,line);
  int rank_max = map.column*map.line;
  if (1 <= rank && rank <= rank_max)
    {
      center->X = ((float)(column-1)*map.long_length)+map.long_length/2.0;
      center->Y = ((float)(line-1)*map.large_length)+map.large_length/2.0;
    }
  else
    perror("Out of grill");
}

//obtenir le rang d'une case à partir des coordonées de son centre , is Ok (inutilisé)
int get_rank_with_center_coos(coordF* center)
{
  coordF* proposition;
  proposition = malloc(sizeof(coordF));

  int column_out = 0;
  int line_out = 0;

      for (int line = 1 ; line <= map.line ; ++line)
      {
        for (int column = 1 ; column <= map.column ; ++column)
        {
          get_center(proposition,column,line);
          if (proposition->X == center->X)
            column_out = column;
          if (proposition->Y == center->Y)
            line_out = line;
        }
      }

  int rank = get_rank_with_axes(column_out,line_out);
  free(proposition);
  return rank;
}


//obtenir le rang d'une case à partir de ses coordonées , is Ok
int get_rank_with_any_coos(coordF position)
{
  coordF* proposition;
  proposition = malloc(sizeof(coordF));

  coord* rank_properties;
  rank_properties = malloc(sizeof(coord));

  int rank_max = map.column*map.line;

  int* distance_table;
  distance_table = malloc(rank_max*sizeof(int));

  int minimum;
  int depth = 0;
  //cas critique : si on appel la fonction lorsque le point se trouve pile entre 2 cases ou entre 4 cases
  int rank_propose[4] = {rank_max,rank_max,rank_max,rank_max};

  //on enregistre l'ensemble des distances du point donné par rapport aux différents centres
  for(int rank = 1; rank <= rank_max; ++rank)
    {
      get_axes_with_rank(rank,rank_properties);
			get_center(proposition,rank_properties->X,rank_properties->Y);
      *(distance_table+rank-1) = distance((int)proposition->X,(int)proposition->Y,(int)position.X,(int)position.Y);
    }

  minimum = get_min(distance_table,rank_max);
  for(int rank = 1; rank <= rank_max; ++rank)
    {
      if (distance_table[rank-1] == minimum)
        {
          *(rank_propose+depth) = rank;
          depth++;
        }
    }
  free(rank_properties);
  free(proposition);
  free (distance_table);
  return rank_propose[0];
}


//obtenir la valeur min d'un tableau , is Ok
int get_min(int* table , int rank_max)
{
  int min = *(table);
  for (int rank = 1; rank <= rank_max; ++rank)
  {
    if (min >= *(table+rank))
      min = *(table+rank);
  }
  return min;
}

//permet d'obtenir la colonne et la ligne en fonction du numéro de la case , is Ok
void get_axes_with_rank(int rank,coord* proposition)
{
  if (rank > map.column*map.line)
	{
			perror("Out of range , rank too high");
	}
	else
		{
			proposition->X = rank%map.column;
  		proposition->Y = (rank/map.column)+1;

  		if (proposition->X == 0)
    		proposition->X = map.column;
  		if (rank%map.column == 0)
    		proposition->Y -= 1;
		}
}

void generate_new_base(int* order)
{
    int rank_max = map.column*map.line;
    int big_reversed_C_size = (2*map.column)+map.line-2;
    int little_reversed_C_size = 2*(map.column-1);
    int lower_counter = 0;
    int coefficient = 0;
    //la première case vaux 1.
    order[0] = 1 ;
    for (int rank = 1 ; rank < rank_max ; ++rank)
      {
          //cas du premier C , anciennement reversed_C_from_top , is Ok
          if (rank < big_reversed_C_size)
            {
              if (rank < map.column)
                order[rank] = order[rank-1]+1;

              if (rank >= map.column && rank < map.column+map.line-1)
                order[rank] = order[rank-1]+map.column;

              if (rank >= map.column+map.line-1)
                order[rank] = order[rank-1]-1;
            } // Premier IF
          //étape de transition
          if (rank == big_reversed_C_size)
            {
              order[rank] = order[rank-1]-map.column;
            }//Deuxième IF

          //cas des petits C succesifs
          if (rank > big_reversed_C_size)
            {
              lower_counter = rank-big_reversed_C_size-(coefficient*little_reversed_C_size);

              if (lower_counter < map.column-1)
                  order[rank] = order[rank-1]+1;

              if (lower_counter == map.column-1)
                order[rank] = order[rank-1]-map.column;

              if (lower_counter > map.column-1 && lower_counter < little_reversed_C_size)
                  order[rank] = order[rank-1]-1;

							if (lower_counter == little_reversed_C_size)
								{
									order[rank] = order[rank-1]-map.column;
									coefficient++;
								}
             }
      }
}


coord spotting()
{
  split();
  int *order = malloc(map.column * map.line * sizeof(int));
  generate_new_base(order);
  coord *position = malloc(sizeof(position));
  coordF *centreF = malloc(sizeof(centreF));
  coord centre;
  get_axes_with_rank(order[old_rank++],position);
  get_center(centreF, position->X , position->Y);
  centre.X = ceil(centreF->X);
  centre.Y = ceil(centreF->Y);
  free(position);
  free(centreF);
  free(order);
  return centre;
}


void save_our_sheeps(rencontre *voisins)
{
  rencontre *pointer = voisins;
  if (pointer != NULL)
  {
    while (pointer != NULL)
    {
      if ((rechercherListeChainee(saved_sheeps, pointer->ID) == NULL) && (!memcmp(pointer->couleur,"\xe6\xf0\xf0",3))) // Pas trouvé dans saved_sheeps
      {
        // On l'ajoute
        rencontre *sheep = malloc(sizeof(sheep));

        // Remplissage des caractéristiques
        sheep->ID = pointer->ID;
        sheep->coord = pointer->coord;
        memcpy(sheep->couleur, pointer->couleur, 4*sizeof(unsigned char));

        // Adresse
        sheep->next = voisins;
        voisins = sheep;

      }
      pointer = pointer->next;
    }
  }

}

//renvoie 1 si la distance entre 2 points dépasse 3, renvoie 0 sinon , isOk
int checkpoint(coord point,coordF target)
{
  int radius = distance(point.X,point.Y,(int)(target.X),(int)(target.Y));
  if (radius < 3)
    return 1;
  else
    return 0;
}


int count_sheeps()
{
  int i;
  i = count_voisins(saved_sheeps);
  return i;
}
/* ----------------main----------------- */

//Reste à faire :
//Fonction spotting()
//dog->mode 6
//Libérer la mémoire quand un mouton est indiqué
coord intel_blue(rencontre *voisins)
{
  save_our_sheeps(voisins); // MAJ sheeps around
  coord obj;

  switch (dog->mode) {
    case 0:
    //Ordre
      //obj = spotting();
    //Sortie
      if(count_sheeps() >= 4)
      dog->mode = 2;
    break;

    case 2:
    //Ordre
      obj.X = dog->coord.X;
      if(dog->coord.Y > MAP_SIZE_Y - 150)
        obj.Y = MAP_SIZE_Y - 150;
        if(dog->coord.Y > MAP_SIZE_Y + 150)
        obj.Y = MAP_SIZE_Y + 150;
    //Sortie
      if((dog->coord.Y == MAP_SIZE_Y/2 - 150) || (dog->coord.Y == MAP_SIZE_Y/2 + 150))
        dog->mode = 3;
    break;
    case 3:
    //Ordre
      obj.X = turn_to_indicate(saved_sheeps).X;
      obj.Y = dog->coord.Y;
    //Sortie
      if(dog->coord.X == obj.X)
        dog->mode = 4;
    break;

    case 4:
    //Ordre
      obj.X = dog->coord.X;
      obj.Y = turn_to_indicate(saved_sheeps).Y;
    //Sortie
      if(dog->coord.Y == obj.Y)
      dog->mode = 5;
    break;

    case 5:
    //Ordre
      backup = dog->coord;
      obj.X = MAP_SIZE_X/2;
      obj.Y = MAP_SIZE_Y/2;
    //Sortie
      if((count_sheeps() > 0) && (distance(dog->coord.X,dog->coord.Y,MAP_SIZE_X/2,MAP_SIZE_Y/2) < 90) && find_voisin_by_color((unsigned char *)"\xff\xff\x0",voisins))
      {
        deleteChainedList(saved_sheeps,saved_sheeps->ID);
        dog->mode = 6;
      }
      if(count_sheeps() == 0)
      {
        dog->mode = 0;
      }
    break;

    case 6:
    //Ordre
      obj = backup;
    //Sortie
    if(distance(dog->coord.X,dog->coord.Y,MAP_SIZE_X/2,MAP_SIZE_Y/2) >= 145)
      dog->mode = 2;
    break;

    default:
    break;


  }
  return obj;

}
/* ----------------main----------------- */
//rallier le centre d'un carreau en fonction du numéro de case donné , à finir , à tester
// coord join_center(int rank,coordF center)
// {
//
//   int rank = get_rank_with_any_coos(center);
//
// }
//renvoie 1 si les coordonnées sont à - de 3 de distance de la target

/* ----------Work in progress----------- */


coord turn_to_indicate(rencontre *sheep)
//Indique les coordonnées à atteindre pour indiquer le mouton
{
  coordF sheep_direction = direction(MAP_SIZE_X/2,MAP_SIZE_Y/2,sheep->coord.X,sheep->coord.Y);
  coord reach_point;
  reach_point.X = MAP_SIZE_X/2 + ceil((sheep_direction.X)*150);
	reach_point.Y = MAP_SIZE_Y/2 + ceil((sheep_direction.Y)*150);
  return reach_point;
}
