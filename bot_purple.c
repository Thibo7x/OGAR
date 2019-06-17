#include "entities.h"
#include "mapFunctions.h"
#include "chainedListFunctions.h"
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include "bot_purple.h"
#define MAP_SIZE_X 9000
#define MAP_SIZE_Y 6000

int distance(int coordX1, int coordY1, int coordX2, int coordY2)
{
	return sqrt((coordX2-coordX1)*(coordX2-coordX1)+(coordY2-coordY1)*(coordY2-coordY1));
}

int checkpoint(coord point,coordF target)
{
  int radius = distance(point.X,point.Y,(int)(target.X),(int)(target.Y));
  if (radius < 3)
    return 1;
  else
    return 0;
}
/* -----------------------*/
//On genère un circuit de 4 points , (0,2800);(1100,2800);(1100,3200);(0,3200) sur un 9000x6000. isOk
void split_purple(void)
{
  // 1er point
  points[0][0] = 0.0 ; //x0
  points[0][1] = (float)((int)(MAP_SIZE_Y/2)-200);  //y0
  // 2ème point
  points[1][0] = (float)((int)MAP_SIZE_X/10+200); //x1
  points[1][1] = points[0][1]; //y1 = y0
  // 3ème point
  points[2][0] = points[1][0];  //x2 = x1
  points[2][1] = (float)((int)MAP_SIZE_Y/2+200); //y2
  // 4ème point
  points[3][0] = 0.0; //x3
  points[3][1] = points[2][1]; //y3 = y2
  // Out Haut (Y <= 3000)
  target_top.X = points[1][0]+300;
  target_top.Y = points[0][1]-900;
  // Out Bas (Y > 3000)
  target_bottom.X = points[1][0]+300;
  target_bottom.Y = points[2][1]+900;
}

// void test_split_purpel(void)
// {
//   for (int n = 0 ; n < 4 ; ++n )
//     {
//       for (int p = 0 ; p <2 ; ++p)
//       {
//         printf("Valeur dans le tableau[%d][%d] = %f\n",n,p,points[n][p]);
//       }
//     }
//    printf("Target Top = (%f,%f) ; Target Bot = (%f,%f)\n",target_top.X,target_top.Y,target_bottom.X,target_bottom.Y);
//  }

//A_REMPLACER par dog->coord. quand on aura intel_purple , sinon , isOK
int get_closest_point(void)
{
  int distance_table[4] = {10817,10817,10817,10817};
  int min = 0;
  for (int rank = 0 ; rank < 4 ; ++rank)
    {
      distance_table[rank] = distance(A_REMPLACER_X,A_REMPLACER_Y,(int)points[rank][0],(int)points[rank][1]);
      if (distance_table[rank] <= distance_table[min])
        min = rank;
    }
    return min;
}
//isOk
// void test_get_closest_point(void)
// {
//   for(int rank = 0 ; rank < 4 ; ++rank)
//     {
//       A_REMPLACER_X = points[rank][0];
//       A_REMPLACER_Y = points[rank][1];
//       printf("Le rank le plus proche est %d.\n",get_closest_point());
//     }
// }


coord spotting_purple(void)
{
		coord target;
		coordF poiiints;
		poiiints.X = points[saved_rank][0];
		poiiints.Y = points[saved_rank][1];
		if (checkpoint(dog->coord,poiiints))
			{
				++saved_rank;
				if (saved_rank == 4)
					saved_rank = 0;
			}
		target.X = (int)points[saved_rank][0];
		target.Y = (int)points[saved_rank][1];
	return target;
}

// coord intel_purple(rencontre* voisins)
// {
// 	coord obj;
//   rencontre *sheep_viseur = voisins;
//   sheep_viseur = find_voisin_by_color((unsigned char *)"\xe6\xf0\xf0", voisins);
//   //mode 0 : il se ballade , mode 1 = il se rapproche du blanc , mode 2 = il repousse l'envahisseur allemand.
//   switch (dog->mode) {
//     case 0:
//       //Sortie
// 			obj = spotting_purple();
//       if(sheep_viseur != NULL && !(is_in_zone(sheep_viseur)))
//         dog->mode = 1;
// 		case 1:
//
// 	return obj;
// }
