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
int get_rank_with_column_line(int column , int line);
void get_center(coordF* center, int column , int line);
int get_rank_with_center_coos(coordF* center);
int get_rank_with_any_coos(coordF* position);
int get_min(int* table , int rank_max);
void get_axes_with_rank(int rank,coord* proposition);
int generate_reversed_c_way_from_top(int column , int line ,int rank,int* order);
void array_safe_roll(int* table,int min,int max,int range);
int generate_reversed_c_way_from_bottom(int column , int line ,int rank,int biggest_column,int* order);
void generate_new_base(int* order);
void save_our_sheeps(rencontre *voisins);
int checkpoint(coord point,coordF target);
coord turn_to_indicate(rencontre *sheep);
//work in progress

//tests
void test_split(void);
void test_get_center(void);
void test_get_rank_with_center_coos(void);
void test_get_rank_with_any_coos(void);
void test_get_min(void);
void test_get_axes_with_rank(void);
void test_generate_reversed_c_way_from_top(void);

int old_rank;
subdivision map;
rencontre *saved_sheeps;
/* ----------------.h----------------- */
coord intel_blue(rencontre *voisins);

#endif
