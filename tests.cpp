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
  std::cout << "Running test " + std::to_string(i) + "\n";
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
    
    // SIDT
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
    int siteSize=24;
    char withLabels=1;
    bool optionInfo = false;
    
    int* scanFuncs;
    int valence = 2;
    
    ProgInfo* scanInfo=NULL;
    decisionTree* dt=newDecisionTree(siteSize, valence);
    std::string st = "Pattern";
    char* msg = (char*) st.c_str();
    int n_pattern = learnGraph(grPattern, dt, msg, siteCountLimit, withLabels);
    printf("%d sites learned from %s\n", n_pattern, msg);
    int* countByProg= (int*) calloc(dt->nProgs, sizeof(int));
    
    if (optionFuncs) scanFuncs= (int*) calloc(scanInfo->n, sizeof(int));
    int n=findGraph(grTest, dt, countByProg, scanInfo, scanFuncs, withLabels);
    printf("%d sites of %s found from %s\n", n, pathTest.c_str(), dt->progs[0]->message);
    printf("Pattern graph %s has %d sites ; test graph %s has %d sites found matching.\n", pathPattern.c_str(), n_pattern, pathTest.c_str(), n);
    if (optionFuncs){
	    printf("\nFunctions found: \n");
	    int k;
	    for (k=0; k<scanInfo->n; k++){
		    if (scanFuncs[k]!=0){
			    printf("%d %s\n", scanFuncs[k], scanInfo->functions[k]->name);
		    }
	    }
    }
    if (optionFuncs) free(scanFuncs);
    
    // GTSI
    
    std::cout << "\n";
 }
}
