void turn_to_indicate(rencontre *sheep)
//Tourne le chien autour du cercle jusqu'à ce qu'il soit aligné avec la brebis à indiquer
{
	double angle;
	coordF direction = direction(dog->coord.X,dog->coord.Y,sheep->coord.X,sheep->coord.Y)
	coord reach_point;
	//Coordonnées du point à atteindre pour être aligné avec la brebis
	reach_point.X = MAP_SIZE_X/2 + ceil((direction.X)*150);
	reach_point.Y = MAP_SIZE_Y/2 + ceil((direction.Y)*150);
	while(dog->coord.X != reach_point.X && dog->coord.Y != reach_point.Y)
	{
		for(angle = 0; angle < 2*M_PI; angle += 0.01)
			{
				//Equation du cercle : (x-4500)²+(y-3000)² = 150²
				moveBot((unsigned int)(150*cos(angle) + MAP_SIZE_X/2), (unsigned int)(150*sin(angle)+MAP_SIZE_Y/2));
		  }
	}
}


//Pas mal de fonction a faire a l'intérieur
void intel_blue(struct lws *wsi,rencontre *voisins)
{
	int counter = 0;
	rencontre *sheepCounter = voisins;
	coord retour;
	while(sheepCounter != NULL && counter < 4) //Mettre le nombre de bots jaunes ici
	{
		//spotting(); Fonction de repérage
		if (!memcmp(sheepCounter->couleur,"\xe6\xf0\xf0",3)) //si c'est une brebis
			counter++;
		sheepCounter = sheepCounter->next;
	}
	if(counter == 4)
	{
		sheepCounter = voisins;
		moveBot(wsi,MAP_SIZE_X/2,MAP_SIZE_Y/2);
		if(distance(dog->coord.X,dog->coord.Y,MAP_SIZE_X/2,MAP_SIZE_Y/2) <= 150)
			moveBot(wsi,dog->coord.X,dog->coord.Y);
		while(counter != 0)
		{
			while(!memcmp(sheepCounter->couleur,"\xe6\xf0\xf0",3))
			{
				sheepCounter = sheepCounter->next;
			}
			counter--;
			turn_to_indicate(sheepCounter->coord);
			//Tourne autour du cercle pour être aligné au mouton à indiquer
			retour.X = dog->coord.X;
			retour.Y = dog->coord.Y;
			moveBot(wsi,MAP_SIZE_X/2,MAP_SIZE_Y/2);
			if(distance(dog->coord.X,dog->coord.Y,MAP_SIZE_X/2,MAP_SIZE_Y/2) <= 90)
				moveBot(wsi,retour.X,retour.Y);
		}
	}

}

//A rajouter dans rcv_function quand on pourra le faire sans tout casser
unsigned char* get_size(struct lws *wsi,unsigned char* rbuf)
{
	unsigned char taille[6];
	int firstBorderInfoMessage = 1;
	//switch(rbuf[0])
			case 0x64:
			if(firstBorderInfoMessage)
			{
				firstBorderInfoMessage = 0;
				taille[0] = rbuf[22];
				taille[1] = rbuf[23];
				taille[2] = rbuf[24];
				taille[3] = rbuf[30];
				taille[4] = rbuf[31];
				taille[5] = rbuf[32];
			}
			break;
	return taille;
}

coord borderCorrection(coord reach_point)
//A tester, permet à un chien de déplacer un mouton se trouvant proche d'une bordure
{
	coord objectif;
	if(dog->coord.X > MAP_SIZE_X-R_ACTION[dog->color])
	{
		objectif.X = MAP_SIZE_X;
		objectif.Y = reach_point.Y;
	}
	else if(dog->coord.X < R_ACTION[dog->color])
	{
		objectif.X = 0;
		objectif.Y = reach_point.Y;
	}
	else if(dog->coord.Y > MAP_SIZE_Y-R_ACTION[dog->color])
	{
		objectif.X = reach_point.X;
		objectif.Y = MAP_SIZE_Y;
	}
	else if(dog->coord.Y < R_ACTION[dog->color])
	{
		objectif.X = reach_point.X;
		objectif.Y = 0;
	}
	else
	{
		objectif.X = reach_point.X;
		objectif.Y = reach_point.Y;
	}
	return
}
