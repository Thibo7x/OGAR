#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "chainedListFunctions.h"
#include "mapFunctions.h"

/**
\brief Delete a chained list
\param firstNode beginning of the chained list
****************************************************************************************************************************/
void freeListeChainee(rencontre *firstNode)
{
	rencontre *viseur;
	rencontre *viseur_back;
	if (firstNode != NULL)
	{
		while (firstNode->next != NULL)
		{
			viseur = firstNode;
			viseur_back = firstNode;
			while (viseur->next != NULL) viseur = viseur->next;
			while (viseur_back->next != viseur) viseur_back = viseur_back->next;
			free(viseur);
			viseur_back->next = NULL;
		}
	}
	free(firstNode);
}
/**
\brief Explore the chained list
\param firstNode beginning of the chained list
\return nothing (show in stdout)
****************************************************************************************************************************/
void explore_chained_list(rencontre *firstNode)
{
	rencontre *viseur = firstNode;
	while (viseur != NULL)
	{
		if (!memcmp(viseur->couleur,"\xe6\xf0\xf0",3))
		{
			printf("Sheep à une distance de %d", distance(dog->coord.X, dog->coord.Y, viseur->coord.X, viseur->coord.Y));
		}
		else if (!memcmp(viseur->couleur,"\x0\x0\xff",3))
		{
			printf("Blue à une distance de %d", distance(dog->coord.X, dog->coord.Y, viseur->coord.X, viseur->coord.Y));
		}
		else if (!memcmp(viseur->couleur,"\x0\xff\x0",3))
		{
			printf("Green à une distance de %d", distance(dog->coord.X, dog->coord.Y, viseur->coord.X, viseur->coord.Y));
		}
		else if (!memcmp(viseur->couleur,"\xff\xff\x0",3))
		{
			printf("Yellow à une distance de %d", distance(dog->coord.X, dog->coord.Y, viseur->coord.X, viseur->coord.Y));
		}
		else if (!memcmp(viseur->couleur,"\xff\x0\xff",3))
		{
			printf("Purple à une distance de %d", distance(dog->coord.X, dog->coord.Y, viseur->coord.X, viseur->coord.Y));
		}
		else if (!memcmp(viseur->couleur,"\x0\xff\xff",3))
		{
			printf("Cyan à une distance de %d", distance(dog->coord.X, dog->coord.Y, viseur->coord.X, viseur->coord.Y));
		}
		else if (!memcmp(viseur->couleur,"\xff\x0\x0",3))
		{
			printf("Red à une distance de %d", distance(dog->coord.X, dog->coord.Y, viseur->coord.X, viseur->coord.Y));
		}
		else {
			printf("OVNI à une distance de %d", distance(dog->coord.X, dog->coord.Y, viseur->coord.X, viseur->coord.Y));
		}
		printf("\n");
		viseur = viseur->next;
	}
	printf("\n");
}

/**
\brief Search a neighbour from its ID field on a chained list
\param firstNode beginning of the chained list
\param id Searched ID
\return pointer to neighbour
****************************************************************************************************************************/
rencontre* rechercherListeChainee(rencontre *firstNode, unsigned char id)
{
	rencontre *viseur = firstNode;
	if (viseur == NULL)
		return NULL;
	while (viseur->ID != id)
	{
		viseur = viseur->next;
		if (viseur == NULL)
		{
			break;
			return NULL;
		}
	}
	return viseur;
}

/**
\brief Decrypt 0x10 messages from server
\param rbuf payload
\return pointer to neighbour chained list with position and colour of each neighbour
****************************************************************************************************************************/
rencontre* dechiffrageMessage(unsigned char* rbuf)
{
	rencontre *firstNode = NULL;
	unsigned char *reader_c = rbuf+3;
	do {
		rencontre *node = malloc(sizeof(node));
		node->ID = (*reader_c); // ID
		reader_c += 4;
		int i;
		// CoordX
		node->coord.X = 0;
		node->coord.X += (unsigned int)(*(reader_c) & 0x0F);
		node->coord.X += (unsigned int)((*(reader_c++) & 0xF0) >> 4)*16;
		node->coord.X += (unsigned int)(*(reader_c) & 0x0F)*256;
		node->coord.X += (unsigned int)((*(reader_c++) & 0xF0) >> 4) *4096;
		 // CoordY
		reader_c += 2;
		node->coord.Y = 0;
		node->coord.Y += (unsigned int)(*(reader_c) & 0x0F);
		node->coord.Y += (unsigned int)((*(reader_c++) & 0xF0) >> 4)*16;
		node->coord.Y += (unsigned int)(*(reader_c) & 0x0F)*256;
		node->coord.Y += (unsigned int)((*(reader_c++) & 0xF0) >> 4) *4096;
		reader_c += 5;
		// Couleur
		for (i = 0; i < 3; i++)
		{
			node->couleur[i] = *(reader_c);
			reader_c += 1;
		}
		node->couleur[i] = '\0';
		i = 0;
		while (*(reader_c+i) != 0x0) i++;
		reader_c += (i+1);

		// Si c'est nous, on modifie chien
		if (node->ID == dog->ID)
		{
			dog->coord.X = node->coord.X;
			dog->coord.Y = node->coord.Y;
		}
		else
		{
			// Sinon, ajout à la liste chaînée
			if (rechercherListeChainee(firstNode, node->ID) == NULL) // On vérifie s'il n'est pas déjà présent
			{
				node->next = firstNode;
				firstNode = node;
			}
		}

	} while ( (*reader_c != 0x0) || (*(reader_c+1) != 0x0) || (*(reader_c+2) != 0x0) || (*(reader_c+3) != 0x0) ); // Fin du payload
	return firstNode;
}
