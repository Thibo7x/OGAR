#ifndef ENTITIES
#define ENTITIES

#define ENTITY_SIZE 32

typedef struct coord {
  int X;
  int Y;
} coord;


typedef struct coordF {
  float X;
  float Y;
} coordF;

typedef struct bot {
    char nickname[10];
    unsigned char ID;
    coord coord;
    unsigned int color;
    int mode;
    int R_action;
    coord view;
} bot;
bot* dog;

typedef struct rencontre {
    char nickname[10];
    unsigned char ID;
    coord coord;
    unsigned char couleur[4];
    struct rencontre *next;
} rencontre;

#endif
