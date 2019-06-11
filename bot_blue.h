#ifndef BOTBLUE
#define BOTBLUE

#include "entities.h"
#include "mapFunctions.h"
int distance(int coordX1, int coordY1, int coordX2, int coordY2);
/* ------------New Things------------- */
int old_rank;

typedef struct rectangle_subdivision subdivision;
struct rectangle_subdivision
{
  float long_length ;
  int column ;
  float large_length ;
  int line;
};

void split(subdivision* map);
void get_center(coordF* center,subdivision* map,int column , int line);
int get_rank_with_column_line(int column , int line , subdivision* map);
int get_rank_with_center_coos(coordF* center,subdivision* map);
int get_rank_with_any_coos(coordF* position,subdivision* map);
void get_axes_with_rank(int rank,subdivision* map,coord* proposition);
int get_min(int* table , int rank_max);
int generate_reversed_c_way_from_top(int column , int line ,int rank,int* order);
void array_safe_roll(int* table,int min,int max,int range);
int generate_reversed_c_way_from_bottom(int column , int line ,int rank,int biggest_column,int* order);
void generate_new_base(subdivision* map,int* order);
int checkpoint(coord point,coordF target);
//work in progress

//tests
void test_split(void);
void test_get_center(void);
void test_get_rank_with_center_coos(void);
void test_get_rank_with_any_coos(void);
void test_get_min(void);
void test_get_axes_with_rank(void);
void test_generate_reversed_c_way_from_top(void);
/* ----------------.h----------------- */
coord intel_blue(rencontre *voisins);

#endif
