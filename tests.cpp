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
  
//   optionLabels=0;
  
 graph_t* grPattern;
 graph_t* grTest;
  
 FILE* fpPattern;
 FILE* fpTest;
 
 int i=0;
 while (i<std::numeric_limits<int>::max()){
  std::string dirPath = "tests_graphs/test" + std::to_string(i) + "/";
  std::string pathPattern = dirPath + "pattern.edg";
  std::string pathTest = dirPath + "test.edg";
  fpPattern = fopen(pathPattern.c_str(), "r");
  fpTest = fopen(pathTest.c_str(), "r");

  // read expected results
  int expected_ullmann_with_labels = -1;
  int expected_ullmann_no_labels = -1;
  int expected_sidt_with_labels = -1;
  int expected_sidt_no_labels = -1;
  int expected_gtsi_with_labels = -1;
  int expected_gtsi_no_labels = -1;
  std::ifstream f_res_ullmann(dirPath + "expected_ullmann");
  std::ifstream f_res_sidt(dirPath + "expected_sidt");
  std::ifstream f_res_gtsi(dirPath + "expected_gtsi");
  
  if (f_res_ullmann.good() and f_res_sidt.good() and f_res_gtsi.good()){
    string sLine;
    getline(f_res_ullmann, sLine);
    expected_ullmann_with_labels = atoi(sLine.c_str());
    getline(f_res_ullmann, sLine);
    expected_ullmann_no_labels = atoi(sLine.c_str());
    
    getline(f_res_sidt, sLine);
    expected_sidt_with_labels = atoi(sLine.c_str());
    getline(f_res_sidt, sLine);
    expected_sidt_no_labels = atoi(sLine.c_str());
    
    getline(f_res_gtsi, sLine);
    expected_gtsi_with_labels = atoi(sLine.c_str());
    getline(f_res_gtsi, sLine);
    expected_gtsi_no_labels = atoi(sLine.c_str());
  }

  f_res_ullmann.close();
  f_res_sidt.close();
  f_res_gtsi.close();
  
    if (fpPattern == NULL || fpTest == NULL){
//       fprintf(stderr, "Can't open pattern or test graph\n");
      break;
    }
    printf("%s", Blue);
    std::cout << "Running test " + std::to_string(i) + "\n";
    printf("%s", Color_Off);
    
    graph_from_file(&grPattern, fpPattern, 1);
    graph_from_file(&grTest, fpTest, 1);
    fclose(fpPattern);
    fclose(fpTest);
    
//     graph_fprint(stdout, grPattern);
//     graph_fprint(stdout, grTest);
    
    // Ullmann
    test_Ullmann(grPattern, grTest, expected_ullmann_with_labels, true, " (Check labels)");
    test_Ullmann(grPattern, grTest, expected_ullmann_no_labels, false, " (Don't check labels)");
    std::cout << "\n";
    // SIDT
    test_SIDT(grPattern, grTest, expected_sidt_with_labels, true, " (Check labels)");
    test_SIDT(grPattern, grTest, expected_sidt_no_labels, false, " (Don't check labels)");
    std::cout << "\n";
    // GTSI
    test_GTSI(grPattern, grTest, expected_gtsi_with_labels, true, " (Check labels)");
    test_GTSI(grPattern, grTest, expected_gtsi_no_labels, false, " (Don't check labels)");
    
    std::cout << "\n";
    i++;
 }
}

void test_Ullmann(graph_t* grPattern, graph_t* grTest, int expected, bool checkLabels, std::string desc){
    std::cout << "Ullmann" + desc + ":\n";
    
    if (checkLabels) optionCheckSymb = 1;
    else optionCheckSymb = 0;
    
    int isoTotal=isoUllman(grPattern, grTest);
    char* color;
    if (isoTotal != expected){
      color = Red;
    }
    else{
      color = Green;
    }
    printf("%sisoTotal: %d (expected: %d).%s\n", color, isoTotal, expected, Color_Off);
}


void test_SIDT(graph_t* grPattern, graph_t* grTest, int expected, bool checkLabels, std::string desc){
    char* color;
    std::cout << "SIDT" + desc + ":\n";
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
    
    char withLabels;
    if (checkLabels) char withLabels=1;
    else withLabels=0;
    
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
    if (n != expected){
      color = Red;
    }
    else{
      color = Green;
    }
    printf("%s%d sites from test graph found matching (expected: %d).%s\n", color, n, expected, Color_Off);
    
    
}

void test_GTSI(graph_t* grPattern, graph_t* grTest, int expected, bool checkLabels, std::string desc){
    char* color;
    int siteSize=grPattern->nodes.count;
    std::cout << "GTSI" + desc + ":\n";
    ParcoursNode* tree = new ParcoursNode();
    tree->addGraph(grPattern, siteSize, 0, checkLabels);
    printf("%d traversals reconstructed from pattern graph.\n", tree->countLeaves());
    
    vsize_t count=tree->parcourir(grTest, siteSize, checkLabels);
    if (count != expected){
      color = Red;
    }
    else{
      color = Green;
    }
    printf("%s%d traversals possible in test graph (expected: %d).%s\n", color, count, expected, Color_Off);

    
}