#include "tests.h"

int main(int argc, char* argv[]){
  char* color;
  Red="\e[1;31m";
  Green="\e[1;32m";
  char* Blue="\e[1;33m";
  Color_Off="\e[0m";
  optionMCSByPattern=0;
  optionMCS=0;
  optionNoPerm=0;
  optionCheckSymb=0;
  optionCount=1;
  optionRedInt=0;
  optionRec=0;
  optionQuiet=0;
  optionVerbose=0;
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
  
  optionLabels=0;
  
// test0
 graph_t* grPattern;
 graph_t* grTest;
  
 FILE* fpPattern;
 FILE* fpTest;
 
 int i;
 for (i=0; i<5; i++){
  printf("%s", Blue);
  std::cout << "Running test " + std::to_string(i) + "\n";
  printf("%s", Color_Off);
  std::string pathPattern = "tests_graphs/test" + std::to_string(i) + "/pattern.edg";
  std::string pathTest = "tests_graphs/test" + std::to_string(i) + "/test.edg";
  fpPattern = fopen(pathPattern.c_str(), "r");
  fpTest = fopen(pathTest.c_str(), "r");

  
    if (fpPattern == NULL || fpTest == NULL){
      printf("Can't open pattern or test graph\n");
      return 1;
    }
    
    graph_from_file(&grPattern, fpPattern, optionLabels);
    graph_from_file(&grTest, fpTest, optionLabels);
    fclose(fpPattern);
    fclose(fpTest);
    
    // Ullmann
    std::cout << "Ullmann:\n";
    int isoTotal=isoUllman(grPattern, grTest);
    if (isoTotal>=1){
      color = Red;
    }
    else{
      color = Green;
    }
    printf("%sisoTotal: %d%s\n", color, isoTotal, Color_Off);
    
    
    // SIDT
    std::cout << "SIDT:\n";
    optionFuncs = 0;
    char optionLearn=0;
    char optionLearnList=0;
    char learnOk=0;
    char scanOk=0;
    char optionTest=0;
    char optionShowStats=0;
    char optionOutputDt=0;
    char* pathDT;
    int siteCountLimit=0;
    int siteSize=grPattern->nodes.count;
    char withLabels=0;
    bool optionInfo = false;
    
    int* scanFuncs;
    int valence = 2;
    
    char debug = 1;
    
    ProgInfo* scanInfo=NULL;
    decisionTree* dt=newDecisionTree(siteSize, valence);
    std::string st = "Pattern";
    char* msg = (char*) st.c_str();
    int n_pattern = learnGraph(grPattern, dt, msg, siteCountLimit, withLabels);
    printf("%d sites learned from pattern graph.\n", n_pattern);
    int* countByProg= (int*) calloc(dt->nProgs, sizeof(int));
    
    int n=findGraph(grTest, dt, countByProg, scanInfo, scanFuncs, withLabels);
    if (n>=1){
      color = Red;
    }
    else{
      color = Green;
    }
    printf("%s%d sites from test graph found matching.%s\n", color, n, Color_Off);
    
    
    // GTSI
    std::cout << "GTSI:\n";
    ParcoursNode* tree = new ParcoursNode();
    tree->addGraph(grPattern, siteSize, 0);
    printf("%d traversals reconstructed from pattern graph.\n", tree->countLeaves());
    
    vsize_t count=tree->parcourir(grTest, siteSize);
    if (count>=1){
      color = Red;
    }
    else{
      color = Green;
    }
    printf("%s%d traversals possible in test graph.%s\n", color, count, Color_Off);

    
    std::cout << "\n";
 }
}
