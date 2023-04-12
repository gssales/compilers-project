#include "cfg.h"

cfg_t* create_cfg();
int cfg_add_node(iloc_code_t* start, iloc_code_t* end);
void cfg_add_edge(int src, int dest);
void destroy_cfg(cfg_t* cfg);