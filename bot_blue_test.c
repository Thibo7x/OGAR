#include "bot_blue.h"

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

void test_get_rank_with_any_coos(void)
{
  coordF* center;
  center = malloc(sizeof(coordF));
  subdivision* map;
  map = malloc(sizeof(subdivision));
	coord* axes;
	axes = malloc(sizeof(coord));

	split(map);
	int buffer;
	int rank_max = map->column*map->line;


	for (int rank = 1 ; rank <= rank_max ; ++rank )
  	{
			get_column_line_with_rank(rank , map , axes);
			get_center(center,map,axes->X,axes->Y);
			buffer = get_rank_with_any_coos(center,map);
			printf("Pour la case %d on obtient la case %d.\n",rank,buffer);
		}
		free(center);
		free(map);
		free(axes);
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

void test_get_axes_with_rank(void)
{
  coord* proposition;
  proposition = malloc(sizeof(coord));
  subdivision* map;
  map = malloc(sizeof(subdivision));

  split(map);
  int rank;
	int rank_max = map->column*map->line;
  //printf("Quelle rank ?\n");
  //scanf("%d",&rank);
	for ( rank = 1 ; rank <= rank_max+1 ; ++rank)
	{
		get_column_line_with_rank(rank,map,proposition);
		printf("Rank %d correspond à la colonne %d et la ligne %d\n",rank,proposition->X,proposition->Y);
	}
}

void test_generate_reversed_c_way_from_top(void)
{
  int table[18];
  *(table) = 1;
  int rank = generate_reversed_c_way_from_top(6,6,1,table);
  for(int length = 0;length < 18;++length)
    {
      printf("Valeur dans le tableau[%d] = %d\n",length,table[length]);
    }
}

void test_generate_reversed_c_way_from_bottom(void);
{
  int table[18];
  *(table) = 31;
  int rank = generate_reversed_c_way_from_top(6,6,31,table);
  for(int length = 0;length < 18;++length)
    {
      printf("Valeur dans le tableau[%d] = %d\n",length,table[length]);
    }
  return 1;
}

void test_generate_new_base(void)
{
  subdivision* map;
  map = malloc(sizeof(subdivision));

  int* table;
  table = malloc(31*sizeof(int));
  *(table) = 0;

  split(map);

  int rank_max = generate_reversed_c_way_from_top(5,4,1,table);

  int rank =  generate_reversed_c_way_from_bottom(4,2,rank_max,map->column,table);

  for (int depth = 0 ; depth < 32 ; ++depth)
    {
      printf("Valeur du tableau[%d] = %d\n",depth ,*(table+depth));
    }
  free(map);
  free(table);
}

int main(int argc,char* argv[])
{
  return 1;
}
