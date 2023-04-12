#include <stdlib.h>
#include "cfg.h"

cfg_t* create_cfg() {
  cfg_t* cfg = malloc(sizeof(cfg_t));
  if (cfg != NULL) {
    cfg->count_nodes = 0;
    cfg->nodes = NULL;
    cfg->edges = NULL;
  }
  return cfg;
}

int cfg_add_node(cfg_t* cfg, iloc_code_t* start, iloc_code_t* end) {
  int id = -1;
  if (cfg != NULL && start != NULL && end != NULL) {
    cfg_node_t* node = malloc(sizeof(cfg_node_t));
    if (node != NULL) {
      node->start = start;
      node->end = end;

      id = cfg->count_nodes;
      cfg->count_nodes++;
      cfg->nodes = realloc(cfg->nodes, cfg->count_nodes * sizeof(cfg_node_t*));
      cfg->nodes[cfg->count_nodes-1] = node;
    }
  }
  return id;
}

void cfg_add_edge(cfg_t* cfg, int src, int dest) {
  if (cfg != NULL) {
    cfg_edge_t* edge = malloc(sizeof(cfg_edge_t));
    if (edge != NULL) {
      edge->src = src;
      edge->dest = dest;

      cfg->count_edges++;
      cfg->edges = realloc(cfg->edges, cfg->count_edges * sizeof(cfg_node_t*));
      cfg->edges[cfg->count_edges-1] = edge;
    }
  }
}

void destroy_cfg(cfg_t* cfg) {
  if (cfg != NULL) {
    for (int n = 0; n < cfg->count_nodes; n++)
      free(cfg->nodes[n]);
    for (int e = 0; e < cfg->count_edges; e++)
      free(cfg->edges[e]);
    free(cfg);
  }
}