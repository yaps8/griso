#ifndef TESTS_H
#define TESTS_H

#include <sys/time.h>
#include <semaphore.h>
#include <stdlib.h>
#include <iostream>
#include <limits>
#include <fstream>
#include <cstdlib>

extern "C" {
#include "graphIO.h"
#include "libGraphBinAlgo.h"
#include "graphParser.h"
}
#include "libSIDT.h"
#include "libGTSI.h"
char optionFuncs;

void printDescription ();
void test_Ullmann (graph_t ** grPattern, int nPattern, graph_t * grTest, int expected, bool checkLabels, std::string desc);
void test_SIDT (graph_t ** grPattern, int nPattern, graph_t * grTest, int expected, bool checkLabels, std::string desc);
void test_GTSI (graph_t ** grPattern, int nPattern, graph_t * grTest, int expected, bool checkLabels, std::string desc);

#endif
