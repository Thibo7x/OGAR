#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>
#include <signal.h>
#include <syslog.h>
#include <sys/time.h>
#include <unistd.h>
#include <math.h>


#include "client.h"

// compile with gcc -Wall -g -lm -o sock ./test-client.c -lwebsockets


// =====================================================================================================================================
//	Start of function definition
// =====================================================================================================================================

// Caught on CTRL C
void sighandler(int sig)
{
	forceExit = 1;
}

/**
\brief Allocate a packet structure and initialise it.
\param none
\return pointer to new allocated packet
****************************************************************************************************************************/
t_packet *allocatePacket()
{
	t_packet *tmp;

	if ((tmp=malloc(sizeof(t_packet))) == NULL ) return NULL;
	memset(tmp,0,sizeof(t_packet));
	return tmp;
}

/**
\brief Add a packet to the list of packet to be sent
\param wsi websocket descriptor
\param buf buffer to be sent
\param len length of packet
\return pointer to new allocated packet
****************************************************************************************************************************/
int sendCommand(struct lws *wsi,unsigned char *buf,unsigned int len)
{
	t_packet *tmp,*list=packetList;

	if (len > MAXLEN ) return -1;
	if ((tmp=allocatePacket()) == NULL ) return -1;
	memcpy(&(tmp->buf)[LWS_PRE],buf,len);
	tmp->len=len;
	if (packetList == NULL )
		packetList=tmp;
	else {
		while (list && list->next) {
			list=list->next;
		}
		list->next=tmp;
	}
	lws_callback_on_writable(wsi);
	return 1;
}


/****************************************************************************************************************************/
int writePacket(struct lws *wsi)
{
	t_packet *tmp=packetList;
	int ret;

	if (packetList == NULL ) return 0;

	packetList=tmp->next;
	ret=lws_write(wsi,&(tmp->buf)[LWS_PRE],tmp->len,LWS_WRITE_BINARY);
	free(tmp);
	lws_callback_on_writable(wsi);
	return(ret);
}

/****************************************************************************************************************************/
unsigned int distance(unsigned int coordX1, unsigned int coordX2, unsigned int coordY1, unsigned int coordY2)
{
	return sqrt((coordX2-coordX1)*(coordX2-coordX1)+(coordY2-coordY1)*(coordY2-coordY1));
}

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
			printf("Sheep à une distance de %d", distance(dog->coordX, dog->coordY, viseur->coordX, viseur->coordY));
		}
		else if (!memcmp(viseur->couleur,"\x0\x0\xff",3))
		{
			printf("Blue à une distance de %d", distance(dog->coordX, dog->coordY, viseur->coordX, viseur->coordY));
		}
		else if (!memcmp(viseur->couleur,"\x0\xff\x0",3))
		{
			printf("Green à une distance de %d", distance(dog->coordX, dog->coordY, viseur->coordX, viseur->coordY));
		}
		else if (!memcmp(viseur->couleur,"\xff\xff\x0",3))
		{
			printf("Yellow à une distance de %d", distance(dog->coordX, dog->coordY, viseur->coordX, viseur->coordY));
		}
		else if (!memcmp(viseur->couleur,"\xff\x0\xff",3))
		{
			printf("Purple à une distance de %d", distance(dog->coordX, dog->coordY, viseur->coordX, viseur->coordY));
		}
		else if (!memcmp(viseur->couleur,"\x0\xff\xff",3))
		{
			printf("Cyan à une distance de %d", distance(dog->coordX, dog->coordY, viseur->coordX, viseur->coordY));
		}
		else if (!memcmp(viseur->couleur,"\xff\x0\x0",3))
		{
			printf("Red à une distance de %d", distance(dog->coordX, dog->coordY, viseur->coordX, viseur->coordY));
		}
		else {
			printf("OVNI à une distance de %d", distance(dog->coordX, dog->coordY, viseur->coordX, viseur->coordY));
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
	while (viseur->ID != id)
	{
		viseur = viseur->next;
		if (viseur == NULL)
		{
			break;
			return 0;
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
		node->coordX = 0;
		node->coordX += (unsigned int)(*(reader_c) & 0x0F);
		node->coordX += (unsigned int)((*(reader_c++) & 0xF0) >> 4)*16;
		node->coordX += (unsigned int)(*(reader_c) & 0x0F)*256;
		node->coordX += (unsigned int)((*(reader_c++) & 0xF0) >> 4) *4096;
		 // CoordY
		reader_c += 2;
		node->coordY = 0;
		node->coordY += (unsigned int)(*(reader_c) & 0x0F);
		node->coordY += (unsigned int)((*(reader_c++) & 0xF0) >> 4)*16;
		node->coordY += (unsigned int)(*(reader_c) & 0x0F)*256;
		node->coordY += (unsigned int)((*(reader_c++) & 0xF0) >> 4) *4096;
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
			dog->coordX = node->coordX;
			dog->coordY = node->coordY;
		}
		else
		{
			// Sinon, ajout à la liste chaînée
			node->next = firstNode;
			firstNode = node;
		}

	} while ( (*reader_c != 0x0) || (*(reader_c+1) != 0x0) || (*(reader_c+2) != 0x0) || (*(reader_c+3) != 0x0) ); // Fin du payload
	return firstNode;
}

void rcvFunction(struct lws *wsi, unsigned char* rbuf, size_t len)
{
	rencontre *voisins;
		switch (rbuf[0]) {
			case 0x12:
			//Q On Command, on répond avec le Nickname
			sendCommand(wsi,CODES_ASCII[dog->color],CODES_ASCII_LENGTH[dog->color]);
			break;

			case 0x20:
			//Add Node Message, on récupère l'ID de notre bot
			dog->ID = rbuf[1];
			break;

			case 0x10:
				voisins = dechiffrageMessage(rbuf);
				// Fonctions de parcours de liste chainee, de suppression de la liste chainee, etc.

				explore_chained_list(voisins);

				if (voisins != NULL)
				{
					while
					moveBot(wsi, 4500,4500);
				}



				freeListeChainee(voisins);
			break;

			default:
			break;
		}
}

/**
\brief Send wished pos to server
\param wsi websocket context
\param coordX X wished pos
\param coordY Y wished pos
****************************************************************************************************************************/
void moveBot(struct lws* wsi,unsigned int coordX,unsigned int coordY)
{
	char buffer[13] = {};
	buffer[0] = 0x10;
	//CoordX
  sprintf(buffer+1,"%c",coordX%256);
	sprintf(buffer+2,"%c",((coordX/4096)<<4)+(coordX%4096)/256);

	//CoordY
	sprintf(buffer+5,"%c",coordY%256);
	sprintf(buffer+6,"%c",((coordY/4096)<<4)+(coordY%4096)/256);
	sendCommand(wsi,(unsigned char *)buffer,sizeof(buffer));
}

/**
\brief Give the ID related to each color (see beginning of client.h file)
****************************************************************************************************************************/
void idColor(char* color) {
	if (!strcmp(color,"red"))
	{
		dog->color = 0;
	}
	if (!strcmp(color,"blue"))
	{
		dog->color = 1;
	}
	if (!strcmp(color,"green"))
	{
		dog->color = 2;
	}
	if (!strcmp(color,"yellow"))
	{
		dog->color = 3;
	}
	if (!strcmp(color,"cyan"))
	{
		dog->color = 4;
	}
	if (!strcmp(color,"purple"))
	{
		dog->color = 5;
	}
}

/****************************************************************************************************************************/

int initiateConnection(struct lws *wsi)
{
	static unsigned char bonjour[] = {0xff,0x0,0x0,0x0,0x0}; // 255
	static unsigned char lets_go[] = {0xfe,0x0d,0x0,0x0,0x0}; // 254

	sendCommand(wsi,bonjour, sizeof(bonjour));
	sendCommand(wsi,lets_go, sizeof(lets_go));
	return 0;
}

/****************************************************************************************************************************/

static int callbackOgar(struct lws *wsi, enum lws_callback_reasons reason, void *user, void *in, size_t len)
{
	static unsigned int offset=0;
	static unsigned char rbuf[MAXLEN];

	switch (reason) {
	case LWS_CALLBACK_CLIENT_ESTABLISHED:
		fprintf(stderr, "ogar: LWS_CALLBACK_CLIENT_ESTABLISHED\n");
		initiateConnection(wsi);
		break;

 	case LWS_CALLBACK_CLIENT_WRITEABLE:
		if (writePacket(wsi) < 0 ) forceExit=1;
		break;

	case LWS_CALLBACK_CLIENT_RECEIVE:
		// we have receive some data, check if it can be written in static allocated buffer (length)

		if (offset + len < MAXLEN ) {
			memcpy(rbuf+offset,in,len);
			offset+=len;
			// we have receive some data, check with websocket API if this is a final fragment
			if (lws_is_final_fragment(wsi)) {

				rcvFunction(wsi,rbuf,len);
				// call recv function here
				offset=0;
			}
		} else {	// length is too long... get others but ignore them...
			offset=MAXLEN;
		 	if ( lws_is_final_fragment(wsi) ) {
				offset=0;
			}
		}
		break;
	case LWS_CALLBACK_CLOSED:
		lwsl_notice("ogar: LWS_CALLBACK_CLOSED\n");
		forceExit = 1;
		break;
	case LWS_CALLBACK_CLIENT_CONNECTION_ERROR:
		lwsl_err("ogar: LWS_CALLBACK_CLIENT_CONNECTION_ERROR\n");
		forceExit = 1;
		break;

	case LWS_CALLBACK_COMPLETED_CLIENT_HTTP:
		lwsl_err("ogar: LWS_CALLBACK_COMPLETED_CLIENT_HTTP\n");
		forceExit = 1;
		break;

	default:
		break;
	}

	return 0;
}

/****************************************************************************************************************************/
int main(int argc, char **argv)
{
	int n = 0;

	struct lws_context_creation_info info;
	struct lws_client_connect_info i;
	struct lws_context *context;
	const char *protocol,*temp;

	memset(&info, 0, sizeof info);
	memset(&i, 0, sizeof(i));

	if (argc < 2)
		goto usage;

	i.origin = "agar.io";

	while (n >= 0) {
		n = getopt(argc, argv, "hsp:P:n:");
		if (n < 0)
			continue;
		switch (n) {
		case 's':
			i.ssl_connection = 2;
			break;
		case 'p':
			i.port = atoi(optarg);
			break;
		case 'P':
			info.http_proxy_address = optarg;
			break;
		case 'n':
		  couleur = optarg;
			break;
		case 'h':
			goto usage;
		}
	}
  dog = malloc(sizeof(dog));
	srandom(time(NULL));

	if (optind >= argc)
		goto usage;

	signal(SIGINT, sighandler);

	if (lws_parse_uri(argv[optind], &protocol, &i.address, &i.port, &temp))
		goto usage;

	if (!strcmp(protocol, "http") || !strcmp(protocol, "ws"))
		i.ssl_connection = 0;
	if (!strcmp(protocol, "https") || !strcmp(protocol, "wss"))
		i.ssl_connection = 1;

	i.host = i.address;
	i.ietf_version_or_minus_one = -1;
	i.client_exts = NULL;
	i.path="/";

	info.port = CONTEXT_PORT_NO_LISTEN;
	info.protocols = protocols;
	info.gid = -1;
	info.uid = -1;

	context = lws_create_context(&info);
	if (context == NULL) {
		fprintf(stderr, "Creating libwebsocket context failed\n");
		return 1;
	}

	i.context = context;

	if (lws_client_connect_via_info(&i)); // just to prevent warning !!

	forceExit=0;
	// the main magic here !!
	while (!forceExit) {
		lws_service(context, 1000);
	}
	// if there is some errors, we just quit
	lwsl_err("Exiting\n");
	lws_context_destroy(context);
  free(dog);
	return 0;

usage:
	fprintf(stderr, "Usage: ogar-client -h -s -p <port> -n <nickname> -P <proxy> <server address> \n");
	return 1;
}


//Vérifie si un chien donné est loin/touche/dans un mouton donné. 2 si "dedans" 1 si "touche" 0 si "loin"
//Pas encore effectif
// unsigned int touch(effect* sheep_effect , bot* sheep)
// {
// 		unsigned int range = R_ACTION[dog->color];
// 		correction(sheep_effect);
//
// 		sheep_effect->left -= range;
// 		sheep_effect->top -= range;
// 		sheep_effect->right += range;
// 		sheep_effect->bottom += range;
//
//
// 			if( (((dog->coordX < sheep_effect->right) && dog->coordY > sheep_effect->top) || dog->coordX < sheep_effect->left)/**/ || /**/(((dog->coordX > sheep_effect->left && dog->coordY < sheep_effect->bottom) ||  dog->coordX > sheep_effect->right)))
// 				return 2;
// 			if((dog->coordX > sheep_effect->left && dog->coordY > sheep_effect->bottom&& dog->coordX < sheep_effect->right && dog->coordY < sheep_effect->top))
// 				return 0;
// 			else
// 				return 1;
// }

unsigned int action_over_sheep(rencontre *sheep)
{
	unsigned int r_action = R_ACTION[dog->color];
	if (distance(dog->coordX, dog->coordY, sheep->coordX, sheep->coordY) < r_action)
		return 1;
	else
		return 0;
}

coord direction(unsigned int *coordX1, unsigned int *coordY1, unsigned int *coordX2, unsigned int *coordY2)
{
	coord dir;
	dir.X = abs(coordX1 - coordX2) / distance(coordX1, coordY1, coordY1, coordY2);
	dir.Y = abs(coordY1 - coordY2) / distance(coordX1, coordY1, coordY1, coordY2);
	return dir;
}

coord reach_point(rencontre *sheep, coord direction)
{
	coord reach_point;
	reach_point.X = sheep->coordX + coord[0]*R_ACTION[dog->color];
	reach_point.Y = sheep->coordX + coord[1]*R_ACTION[dog->color];
	return reach_point;
}

coord circumvention(rencontre *sheep)
{

	objectif = reach_point(sheep);
	coord chemin;
	chemin.X = reach_point.X;
	if (reach_point.X != dog->coordX)
	{
		// if (action_over_sheep(coord++)) ...
		chemin.Y = dog->coordY;
	}
	else
	{
		chemin.Y = reach_point.Y;
	}
	return chemin;
}

//correction de plage d'effet si on se trouve près du plafond ou près du mur de gauche.
//Pas encore effectif
void correction(effect* border)
{
	if (border->left < R_ACTION[dog->color])
		border->left = R_ACTION[dog->color];
	if (border->top < R_ACTION[dog->color])
		border->top = R_ACTION[dog->color];
}
