#ifndef BOTBLUE
#define BOTBLUE

#include "entities.h"
#include "mapFunctions.h"
#include "chainedListFunctions.h"
/* ------------New Things------------- */

typedef struct rectangle_subdivision subdivision;
struct rectangle_subdivision
{
  float long_length ;
  int column ;
  float large_length ;
  int line;
};

void split(void);
int get_rank_with_axes(int column , int line);
void get_center(coordF* center, int column , int line);
int get_rank_with_center_coos(coordF* center);
int get_rank_with_any_coos(coordF position);
int get_min(int* table);
void get_axes_with_rank(int rank,coord* proposition);
void generate_new_base(int* order);
void save_our_sheeps(rencontre *voisins);
coord spotting();
int first_rank(void);
int count_voisins(rencontre *firstNode);

//tests
void test_split(void);
void test_get_center(void);
void test_get_rank_with_center_coos(void);
void test_get_rank_with_any_coos(void);
void test_get_min(void);
void test_get_axes_with_rank(void);
void test_generate_reversed_c_way_from_top(void);

int old_rank;
int max_rank;
int init_rank;
int absolute_rank;
int count_save_sheeps;
int backup_done;
subdivision map;
int* table;

rencontre *saved_sheeps;
rencontre *old_saved_sheeps;
coord backup;
/* ----------------.h----------------- */
coord intel_blue(rencontre *voisins);

#endif
