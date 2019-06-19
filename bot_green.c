#include "bot_green.h"

coord intel_green(rencontre *voisins)
{
  coord obj;
  rencontre *sheep_viseur = voisins;
  rencontre *sheep_vise;
  sheep_viseur = find_voisin_by_color((unsigned char *)"\xe6\xf0\xf0", voisins);
  int radius = 1.4*MAP_SIZE_X/10;
  //printf("MODE %d, (%d,%d)\n",dog->mode, dog->coord.X,dog->coord.Y );
  switch(dog->mode) {
    case 0 :
      obj.X = CENTER_PEN_X;
      obj.Y = CENTER_PEN_Y + radius;
      if(distance(dog->coord.X,dog->coord.Y,obj.X,obj.Y) < 50)
      {
        dog->mode = 1;
      }
      if(sheep_viseur != NULL && !is_in_zone(sheep_viseur))
      {
        ID_suivi = sheep_viseur->ID;
        dog->mode = 2;
      }
    break;
 //(x-CENTER_PEN_X)² +(y-CENTER_PEN_Y)² = (1.4*900)²
    case 1 :
      if(dog->coord.Y > CENTER_PEN_Y)
      {
        if(CENTER_PEN_X == 0.0)
        {
          obj.X = dog->coord.X + 100;
          obj.Y = CENTER_PEN_Y + sqrt(pow(radius,2) - pow(obj.X - CENTER_PEN_X,2));
        }
        if(CENTER_PEN_X == 9000.0)
        {
          obj.X = dog->coord.X - 100;
          obj.Y = CENTER_PEN_Y + sqrt(pow(radius,2) - pow(obj.X - CENTER_PEN_X,2));
        }
      }

      if(dog->coord.Y <= CENTER_PEN_Y)
      {
        if(CENTER_PEN_X == 0.0)
        {
          obj.X = dog->coord.X - 100;
          if(obj.X < 0) obj.X = 0;
          obj.Y = CENTER_PEN_Y - sqrt(pow(radius,2) - pow(dog->coord.X - CENTER_PEN_X,2));
        }

        if(CENTER_PEN_X == 9000.0)
        {
          obj.X = dog->coord.X + 100;
          if(obj.X > CENTER_PEN_X) obj.X = CENTER_PEN_X;
          obj.Y = CENTER_PEN_Y - sqrt(pow(radius,2) - pow(dog->coord.X - CENTER_PEN_X,2));
        }


      }
      if((abs(CENTER_PEN_X - dog->coord.X) < 50) && (dog->coord.Y < CENTER_PEN_Y))
        dog->mode = 0;
      if(sheep_viseur != NULL && !is_in_zone(sheep_viseur))
      {
        ID_suivi = sheep_viseur->ID;
        dog->mode = 2;
      }
    break;
    case 2 :
      sheep_vise = rechercherListeChainee(sheep_viseur, ID_suivi);
      if(sheep_vise == NULL)
      {
        dog->mode = 0;
        break;
      }
      obj = bring_back_our_sheeps(sheep_vise);
      if(is_in_zone(sheep_vise))
        dog->mode = 1;
    break;
    default:
    dog->mode = 0;
    break;
  }
  return obj;
}
