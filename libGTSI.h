#ifndef LIBGTSI_H
#define LIBGTSI_H

#include <stdio.h>
extern "C" {
#include "graphIO.h"
}
#include "Traversal.hpp"
#include "utils-gtsi.h"
#include <unordered_set>

extern char optionFuncs;
char* labCharToString(vsize_t label);
#endif
