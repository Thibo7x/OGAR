#include "libwebsockets.h"
#define MAXLEN 20000
#define ENTITY_SIZE 32

// RED_ASCII, BLUE_ASCII, GREEN_ASCII, YELLOW_ASCII, CYAN_ASCII, PURPLE_ASCII
unsigned char CODES_ASCII[][8] = { {0x0, 0x72, 0x65, 0x64, 0x0, 0x0, 0x0, 0x0}, {0x0, 0x62, 0x6C, 0x75, 0x65, 0x0, 0x0, 0x0}, {0x0, 0x67, 0x72, 0x65, 0x65, 0x6E, 0X0, 0x0}, {0x0, 0x79, 0x65, 0x6C, 0x6C, 0x6F, 0x77, 0x0}, {0x0, 0x63, 0x79, 0x61, 0x6E, 0x0, 0x0, 0x0}, {0x0, 0x70, 0x75, 0x72, 0x70, 0x6C, 0x65, 0x0} };
int CODES_ASCII_LENGTH[] = { 5, 6, 7, 8, 6, 8 };
const unsigned long R_ACTION[] = { 50, 0, 300, 100, 300, 200 };
const unsigned long X_VIEW[] = { 200, 2000, 800, 600, 600, 400 };
const unsigned long Y_VIEW[] = { 200, 2000, 800, 600, 600, 1600 };
char* couleur;
static int callbackOgar(struct lws *wsi, enum lws_callback_reasons reason, void *user, void *in, size_t len);
int idColor(char* couleur);
void rcvFunction(struct lws *wsi, unsigned char* rbuf, size_t len);
int forceExit;

typedef struct s_packet {
        unsigned char buf[MAXLEN+LWS_PRE];
        unsigned int len;
        struct s_packet *next;
} t_packet;

/// Pointer on the list of packet to be send when socket is writeable
t_packet *packetList=NULL;

/// LibWebsocket interlan structure needed for API
struct lws_protocols protocols[] = {
	{
    	"ogar_protocol",
	callbackOgar,
    	0,
    	20
    	},
	{
	NULL,
	NULL,
	0,
	0
	}
};

typedef struct bot {
    int ID;
    unsigned int coordX;
    unsigned int coordY;
} bot;
bot* chien;

typedef struct rencontre {
    unsigned char ID;
    unsigned int coordX;
    unsigned int coordY;
    unsigned char couleur[4];
} rencontre;

void moveBot(struct lws* wsi,unsigned int coordX,unsigned int coordY);
unsigned int distance(unsigned int coordX1, unsigned int coordX2, unsigned int coordY1, unsigned int coordY2);
static int callbackOgar(struct lws *wsi, enum lws_callback_reasons reason, void *user, void *in, size_t len);
