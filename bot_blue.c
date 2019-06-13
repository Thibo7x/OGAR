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
      distance_table[rank-1] = distance((unsigned int)proposition->X,(unsigned int)proposition->Y,(unsigned int)position.X,(unsigned int)position.Y);
    }

  minimum = get_min(distance_table,rank_max);
  for(int rank = 1; rank <= rank_max; ++rank)
    {
      if (distance_table[rank-1] == minimum)
        {
          rank_propose[depth] = rank;
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
  int min = table[0];
  for (int rank = 1; rank < rank_max; ++rank)
  {
    if (min >= table[rank])
      min = table[rank];
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

//genère un chemin , un grand C exterieur partant du haut, puis des petits c les uns au dessus des autres. isOk;
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

//renvoie l'indice d'une case de réference en fonction du cadran dans lequel se situe le chien, priorité à droite et en bas
int first_rank(void)
{
  int references[4] = {19,17,14,16};
	//printf("Ref 1 : %d\nRef 2 : %d\nRef 3 : %d\nRef 4 : %d\n",references[0],references[1],references[2],references[3]);
	int x = dog->coord.X;
	int y = dog->coord.Y;
  int buffer;
		if ( x >= (MAP_SIZE_X/2.0) )
		{
			if ( y > (MAP_SIZE_Y/2.0))
				buffer = references[3];
			if ( y <= (MAP_SIZE_Y/2.0))
				buffer = references[1];
		}
		if ( x < (MAP_SIZE_X/2) )
		{
			if ( y > (MAP_SIZE_Y/2.0))
				buffer = references[2];
			if ( y <= (MAP_SIZE_Y/2.0))
				buffer = references[0];
		}
    printf("First_Rank = %d\n",buffer);
    return buffer;
}
/* ---------------------------------*/
coord spotting()
{
  coord target;
  coordF targetF;
  coord *proposition = malloc(sizeof(proposition));
  //attention , les tableau commencent à l'indice 0 , rank commence à l'indice 1.
  get_axes_with_rank(table[old_rank-1], proposition);
  coordF center;
  get_center(&center,proposition->X,proposition->Y);
  if(checkpoint(dog->coord,center))
  {
    old_rank++;
    if (old_rank > max_rank)
    {
      old_rank = 1;
    }
    get_axes_with_rank(table[old_rank-1], proposition);
    get_center(&targetF,proposition->X,proposition->Y);
  }
  else
  {
    get_axes_with_rank(table[old_rank-1], proposition);
    get_center(&targetF,proposition->X,proposition->Y);
  }
  target.X = (int)(targetF.X);
  target.Y = (int)(targetF.Y);
  printf("Les coordonnées cibles sont : (%d,%d)\n",target.X,target.Y);
  free(proposition);
  return target;
}



/* ---------------------------------*/
void save_our_sheeps(rencontre *voisins)
{
  rencontre *pointer = voisins;

  while (pointer != NULL)
  {
    if ((rechercherListeChainee(saved_sheeps, pointer->ID) == NULL) && (!memcmp(pointer->couleur,"\xe6\xf0\xf0",3))) // Pas trouvé dans saved_sheeps
    {
      if(distance(0, MAP_SIZE_Y/2, pointer->coord.X, pointer->coord.Y) > MAP_SIZE_X/10)
      {
        // On l'ajoute
        rencontre *sheep = malloc(sizeof(sheep));

        // Remplissage des caractéristiques
        sheep->ID = pointer->ID;
        sheep->coord = pointer->coord;
        memcpy(sheep->couleur, pointer->couleur, 3*sizeof(unsigned char));

        // Adresse
        sheep->next = saved_sheeps;
        saved_sheeps = sheep;

      }

    }
    pointer = pointer->next;
  }
}

//renvoie 0 si la distance entre 2 points dépasse 3, renvoie 1 sinon , isOk
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
//Carrément toute la fonction intel_blue
coord intel_blue(rencontre *voisins)
{
  printf("MODE: %d\n", dog->mode);
  save_our_sheeps(voisins); // MAJ sheeps around
  coord obj;
  //printf("MODE : %d\n",dog->mode );
  switch (dog->mode) {
    case 0:
      //Ordre
      obj = spotting();
      //Sortie
      if(count_sheeps() >= 4)
      dog->mode = 1;
    break;

    case 1:
      //Ordre
      obj.X = MAP_SIZE_X/2;
      obj.Y = MAP_SIZE_Y/2;
      //Sortie
      if(distance(dog->coord.X,dog->coord.Y,MAP_SIZE_X/2,MAP_SIZE_Y/2) < 2)
      {
        if(find_voisin_by_color((unsigned char *)"\xff\xff\x0",voisins) != NULL && distance(MAP_SIZE_X/2,MAP_SIZE_Y/2,find_voisin_by_color((unsigned char *)"\xff\xff\x0",voisins)->coord.X,find_voisin_by_color((unsigned char *)"\xff\xff\x0",voisins)->coord.Y) < 2)
          dog->mode = 2;
      }
    break;

    case 2:
      //Ordre
      obj = saved_sheeps->coord;
      //Sorties
      if(distance(MAP_SIZE_X/2,MAP_SIZE_Y/2,dog->coord.X,dog->coord.Y) >= 300)
      {
        deleteChainedList(saved_sheeps,saved_sheeps->ID);
        if(count_sheeps() == 0)
        {
          dog->mode = 0;
        }
        else
        {
          dog->mode = 1;
        }
      }
    break;

    default:
      dog->mode = 0;
    break;


  }
  return obj;
  //   case 0:
  //   //Ordre
  //     obj = spotting();
  //   //Sortie
  //     if(count_sheeps() >= 4)
  //     dog->mode = 2;
  //   break;
  //
  //   case 2:
  //   //Ordre
  //     obj.X = dog->coord.X;
  //     if(dog->coord.Y < MAP_SIZE_Y/2 - 160)
  //       obj.Y = MAP_SIZE_Y/2 - 160;
  //     if(dog->coord.Y > MAP_SIZE_Y/2 + 160)
  //       obj.Y = MAP_SIZE_Y/2 + 160;
  //
  //     backup_done = 0;
  //   //Sortie
  //     if((dog->coord.Y == MAP_SIZE_Y/2 - 160) || (dog->coord.Y == MAP_SIZE_Y/2 + 160))
  //       dog->mode = 3;
  //   break;
  //   case 3:
  //   //Ordre
  //     obj.X = turn_to_indicate(saved_sheeps).X;
  //     obj.Y = dog->coord.Y;
  //   //Sortie
  //     if((dog->coord.X >= obj.X -1) || (dog->coord.X <= obj.X +1))
  //       dog->mode = 4;
  //   break;
  //
  //   case 4:
  //   //Ordre
  //     obj.X = dog->coord.X;
  //     obj.Y = turn_to_indicate(saved_sheeps).Y;
  //   //Sortie
  //     if(dog->coord.Y == obj.Y)
  //     dog->mode = 5;
  //   break;
  //
  //   case 5:
  //   //Ordre
  //     if(!backup_done)
  //     {
  //       backup = dog->coord;
  //       backup_done = 1;
  //     }
  //     obj.X = MAP_SIZE_X/2;
  //     obj.Y = MAP_SIZE_Y/2;
  //   //Sortie
  //     if((count_sheeps() > 0) && (distance(dog->coord.X,dog->coord.Y,MAP_SIZE_X/2,MAP_SIZE_Y/2) < 90))
  //     {
  //       if(find_voisin_by_color((unsigned char *)"\xff\xff\x0",voisins) != NULL && distance(MAP_SIZE_X/2,MAP_SIZE_Y/2,find_voisin_by_color((unsigned char *)"\xff\xff\x0",voisins)->coord.X,find_voisin_by_color((unsigned char *)"\xff\xff\x0",voisins)->coord.Y) < 2)
  //       {
  //         deleteChainedList(saved_sheeps,saved_sheeps->ID);
  //         dog->mode = 6;
  //       }
  //     }
  //     if(count_sheeps() == 0)
  //     {
  //       dog->mode = 0;
  //     }
  //   break;
  //
  //   case 6:
  //   //Ordre
  //     obj = backup;
  //   //Sortie
  //   if(distance(dog->coord.X,dog->coord.Y,MAP_SIZE_X/2,MAP_SIZE_Y/2) >= 145)
  //     dog->mode = 2;
  //   break;
  //
  //   default:
  //   break;
  //
  //
  // }
  // return obj;

}
/* ----------------main----------------- */

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
