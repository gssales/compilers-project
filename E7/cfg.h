#ifndef _CFG_H_
#define _CFG_H_
// Control-flow Graph
#include "iloc.h"

typedef struct cfg_node cfg_node_t;
struct cfg_node {
  int id;
  iloc_code_t* start;
  iloc_code_t* end;
};

typedef struct cfg_edge cfg_edge_t;
struct cfg_edge {
  int src, dest;
};

typedef struct cfg cfg_t;
struct cfg {
  int count_nodes;
  cfg_node_t** nodes;
  int count_edges;
  cfg_edge_t** edges;
};

cfg_t* create_cfg();
int cfg_add_node(cfg_t* cfg, iloc_code_t* start, iloc_code_t* end);
void cfg_add_edge(cfg_t* cfg, int src, int dest);
void destroy_cfg(cfg_t* cfg);

cfg_t* generate_cfg(iloc_program_t* program);

#endif //_CFG_H_