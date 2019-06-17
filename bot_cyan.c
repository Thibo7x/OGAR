#include "bot_cyan.h"

coord intel_cyan(rencontre *voisins)
{
  coord obj;
  rencontre *sheep_viseur = voisins;
  rencontre *sheep_vise;
  int radius;
  sheep_viseur = find_voisin_by_color((unsigned char *)"\xe6\xf0\xf0", voisins);
  switch (dog->mode) {
    case 0:
    //Ordre
      if(CENTER_PEN_X == 0.0) obj.X = CENTER_PEN_X + 180;
      if(CENTER_PEN_X == 9000.0) obj.X = CENTER_PEN_X - 180;
      obj.Y = CENTER_PEN_Y;
    //Sortie
      if(sheep_viseur != NULL && is_in_zone(sheep_viseur))
        dog->mode = 1;
    break;
    case 1:
    //Ordre
    if(sheep_viseur != NULL)
    {
      ID_suivi = sheep_viseur->ID;
      obj = circumvention(sheep_viseur);
    }
    //Sortie
      radius = ceil(distance(obj.X, obj.Y, dog->coord.X, dog->coord.Y));
      if (radius <= 20) // Il pousse le mouton s'il est bien positionné
      {
        dog->mode = 2;
      }
    break;
    case 2:
    //Ordre
      sheep_vise = rechercherListeChainee(sheep_viseur, ID_suivi);

    //Sortie
      if(distance(dog->coord.X,dog->coord.Y,CENTER_PEN_X,CENTER_PEN_Y) > MAP_SIZE_X/10)
        dog->mode = 0;
      if (sheep_vise == NULL) // On a perdu le mouton
      {
        dog->mode = 0;
        obj = dog->coord;
      }
      else
      {
        obj = bring_back_our_sheeps(sheep_vise);
        iii++;
      }
      if(iii == 0.5*dog->R_action)
      {
        iii = 0;
        dog->mode = 1;
      }
    break;
    default:
    break;
  }
  return obj;
}
