#include "tests.h"

int main(int argc, char* argv[]){
// test0
  graph_t* grPattern;
  graph_t* grTest;
  
  FILE* fpPattern = fopen("tests_graphs/test0/pattern.edg", "r");
  FILE* fpTest = fopen("tests_graphs/test0/test.edg", "r");

  if (fpPattern == NULL || fpTest == NULL){
    printf("Can't open pattern or test graph\n");
    return 1;
  }
  
  graph_from_file(&grPattern, fpPattern);
  graph_from_file(&grTest, fpTest);
  fclose(fpPattern);
  fclose(fpTest);
  
  int isoTotal=isoUllman(grPattern, grTest);
//   if (isoTotal>=1 && grPattern->nodes.count >=1 && grTest->nodes.count >=1){
// 	  printf("%sisoTotal: %d%s\n", Red, isoTotal, Color_Off);
//   }
//   else{
// 	  //printf("%snot found%s\n", Green, Color_Off);
// 	  printf("%sisoTotal: %d%s\n", Green, isoTotal, Color_Off);
//   }
}