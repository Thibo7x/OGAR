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
#include "bot_yellow.h"
#include "bot_blue.h"
#include "chainedListFunctions.h"
#include "entities.h"
#include "mapFunctions.h"

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

			case 0x40:
			if(firstBorderInfoMessage)
			{
				firstBorderInfoMessage = 0;
				MAP_SIZE_X = *((double *)(rbuf+17));
			  MAP_SIZE_Y = *((double *)(rbuf+25));
				if(dog->color == 1)
				{
					split();
					max_rank = map.column*map.line;
					table = malloc(max_rank*sizeof(int));
					generate_new_base(table);
				}
			}
			break;

			case 0x10:
				voisins = dechiffrageMessage(rbuf);
				// Fonctions de parcours de liste chainee, de suppression de la liste chainee, etc.

				//explore_chained_list(voisins);
				coord pos;
				switch (dog->color)
				{
					case 0:
					// RED
						pos.X = 0;
						pos.Y = 0;
					break;
					case 1:
					// BLUE
						if (init_rank) {
							init_rank = 0;
							coordF dogF;
							dogF.X = (float)(dog->coord.X);
							dogF.Y = (float)(dog->coord.Y);
							old_rank = get_rank_with_any_coos(dogF);
							// printf("%d & %d\n", dog->coord.X, dog->coord.Y);
							// printf("Rank : %d\n", table[old_rank]);
							saved_sheeps = NULL;
						}
						pos = intel_blue(voisins);
					break;
					case 2:
					// GREEN
						pos.X = 0;
						pos.Y = 0;
					break;
					case 3:
						pos = intel_yellow(voisins);
					break;
					case 4:
					// CYAN
						pos.X = 0;
						pos.Y = 0;
					break;
					case 5:
					// PURPLE
						pos.X = 0;
						pos.Y = 0;
					break;
					default:
						pos.X = 0;
						pos.Y = 0;
					break;
				}

				moveBot(wsi, pos.X, pos.Y);

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
	iii = 0;
	firstBorderInfoMessage = 1;

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
	idColor(couleur);
	dog->mode = 0;

	if (dog->mode == 1)
	{
		init_rank = 1;
	}
	if (dog->mode == 3) // Jaune
	{
		blue_dog = 0;
	}


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

	dog->R_action = R_ACTION[dog->color];
	dog->view.X = X_VIEW[dog->color];
	dog->view.Y = Y_VIEW[dog->color];

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
