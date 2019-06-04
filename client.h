#include "libwebsockets.h"
#define MAXLEN 20000
#define ENTITY_SIZE 32

#define RED_ASCII {0x0, 0x72, 0x65, 0x64, 0x0}
#define RED_R_ACTION 50
#define RED_X_VIEW 200
#define RED_Y_VIEW 200

#define BLUE_ASCII {0x0, 0x62, 0x6C, 0x75, 0x65, 0x0}
#define BLUE_R_ACTION 0
#define BLUE_X_VIEW 2000
#define BLUE_Y_VIEW 2000

#define GREEN_ASCII {0x0, 0x67, 0x72, 0x65, 0x65, 0x6E, 0X0}
#define GREEN_R_ACTION 280
#define GREEN_X_VIEW 800
#define GREEN_Y_VIEW 800

#define YELLOW_ASCII {0x0, 0x79, 0x65, 0x6C, 0x6C, 0x6F, 0x77, 0x0}
#define YELLOW_R_ACTION 70
#define YELLOW_X_VIEW 600
#define YELLOW_Y_VIEW 600

#define CYAN_ASCII {0x0, 0x63, 0x79, 0x61, 0x6E, 0x0}
#define CYAN_R_ACTION 300
#define CYAN_X_VIEW 600
#define CYAN_Y_VIEW 600

#define PURPLE_ASCII {0x0, 0x70, 0x75, 0x72, 0x70, 0x6C, 0x65, 0x0}
#define PURPLE_R_ACTION 200
#define PURPLE_X_VIEW 400
#define PURPLE_Y_VIEW 1600

int initiateConnection(struct lws *wsi)
{
	static unsigned char bonjour[] = {0xff,0x0,0x0,0x0,0x0}; // 255
	static unsigned char lets_go[] = {0xfe,0x0d,0x0,0x0,0x0}; // 254

	sendCommand(wsi,bonjour, sizeof(bonjour));
	sendCommand(wsi,lets_go, sizeof(lets_go));
}

static int callbackOgar(struct lws *wsi, enum lws_callback_reasons reason, void *user, void *in, size_t len);

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

static int callbackOgar(struct lws *wsi, enum lws_callback_reasons reason, void *user, void *in, size_t len);
