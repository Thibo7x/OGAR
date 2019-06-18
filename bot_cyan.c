#include "bot_cyan.h"

coord intel_cyan(rencontre *voisins)
{
  coord obj;
  rencontre *sheep_viseur = voisins;
  rencontre *sheep_vise;
  sheep_viseur = find_voisin_by_color((unsigned char *)"\xe6\xf0\xf0", voisins);
  //printf("MODE %d\n",dog->mode );
  switch (dog->mode) {
    case 0:
    //Ordre
      if(CENTER_PEN_X == 0.0) obj.X = 9000 - 180;
      if(CENTER_PEN_X == 9000.0) obj.X = 180;
      obj.Y = CENTER_PEN_Y;
    //Sortie
      if(sheep_viseur != NULL && is_in_opposite_zone(sheep_viseur))
      {
        ID_suivi = sheep_viseur->ID;
        dog->mode = 1;
      }
    break;
    case 1:
    //Ordre
      sheep_vise = rechercherListeChainee(sheep_viseur, ID_suivi);
      if(sheep_vise != NULL)
      {
        sheep_direction = direction(dog->coord.X,dog->coord.Y,sheep_vise->coord.X,sheep_vise->coord.Y);
      	obj = sheep_vise->coord;
      }
      if(sheep_vise == NULL)
      {
      	obj.X = round(dog->coord.X + (sheep_direction.X)*20);
      	obj.Y = round(dog->coord.Y + (sheep_direction.Y)*20);
      }
    //Sortie
      if((distance(dog->coord.X,dog->coord.Y,9000-CENTER_PEN_X,CENTER_PEN_Y) > MAP_SIZE_X/10 + 100))
        dog->mode = 0;
    break;
    default:
    break;
  }
  return obj;
}
