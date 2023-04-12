#ifndef _CFG_H_
#define _CFG_H_
// Control-flow Graph
#include "iloc.h"

typedef struct cfg_node cfg_node_t;
struct cfg_node {
  int id;
  iloc_code_t* start, end;
};

typedef struct cfg_edge cfg_edge_t;
struct cfg_edge {
  int src, dest;
};

typedef struct cfg cfg_t;
struct cfg {
  int count_nodes;
  cfg_node_t** nodes;
  cfg_edge_t** edges;
};

cfg_t* create_cfg();
int cfg_add_node(iloc_code_t* start, iloc_code_t* end);
void cfg_add_edge(int src, int dest);
void destroy_cfg(cfg_t* cfg);

#endif //_CFG_H_