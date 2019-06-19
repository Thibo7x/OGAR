#include "bot_cyan.h"

coord intel_cyan(rencontre *voisins)
{
  coord obj;
  //printf("MODE %d\n",dog->mode );
  if(CENTER_PEN_X == 0.0) obj.X = 9000 - 1.05*dog->R_action;
  if(CENTER_PEN_X == 9000.0) obj.X = 1.05*dog->R_action;
  //MERCI BEAUCOUP MONSIEUR LORIN VINCENT
  switch (dog->mode) {
    case 0 :
    obj.Y = CENTER_PEN_Y - 1050;
    if(distance(dog->coord.X,dog->coord.Y,obj.X,obj.Y) < 3) dog->mode = 1;
    break;

    case 1 :
    obj.Y = CENTER_PEN_Y + 1050;
    if(distance(dog->coord.X,dog->coord.Y,obj.X,obj.Y) < 3) dog->mode = 0;
    break;
    default:
    dog->mode = 0;
    break;
  }
  return obj;
}
