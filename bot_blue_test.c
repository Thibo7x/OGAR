#include "bot_blue.h"
#include "entities.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#define MAP_SIZE_X 10800
#define MAP_SIZE_Y 10800

int distance(int coordX1, int coordY1, int coordX2, int coordY2)
{
	return sqrt((coordX2-coordX1)*(coordX2-coordX1)+(coordY2-coordY1)*(coordY2-coordY1));
}

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

int get_rank_with_axes(int column , int line)
{
  int rank = (line-1)*map.column + column;
  return rank;
}

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

// void test_split(void)
// {
//   split();
//   printf("longueur : %f (%d de long)\nlargeur : %f (%d de large)\n",map.long_length,map.column,map.large_length,map.line);
// }
//
//Peut permettre de tester get_rank_with_axes
// void test_get_center(void)
// {
//   coordF* center;
//   center = malloc(sizeof(coordF));
//
//   int column;
//   int line;
//   printf("Quelle colonne ? Quelle ligne ?\n");
//   scanf("%d %d",&column,&line);
//
//   int rank = get_rank_with_axes(column,line);
//   int rank_max = map.column*map.line;
//   get_center(center,column,line);
//
//   if (rank == 1)
//     printf("Coordoonées du centre du 1er carrée en : (%f,%f)\n",center->X,center->Y);
//   else if (rank > 1 && rank <= rank_max )
//     printf("Coordoonées du centre du %d-ème carrée en  : (%f,%f)\n",rank,center->X,center->Y);
//   else
//     printf("Hors grille , c'est le %d-ème carrée\n",rank);
//
//   free(center);
// }
//
// void test_get_rank_with_center_coos(void)
// {
//   coordF* center;
//   center = malloc(sizeof(coordF));
//   split();
//
//   int column;
//   int line;
//   printf("Quelle colonne ? Quelle ligne ?\n");
//   scanf("%d %d",&column,&line);
//
//   get_center(center,column,line);
//
//   int rank_with_column = get_rank_with_axes(column,line);
//   int rank_with_coos = get_rank_with_center_coos(center);
//   printf("Le centre de coordonnées :%fx%f correspond à la %d-ème case\n",center->X,center->Y,rank_with_coos);
//   printf("Alors que avec les colonnes/lignes on obtien %d-ème case\n",rank_with_column);
//
//   free(center);
// }
//
// void test_get_rank_with_any_coos(void)
// {
//   coordF* center;
//   center = malloc(sizeof(coordF));
// 	coord* axes;
// 	axes = malloc(sizeof(coord));
//
// 	int buffer;
// 	int rank_max = map.column*map.line;
//
//
// 	for (int rank = 1 ; rank <= rank_max ; ++rank )
//   	{
// 			get_axes_with_rank(rank,axes);
// 			get_center(center,axes->X,axes->Y);
// 			buffer = get_rank_with_any_coos(*center);
// 			printf("Pour la case %d on obtient la case %d.\n",rank,buffer);
// 		}
// 		free(center);
// 		free(axes);
// }
//
// void test_get_min(void)
// {
//   int tableau[20]={};
//   for (int depth = 0 ; depth < 10 ; ++depth)
//   {
//     tableau[depth] = 19-depth;
//   }
//   int min = get_min(tableau,20);
//   printf("Le minimum est : %d\n",min);
// }
//
// void test_get_axes_with_rank(void)
// {
//   coord* proposition;
//   proposition = malloc(sizeof(coord));
//
//   int rank;
// 	int rank_max = map.column*map.line;
//   //printf("Quelle rank ?\n");
//   //scanf("%d",&rank);
// 	for ( rank = 1 ; rank <= rank_max+1 ; ++rank)
// 	{
// 		get_axes_with_rank(rank,proposition);
// 		printf("Rank %d correspond à la colonne %d et la ligne %d\n",rank,proposition->X,proposition->Y);
// 	}
//   free(proposition);
// }
//
// void test_generate_reversed_c_way_from_top(void)
// {
//   int table[18];
//   *(table) = 1;
//   int rank = generate_reversed_c_way_from_top(6,6,1,table);
//   for(int length = 0;length < 18;++length)
//     {
//       printf("Valeur dans le tableau[%d] = %d\n",length,table[length]);
//     }
// }
//
// void test_generate_reversed_c_way_from_bottom(void);
// {
//   int table[18];
//   *(table) = 31;
//   int rank = generate_reversed_c_way_from_top(6,6,31,table);
//   for(int length = 0;length < 18;++length)
//     {
//       printf("Valeur dans le tableau[%d] = %d\n",length,table[length]);
//     }
//   return 1;
// }
//
void test_generate_new_base(void)
{
  int* table;
  table = malloc(36*sizeof(int));
  table[0] = 1;

  generate_new_base(table);
  for (int depth = 0 ; depth < 37 ; ++depth)
    {
      printf("Valeur du tableau[%d] = %d\n",depth ,table[depth]);
    }
  free(table);
}
//void test_checkpoint(void)
// {
//   coord point;
//   point.X = 0;
//   point.Y = 0;
//   coordF target;
//   target.X = 1.0;
//   target.Y = 1.0;
//
//   int range = distance(point.X , point.Y , (int)target.X , (int)target.Y);
//   char* feedback[] = {"Distance >= 3\n","Distance < 3\n"};
//
//   printf("La distance entre les points (%d,%d) et (%d,%d) est de %d.\n",point.X,point.Y,(int)target.X,(int)target.Y,range);
//   printf("Checkpoint renvoie : %s",*(feedback+checkpoint(point,target)));
//   return 1;
// }

int main(int argc,char* argv[])
{
  split();
  test_generate_new_base();
  return 1;
}
