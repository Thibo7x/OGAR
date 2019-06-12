#define MAP_SIZE_X 9000
#define MAP_SIZE_Y 6000
#include <stdlib.h>
#include <stdio.h>
#include "test-secret.h"
#include <math.h>

void create_map(lines, columns)
{
  int i,lines_temp, columns_temp;
  if (lines == 0 || columns == 0 || columns == 1 || lines == 1)
  {
    return;
  }

  lines_temp = indice_first_line;

  for (i = 0; i < columns; i++)
  {
    map[lines_temp][i] = j;
    j++;
  }

  columns_temp = (columns-1);

  for (i = 1; i < lines; i++)
  {
    map[i+indice_first_line][columns_temp] = j;
    j++;
  }
  lines_temp = (lines-1+indice_first_line);
  for (i = (columns-2); i >= 0; i--)
  {
    map[lines_temp][i] = j;
    j++;
  }
  indice_first_line+=1;
  indice_last_column+=1;
  create_map(lines-2, columns-1);
}

void show_map(int lines, int columns)
{
  int i,j;
  for (i = 0; i < lines; i++)
  {
    for(j = 0; j < columns; j++)
    {
      if (map[i][j] < 10)
        printf(" %d ", map[i][j]);
      else
        printf("%d ", map[i][j]);
    }
    printf("\n");
  }
}

int main()
{
  int lines = (int) ceil( (float)(MAP_SIZE_Y/2) / 2000.0) * 2;
  int columns = (int) ceil( (float)(MAP_SIZE_X/2) / 2000.0) * 2; // 600 = R_VUE[yellow]

  printf("L : %d * C : %d\n",lines,columns);
// 1  2  3  4
// 10 11 12 5
// 9  8  7  6
//
// 1  2  3  4  5
// 13 14 15 16 6
// 20 19 18 17 7
// 12 11 10 9  8
//
// 1  2  3  4  5
// 14 15 16 17 6
// 23 24 25 18 7
// 22 21 20 19 8
// 13 12 11 10 9
//
// 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20
  j = 1;
  indice_first_line = 0;
  indice_last_column = 0;

  int i;
  map = malloc(sizeof(int *) * lines);
  for (i = 0; i < lines; i++)
  {
    map[i] = malloc(sizeof(int)*columns);
  }

  //map = malloc(lines*columns);

  create_map(lines, columns);
  show_map(lines, columns);

  return 0;
//map[lines][columns]
}



// get_square_center(id)
// {
//   coord obj;
//   obj.X = MAP_SIZE_X /
// }
