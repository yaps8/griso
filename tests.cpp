#include "tests.h"

void printDescription()
{
  std::cout << "Tests are done as follows.\n";
  std::cout << "There are test folders. Each test folder:\n";
  std::cout << "  Tests with every method (Ullmann, SIDT, GTSI):\n";
  std::cout << "    pattern_*.dot are learnt.\n";
  std::cout << "    test.dot is tested against learnt graphs.\n";
  std::cout << "    Once with symbols (labels) checking, once without.\n";
  std::cout << "\n";
  std::cout << "Ullmann:\n";
  std::cout << "  No learning.\n";
  std::cout << "  Counting the subgraph isomorphisms between each pattern graph and the test graph.\n";
  std::cout << "  The result is the addition.\n";
  std::cout << "\n";
  std::cout << "SIDT:\n";
  std::cout << "  Pattern graphs are cut into sites (site size: minimum size between pattern graphs).\n";
  std::cout << "  Learning: via decision tree.\n";
  std::cout << "  Testing: via decision tree (with sites)\n";
  std::cout << "  The result is the number of matching sites.\n";
  std::cout << "\n";
  std::cout << "GTSI:\n";
  std::cout << "  Pattern graphs are cut into sites (site size: minimum size between pattern graphs).\n";
  std::cout << "  Learning: via traversal tree.\n";
  std::cout << "  Testing: via traversal tree (with sites)\n";
  std::cout << "  The result is the number of possible traversals.\n";
  std::cout << "\n";
  std::cout << "Test 0: small identical graphs.\n";
  std::cout << "Test 1: small test graph with one child more (to a leaf) than pattern.\n";
  std::cout << "Test 2: small test graph with one edge more (leaf -> leaf) than pattern.\n";
  std::cout << "Test 3: small test graph with one child more (to a leaf) and an edge more (leaf -> root) than pattern.\n";
  std::cout << "Test 4: same as test 3 but with different labels.\n";
  std::cout << "Test 5: small test graph with two JCC (that have two children).\n";
}


int main(int argc, char* argv[]){
  if (argc >= 2){
    printDescription();
    return 1;
  }
  
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
  
 graph_t** grPattern;
 graph_t* grTest;
  
 FILE* fpPattern;
 FILE* fpTest;
 
 int i=0;
 while (i<std::numeric_limits<int>::max()){
  std::string dirPath = "tests_graphs/test" + std::to_string(i) + "/";
  std::string pathTest = dirPath + "test.edg";
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
  
    if (fpTest == NULL){
//       fprintf(stderr, "Can't open pattern or test graph\n");
      break;
    }
    printf("%s", Blue);
    std::cout << "Running test " + std::to_string(i) + "\n";
    printf("%s", Color_Off);
    
    int j = 0;
    int nPattern = 0;
    grPattern = (graph_t**) std::malloc(sizeof(graph_t*));
    
    while (j<std::numeric_limits<int>::max()){
      std::string pathPattern = dirPath + "pattern_" + to_string(j) + ".edg";
      grPattern = (graph_t**) std::realloc(grPattern, (j+1) * sizeof(graph_t*));
      fpPattern = fopen(pathPattern.c_str(), "r");
      
      if (fpPattern == NULL) break;
      graph_from_file(&grPattern[j], fpPattern, 1);
      fclose(fpPattern);
      j++;
      nPattern++;
    }
    
    if (nPattern == 0) break;
    
//     graph_from_file(&grPattern, fpPattern, 1);
    graph_from_file(&grTest, fpTest, 1);
    fclose(fpTest);
    
//     graph_fprint(stdout, grPattern);
//     graph_fprint(stdout, grTest);
    
    // Ullmann
    test_Ullmann(grPattern, nPattern, grTest, expected_ullmann_with_labels, true, " (Check labels)");
    test_Ullmann(grPattern, nPattern, grTest, expected_ullmann_no_labels, false, " (Don't check labels)");
    std::cout << "\n";
    // SIDT
    test_SIDT(grPattern, nPattern, grTest, expected_sidt_with_labels, true, " (Check labels)");
    test_SIDT(grPattern, nPattern, grTest, expected_sidt_no_labels, false, " (Don't check labels)");
    std::cout << "\n";
    // GTSIs
    test_GTSI(grPattern, nPattern, grTest, expected_gtsi_with_labels, true, " (Check labels)");
    test_GTSI(grPattern, nPattern, grTest, expected_gtsi_no_labels, false, " (Don't check labels)");
    
    std::cout << "\n";
    i++;
 }
}

void test_Ullmann(graph_t** grPattern, int nPattern, graph_t* grTest, int expected, bool checkLabels, std::string desc){
    std::cout << "Ullmann" + desc + ":\n";
    
    if (checkLabels) optionCheckSymb = 1;
    else optionCheckSymb = 0;
    
    int i;
    int isoTotal = 0;
    
    for (i = 0; i < nPattern; i++){
      int res = isoUllman(grPattern[i], grTest);
      isoTotal += res;
    }
      
    char* color;
    if (isoTotal != expected){
      color = Red;
    }
    else{
      color = Green;
    }
      
    printf("%sisoTotal: %d (expected: %d).%s\n", color, isoTotal, expected, Color_Off);
}


void test_SIDT(graph_t** grPattern, int nPattern, graph_t* grTest, int expected, bool checkLabels, std::string desc){
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
    
    int i;
    int siteSize=grPattern[0]->nodes.count;
    for (i = 1; i < nPattern; i++){
      if (grPattern[i]->nodes.count < siteSize) siteSize = grPattern[i]->nodes.count;
    }
    
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
    
    int learnt_pattern = 0;
    for (i = 0; i < nPattern; i++){
      learnt_pattern += learnGraph(grPattern[i], dt, msg, siteCountLimit, withLabels);
    }
    
    printf("%d sites learned from pattern graph.\n", learnt_pattern);
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

void test_GTSI(graph_t** grPattern, int nPattern, graph_t* grTest, int expected, bool checkLabels, std::string desc){
    char* color;
    std::cout << "GTSI" + desc + ":\n";
    
    int i;
    int siteSize=grPattern[0]->nodes.count;
    for (i = 1; i < nPattern; i++){
      if (grPattern[i]->nodes.count < siteSize) siteSize = grPattern[i]->nodes.count;
    }
    
    ParcoursNode* tree = new ParcoursNode();
    
    for (i = 0; i < nPattern; i++){
      tree->addGraph(grPattern[i], siteSize, 0, checkLabels);
    }
    
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