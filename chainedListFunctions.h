#ifndef CHAINEDLISTFUNCTIONS
#define CHAINEDLISTFUNCTIONS

#include "entities.h"
#include "global.h"

void freeListeChainee(rencontre *firstNode);
rencontre* rechercherListeChainee(rencontre *firstNode, unsigned char id);
rencontre* dechiffrageMessage(unsigned char* rbuf);
void explore_chained_list(rencontre *firstNode);

#endif
