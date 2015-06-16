#include "tests.h"

int main(int argc, char* argv[]){
  Red="\e[1;31m";
  Green="\e[1;32m";
  Color_Off="\e[0m";
  optionMCSByPattern=0;
  optionMCS=0;
  optionNoPerm=0;
  optionCheckSymb=0;
  optionCount=0;
  optionRedInt=0;
  optionRec=0;
  optionQuiet=0;
  optionVerbose=1;
  optionOutSmall=0;
  optionExport=0;
  optionMultiThreaded=0;
  optionForceRoots=0;
  optionIsoOnly=0;
  optionDebug=0;
  optionInfo=0;
  optionOnlyInduced=0;
  nThreads=1;

  FILEeP;
  FILEeT;
  FILEenP;
  FILEenT;
  FILEesP;
  FILEesT;

  mutexExport;
  mutexIsoTotal;
  mutexnSgraphP;
  mutexStructRead;
  mutexBSFPattern;
  isThreadBusy;

  maxFound=0;
  maxPatternFound=0;
  isoTotal=0;
  wP=0;
  wT=0;
  nSgraphP=0;
  
// test0
 graph_t* grPattern;
 graph_t* grTest;
  
 FILE* fpPattern;
 FILE* fpTest;
 
 int i;
 for (i=0; i<5; i++){
  cout << "Running test " + std::to_string(i) + "\n";
  std::string pathPattern = "tests_graphs/test" + std::to_string(i) + "/pattern.edg";
  std::string pathTest = "tests_graphs/test" + std::to_string(i) + "/test.edg";
  fpPattern = fopen(pathPattern.c_str(), "r");
  fpTest = fopen(pathTest.c_str(), "r");

  
    if (fpPattern == NULL || fpTest == NULL){
      printf("Can't open pattern or test graph\n");
      return 1;
    }
    
    graph_from_file(&grPattern, fpPattern);
    graph_from_file(&grTest, fpTest);
    fclose(fpPattern);
    fclose(fpTest);
    
    // Ullmann
    int isoTotal=isoUllman(grPattern, grTest);
    if (isoTotal>=1 && grPattern->nodes.count >=1 && grTest->nodes.count >=1){
	    printf("%sisoTotal: %d%s\n", Red, isoTotal, Color_Off);
    }
    else{
	    //printf("%snot found%s\n", Green, Color_Off);
	    printf("%sisoTotal: %d%s\n", Green, isoTotal, Color_Off);
    }
    
    // GTSI
    
    // SIDT
    
    cout << "\n";
 }
}
