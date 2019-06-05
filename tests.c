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

int main(int argc ,char* argv[])
{
	node* sheep;
	sheep = malloc(sizeof(node));
	effect* sheep_effect;
	sheep_effect = malloc(sizeof(effect));

	node* me;
	me = malloc(sizeof(node));
	view* me_view;
	me_view = malloc(sizeof(view));
	effect* me_effect;
 	me_effect = malloc(sizeof(effect));

	if (me == NULL || me_view == NULL || me_effect == NULL || sheep == NULL || sheep_effect == NULL)
		{
			printf("Out of Memory");
			exit(0);
		}
		unsigned int x_sheep = 4500;
		unsigned int y_sheep = 3000;

		sheep->X = x_sheep ;
		sheep->Y = y_sheep ;
		sheep->nickname = "sheep" ;

		sheep_effect->left = (float) (x_sheep+(ENTITY_SIZE/2));
		sheep_effect->top = (float) (y_sheep+(ENTITY_SIZE/2));
		sheep_effect->right = (float) (x_sheep+(ENTITY_SIZE/2));
		sheep_effect->bot = (float) (y_sheep+(ENTITY_SIZE/2));

		me->X=3000;
		me->Y=3000;
		me->nickname="green";

		unsigned int range_effect = R_ACTION[idColor(me->nickname)];
		unsigned int range_view_X = X_VIEW[idColor(me->nickname)];
		unsigned int range_view_Y = Y_VIEW[idColor(me->nickname)];
		me_view->left = me->X - range_effect;
		me_view->top = me->Y - range_effect;
		me_view->right = me->X + range_effect;
		me_view->bot = me->Y + range_effect;

		me_effect->left = me->X - range_view_X;
		me_effect->top = me->Y - range_view_Y;
		me_effect->right = me->X + range_view_X;
		me_effect->bot = me->Y + range_view_Y;

		unsigned int test = 1000;
		test = touch (me,sheep_effect,sheep);
		printf("%d",test);


free(me);
free(me_view);
free(me_effect);
free(sheep);
free(sheep_effect);
}
