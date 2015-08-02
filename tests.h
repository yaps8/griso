#include <sys/time.h>
#include <semaphore.h>
#include <stdlib.h>
#include <iostream>
#include <limits>

extern "C" {
#include "graphIO.h"
#include "libGraphBinAlgo.h"
}

#include "libSIDT.h"
#include "libGTSI.h"
char optionFuncs;

// #include "GTSI.h"

void test_Ullmann(graph_t* grPattern, graph_t* grTest);
void test_SIDT(graph_t* grPattern, graph_t* grTest);
void test_GTSI(graph_t* grPattern, graph_t* grTest);