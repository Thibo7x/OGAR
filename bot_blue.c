#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>
#include <signal.h>
#include <syslog.h>
#include <sys/time.h>
#include <unistd.h>
#include <math.h>

/* ----------------.h----------------- */
#define MAP_SIZE_X 9000.0
#define MAP_SIZE_Y 6000.0
typedef struct coordF {
  float X;
  float Y;
} coordF;

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
int get_rank_with_coos(coordF* center,subdivision* map);

//tests
void test_split(void);
void test_get_center(void);
void test_get_rank_with_coos(void);
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
int get_rank_with_coos(coordF* center,subdivision* map)
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


int main(int argc,char* argv[])
{
  return 1;
}

/* ----------------tests.c----------------- */
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

void test_get_rank_with_coos(void)
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
  int rank_with_coos = get_rank_with_coos(center,map);
  printf("Le centre de coordonnées :%fx%f correspond à la %d-ème case\n",center->X,center->Y,rank_with_coos);
  printf("Alors que avec les colonnes/lignes on obtien %d-ème case\n",rank_with_column);

  free(center);
  free(map);
}
/* ----------------tests.c----------------- */
