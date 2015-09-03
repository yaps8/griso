#include "edg2dot.h"

int main(int argc, char* argv[]){
  if (argc <= 2){
    std::cout << "Usage: ./edg2dot (in.edg) (out.dot)\n";
  }

  char* pathIn = argv[1];
  char* pathOut = argv[2];

  graph_t* gr;

  FILE* fpIn;
  FILE* fpOut;

  fpIn = fopen(pathIn, "r");
  fpOut = fopen(pathOut, "w");

  if (fpIn == NULL || fpOut == NULL){
    return 1;
  }

  graph_from_file(&gr, fpIn, 1);
  fclose(fpIn);

  graph_fprint(fpOut, gr);
  fclose(fpOut);
}