#include "nodeIO.h"

size_t node_to_dot(const node_t* node, const node_t* root, size_t node_number,FILE* fp) {
  size_t ret;

  ret = fprintf(fp, "\"%"PRIx64"\" [label = \"%"PRIx64"(%d) : ", node->node_id, node->node_id, node_number);
  char* symb;
  
  switch (node->symb) {
  case INST_SEQ:
    symb = "INST";
    break;
  case INST_RET:
    symb = "RET";
    break;
  case INST_CALL:
    symb = "CALL";
    break;
  case INST_JCC:
    symb = "JCC";
    break;
  case INST_JMP:
    symb = "JUMP";
    break;
  case INST_END:
    symb = "HLT";
    break;
  case INST_UREACH:
    symb = "UNREACHEABLE";
    break;
  case INST_UNDEF:
    symb = "UNDEFINED";
    break;
  case INST_SCALL:
    symb = "INT";
    break;
  default:
  case SYMB_END:
    symb = "ERROR";
    break;
  }
  
  ret += fprintf(fp, symb);
  ret += fprintf(fp, "\"");
  ret += fprintf(fp, ", symb = ");
  ret += fprintf(fp, symb);
  
  if (node == root)
     ret += fprintf(fp, ", style=\"bold,filled\", fillcolor=yellow]\n");
  else
	  ret += fprintf(fp, "]\n");

  return ret;
}

size_t node_edges_to_dot(const node_t* node, FILE* fp) {
  vsize_t j;
  size_t ret;

  ret = 0;
  for (j = 0; j < node->children_nb; ++j) {
    const node_t* child = node_child_const(node, j);
    ret += fprintf(fp, "\"%"PRIx64"\" -> \"%"PRIx64"\" [label = \"%d\"]", node->node_id, child->node_id, j);
    ret += fprintf(fp, "\n");
  }
  return ret;
}

size_t node_to_file(const node_t* node, FILE* fp) {
  size_t ret, count;

  ret = 0;

  count = fwrite_le_swap(&node->node_id, sizeof(node->node_id), 1, fp);
  ret += count * sizeof(node->node_id);

  count = fwrite_le_swap(&node->symb, sizeof(node->symb), 1, fp);
  ret += count * sizeof(node->symb);

  return ret;
}

size_t node_edges_to_file(const node_t* node, FILE* fp) {
  vsize_t j;
  size_t ret;

  ret = 0;
  for (j = 0; j < node->children_nb; ++j) {
    const node_t* child;

    child = node_child_const(node, j);

    putc('e', fp);
    ret++;

    ret += fwrite_le_swap(&node->node_id, sizeof(node->node_id), 1, fp) * sizeof(node->node_id);
    ret += fwrite_le_swap(&child->node_id, sizeof(child->node_id), 1, fp) * sizeof(child->node_id);
  }
  return ret;
}

size_t node_from_file(node_t* node, FILE* fp) {
  size_t ret;

  ret = 0;
  ret += fread_le_swap(&node->node_id, sizeof(node->node_id), 1, fp) * sizeof(node->node_id);
  ret += fread_le_swap(&node->symb, sizeof(node->symb), 1, fp) * sizeof(node->symb);

  return ret;
}
