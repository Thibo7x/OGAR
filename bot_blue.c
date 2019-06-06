#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>
#include <signal.h>
#include <syslog.h>
#include <sys/time.h>
#include <unistd.h>
#include <math.h>

int distance(int coordX1, int coordY1, int coordX2, int coordY2);
int distance(int coordX1, int coordY1, int coordX2, int coordY2)
{
	return sqrt((coordX2-coordX1)*(coordX2-coordX1)+(coordY2-coordY1)*(coordY2-coordY1));
}
typedef struct coord {
  int X;
  int Y;
} coord;

typedef struct coordF {
  float X;
  float Y;
} coordF;
/* ----------------.h----------------- */
#define MAP_SIZE_X 9000.0
#define MAP_SIZE_Y 6000.0


typedef struct rectangle_subdivision subdivision;
struct rectangle_subdivision
{
  float long_length ;
  int column ;
  float large_length ;
  int line;
};

void split(subdivision* map);
void get_center(coordF* center,subdivision* map,int column , int line);
int get_rank_with_column_line(int column , int line , subdivision* map);
int get_rank_with_center_coos(coordF* center,subdivision* map);
void get_column_line_with_rank(int rank,subdivision* map,coord* proposition);
//tests
void test_split(void);
void test_get_center(void);
void test_get_rank_with_center_coos(void);
void test_get_min(void);
void test_get_column_line_with_rank(void);
/* ----------------.h----------------- */
//obtenir les dimensions
void split(subdivision* map)
{
  //On étudie sur un demi terrain , pour s'assurer de ne jamais avoir un bandeau centré sur le milieu du terrain
  float long_max = MAP_SIZE_X;
  float large_max = MAP_SIZE_Y/2;
  float number = 1.0;
  map->long_length = 0.0;
  map->large_length = 0.0;
  //on cherche à avoir le minimum de carrées , d'une taille max de 2000

  while ((large_max/number > 2000.0 || long_max/number > 2000.0) || (map->long_length == 0.0 || map->large_length == 0.0))
  {
    if (large_max/number < 2000 && map->large_length == 0.0)
      {
        map->large_length = large_max/number;
        map->line = 2*(int)number;
      }
    else if (long_max/number < 2000.0 && map->long_length == 0.0)
      {
        map->long_length = long_max/number;
        map->column = (int)number;
      }
    number++;
  }
}
//obtenir le rank de la case en fonction de sa colonne/ligne
int get_rank_with_column_line(int column , int line , subdivision* map)
{
  split(map);
  int rank = (line-1)*map->column + column;

  return rank;
}

//obtenir le centre d'un rectangle en fonction de sa position sur le cadrillage
void get_center(coordF* center,subdivision* map,int column , int line)
{
  int rank = get_rank_with_column_line(column,line,map);
  int rank_max = map->column*map->line;
  if (1 <= rank && rank <= rank_max)
    {
      center->X = ((float)(column-1)*map->long_length)+map->long_length/2.0;
      center->Y = ((float)(line-1)*map->large_length)+map->large_length/2.0;
    }
  else
    perror("Out of grill");
}
//obtenir le rang d'une case à partir des coordonées de son centre
int get_rank_with_center_coos(coordF* center,subdivision* map)
{
  coordF* proposition;
  proposition = malloc(sizeof(coordF));

  int column_out = 0;
  int line_out = 0;


      for (int line = 1 ; line <= map->line ; ++line)
      {
        for (int column = 1 ; column <= map->column ; ++column)
        {
          get_center(proposition,map,column,line);
          if (proposition->Y == center->Y)
            column_out = column;
          if (proposition->X == center->X)
            line_out = line;
        }
      }

  free(proposition);
  int rank = get_rank_with_column_line(column_out,line_out,map);
  return rank;
}
//obtenir le rang d'une case à partir de ses coordonées , à finir, à tester
/*int get_rank_with_every_coos(coordF* coord,subdivision* map)
{
  coordF* proposition;
  proposition = malloc(sizeof(coordF));

  coord* rank_properties;
  rank_properties = malloc(sizeof(coord));

  split(map);


  int rank_max = map->column*map->line;

  int* distance_table;
  distance_table = malloc(rank_max*sizeof(int));

  int minimum;
  int depth = 0;
//cas critique : si on appel la fonction lorsque le point se trouve pile entre 2 cases ou entre 4 cases
  int rank_propose[4] = {rank_max,rank_max,rank_max,rank_max};

//on enregistre l'ensemble des distances du point donné par rapport aux différents centres
  for(int rank = 1; rank <= rank_max; ++rank)
    {
      get_column_line_with_rank(rank);
      get_center(proposition,map,rank_properties->X,rank_properties->Y);
      *(distance_table+rank-1) = distance((int)proposition->X,(int)proposition->Y,(int)coord->X,(int)coord->Y);
    }

  minimum = get_min(proposition,rank_max);
  for(int rank = 1; rank <= rank_max; ++rank)
    {
      if (distance_table(rank-1) == minimum)
        {
          *(rank_propose+depth) = rank;
          depth++;
        }
    }
  free(rank_properties);
  free(proposition);
  free (distance_table);
  return rank_propose[0];
}*/
//obtenir la valeur min d'un tableau
int get_min(int* table , int length_max)
{
  int min = *(table);
  for (int rank = 1; ranhk < length_max; ++rank)
  {
    if (min >= *(table+rank))
      min = *(table+rank);
  }
return min;
}

//à terminer , fonction de test déjà faites
void get_column_line_with_rank(int rank,subdivision* map,coord* proposition)
{
  split(map);
  proposition->X = rank%map->column;
  proposition->Y = (rank/map->column)+1;

  if (proposition->X == 0)
    proposition->X = map->column;
  if (proposition->Y == map->line+1 || proposition->Y == 0)
    proposition->Y = map->line;
}
/* ----------------tests.c----------------- */
int main(int argc,char* argv[])
{
  test_get_column_line_with_rank();
  return 1;
}

void test_split(void)
{
  subdivision* map;
	map = malloc(sizeof(subdivision));

  split(map);
  printf("longueur : %f (%d de long)\nlargeur : %f (%d de large)\n",map->long_length,map->column,map->large_length,map->line);

free(map);
}

void test_get_center(void)
{
//Peut permettre de tester get_rank_with_column_line
  coordF* center;
  center = malloc(sizeof(coordF));
  subdivision* map;
	map = malloc(sizeof(subdivision));

  int column;
  int line;
  printf("Quelle colonne ? Quelle ligne ?\n");
  scanf("%d %d",&column,&line);

  int rank = get_rank_with_column_line(column,line,map);
  int rank_max = map->column*map->line;
  get_center(center,map,column,line);

  if (rank == 1)
    printf("Coordoonées du centre du 1er carrée en : (%f,%f)\n",center->X,center->Y);
  else if (rank > 1 && rank <= rank_max )
    printf("Coordoonées du centre du %d-ème carrée en  : (%f,%f)\n",rank,center->X,center->Y);
  else
    printf("Hors grille , c'est le %d-ème carrée\n",rank);

  free(center);
  free(map);
}

void test_get_rank_with_center_coos(void)
{
  coordF* center;
  center = malloc(sizeof(coordF));
  subdivision* map;
  map = malloc(sizeof(subdivision));

  int column;
  int line;
  printf("Quelle colonne ? Quelle ligne ?\n");
  scanf("%d %d",&column,&line);

  get_center(center,map,column,line);

  int rank_with_column = get_rank_with_column_line(column,line,map);
  int rank_with_coos = get_rank_with_center_coos(center,map);
  printf("Le centre de coordonnées :%fx%f correspond à la %d-ème case\n",center->X,center->Y,rank_with_coos);
  printf("Alors que avec les colonnes/lignes on obtien %d-ème case\n",rank_with_column);

  free(center);
  free(map);
}

void test_get_rank_with_every_cooos(void)
{
  coordF* center;
  center = malloc(sizeof(coordF));
  subdivision* map;
  map = malloc(sizeof(subdivision));

  int column;
  int line;
  printf("Quelle colonne ? Quelle ligne ?\n");
  scanf("%d %d",&column,&line);

  get_center(center,map,column,line);

}

void test_get_min(void)
{
  int tableau[20]={};
  for (int depth = 0 ; depth < 10 ; ++depth)
  {
    tableau[depth] = 19-depth;
  }
  int min = get_min(tableau,20);
  printf("Le minimum est : %d\n",min);
}

void test_get_column_line_with_rank(void)
{
  coord* proposition;
  proposition = malloc(sizeof(coord));
  subdivision* map;
  map = malloc(sizeof(subdivision));

  split(map);
  int rank;

  printf("Quelle rank ?\n");
  scanf("%d",&rank);

  get_column_line_with_rank(rank,map,proposition);
  printf("Rank %d correspond à la colonne %d et la ligne %d\n",rank,proposition->X,proposition->Y);
}
/* ----------------tests.c----------------- */
//Pas mal de fonction a faire a l'intérieur
// void intel_blue(struct lws *wsi,rencontre *voisins)
// {
// 	int counter = 0;
// 	rencontre *sheepCounter = voisins;
// 	coord retour;
// 	while(sheepCounter != NULL && counter < 4) //Mettre le nombre de bots jaunes ici
// 	{
// 		//spotting(); Fonction de repérage
// 		if (!memcmp(sheepCounter->couleur,"\xe6\xf0\xf0",3)) //si c'est une brebis
// 			counter++;
// 		sheepCounter = sheepCounter->next;
// 	}
// 	if(counter == 4)
// 	{
// 		sheepCounter = voisins;
// 		moveBot(wsi,4500,3000);
// 		if(distance(dog->coord.X,dog->coord.Y,4500,3000) <= 150)
// 			moveBot(wsi,dog->coord.X,dog->coord.Y);
// 		while(counter != 0)
// 		{
// 			while(!memcmp(sheepCounter->couleur,"\xe6\xf0\xf0",3))
// 			{
// 				sheepCounter = sheepCounter->next;
// 			}
// 			counter--;
// 			//turn_to_indicate(sheepCounter->coord.X,sheepCounter->coord.Y);
// 			//Equation du cercle : (x-4500)²+(y-3000)² = 150²
// 			//Tourne autour du cercle pour être aligné au mouton à indiquer
// 			retour.X = dog->coord.X;
// 			retour.Y = dog->coord.Y;
// 			moveBot(wsi,4500,3000);
// 			if(distance(dog->coord.X,dog->coord.Y,4500,3000) <= 90)
// 				moveBot(wsi,retour.X,retour.Y);
// 		}
// 	}
//
// }
