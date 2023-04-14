#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include "cfg.h"

cfg_t* create_cfg() {
  cfg_t* cfg = malloc(sizeof(cfg_t));
  if (cfg != NULL) {
    cfg->count_nodes = 0;
    cfg->count_edges = 0;
    cfg->nodes = malloc(0);
    cfg->edges = malloc(0);
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
      node->id = cfg->count_nodes;

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
    free(cfg->nodes);
    free(cfg->edges);
    free(cfg);
  }
}

iloc_code_t* get_label_code(iloc_program_t* program, int label) {
  iloc_code_t* label_code = NULL;
  iloc_code_t* code = program->head;
  while (code != NULL) {
    if (code->label == label) {
      label_code = code;
      break;
    }
    code = code->next;
  }
  return label_code;
}

void mark_leaders(iloc_program_t* program) {
  iloc_code_t* code = program->head;
  if (code != NULL)
    code->cfg_leader = true; // first command is leader
  iloc_code_t* label_code = NULL;
  while(code != NULL) {
    switch (code->op) {
    case JUMP: // consider the only time we use JUMP is in the return command, the return address is always an instruction after a function call with a JUMP_I, which is already marked as a leader
      if (code->next != NULL)
        code->next->cfg_leader = true; // command after a branch is leader
      break;

    case JUMP_I:
      if (code->next != NULL)
        code->next->cfg_leader = true; // command after a branch is leader

      label_code = get_label_code(program, code->args[0]);
      if (label_code != NULL)
        label_code->cfg_leader = true; // target of branch is leader
      break;

    case CBR:
      if (code->next != NULL)
        code->next->cfg_leader = true; // command after a branch is leader

      label_code = get_label_code(program, code->args[1]);
      if (label_code != NULL)
        label_code->cfg_leader = true; // target of branch is leader
      label_code = get_label_code(program, code->args[2]);
      if (label_code != NULL)
        label_code->cfg_leader = true; // target of branch is leader
      break;
    }
    code = code->next;
  }
}

iloc_code_t* get_next_leader(iloc_code_t* code) {
  iloc_code_t* leader = NULL;
  iloc_code_t* c = code->next;
  while (c != NULL) {
    if (c->cfg_leader) {
      leader = c;
      break;
    }
    c = c->next;
  }
  return leader;
}

iloc_code_t* get_basic_block_end(iloc_code_t* code) {
  iloc_code_t* end = NULL;
  iloc_code_t* c = code->next;
  while (c != NULL) {
    if (c->cfg_leader) {
      end = c->previous;
      break;
    }
    c = c->next;
  }
  return end;
}

int get_label_basic_block(cfg_t* cfg, int label) {
  int bb = -1;
  for (int i=0; i < cfg->count_nodes; i++) {
    cfg_node_t* node = cfg->nodes[i];
    if (node->start->label == label) {
      bb = node->id;
      break;
    }
  }
  return bb;
}

cfg_t* generate_cfg(iloc_program_t* program) {
  cfg_t* cfg = NULL;
  if (program != NULL) {
    // mark leader statements
    mark_leaders(program);
    // create graph
    cfg = create_cfg();
    iloc_code_t* leader = program->head;
    while (leader != NULL) {
      // add to graph
      iloc_code_t* end = get_basic_block_end(leader);
      if (end == NULL)
        end = program->tail;
      cfg_add_node(cfg, leader, end);
      // get next basic block
      leader = get_next_leader(leader);
    }
    // mark branches and fallthroughs
    // em iloc todos os jumps são incondicionais, então não existe nodo que tenha aresta de branch E aresta de fallthrough
    for (int i=0; i < cfg->count_nodes; i++) {
      cfg_node_t* node = cfg->nodes[i];
      iloc_code_t* end = node->end;
      int label_basic_block = -1;
      switch(end->op) {
      case HALT:
        break;

      case JUMP_I:
        label_basic_block = get_label_basic_block(cfg, end->args[0]);
        if (label_basic_block >= 0)
          cfg_add_edge(cfg, node->id, label_basic_block);
        break;

      case CBR:
        label_basic_block = get_label_basic_block(cfg, end->args[1]);
        if (label_basic_block >= 0)
          cfg_add_edge(cfg, node->id, label_basic_block);

        label_basic_block = get_label_basic_block(cfg, end->args[2]);
        if (label_basic_block >= 0)
          cfg_add_edge(cfg, node->id, label_basic_block);
        break;

      case JUMP:
        for (int i=0; i < cfg->count_nodes; i++) {
          cfg_node_t* n = cfg->nodes[i];
          if (end->function_ret_label == n->start->function_call_label)
            cfg_add_edge(cfg, node->id, n->id);
        }
        break;

      default:
        if (i+1 < cfg->count_nodes) { // se não for o último bloco basico
          cfg_node_t* next_node = cfg->nodes[i+1];
          cfg_add_edge(cfg, node->id, next_node->id);
        }
      }
    }
  }
  return cfg;
}

void print_cfg(cfg_t* cfg) {
  if (cfg != NULL) {
    for (int i=0; i < cfg->count_nodes; i++) {
      cfg_node_t* node = cfg->nodes[i];
      printf("=== Bloco Básico %d ===\n", node->id);
      iloc_code_t* code = node->start;
      while (code != node->end) {
        char* str = iloc_code_to_string(code);
        printf("%s\n", str);
        free(str);
        code = code->next;
      }
      char* str = iloc_code_to_string(code);
      printf("%s\n", str);
      free(str);
      printf("----------------------\n");
    }
    printf("=== Arestas ===\n");
    for (int i=0; i < cfg->count_edges; i++) {
      cfg_edge_t* edge = cfg->edges[i];
      printf(" %d --> %d\n", edge->src, edge->dest);
    }
  }
}

void print_cfg_dot(cfg_t* cfg) {
  if (cfg != NULL) {
    // print do grafo em formato .DOT
    printf("digraph {\n");

    for (int i=0; i < cfg->count_nodes; i++) {
      cfg_node_t* node = cfg->nodes[i];
      printf("%d [label=\"", node->id);
      iloc_code_t* code = node->start;
      while (code != node->end) {
        char* str = iloc_code_to_string(code);
        printf("%s\n", str);
        free(str);
        code = code->next;
      }
      char* str = iloc_code_to_string(code);
      printf("%s\n", str);
      free(str);
      printf("\",shape=box];\n");
    }
    for (int i=0; i < cfg->count_edges; i++) {
      cfg_edge_t* edge = cfg->edges[i];
      printf("%d -> %d\n", edge->src, edge->dest);
    }
    
    printf("}");
  }
}
