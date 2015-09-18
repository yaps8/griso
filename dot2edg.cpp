#include "dot2edg.h"

int main(int argc, char* argv[]){
  if (argc <= 2){
    std::cout << "Usage: ./dot2edg (in.dot) (out.edg)\n";
  }

  char* pathIn = argv[1];
  char* pathOut = argv[2];

  graph_t* gr = getGraphFromPath(pathIn);


  FILE* fpOut;
  fpOut = fopen(pathOut, "w");

  if (gr == NULL || fpOut == NULL){
    return 1;
  }

  graph_to_file(gr, fpOut);
  fclose(fpOut);
}