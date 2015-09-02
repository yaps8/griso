#ifndef TRAVERSAL_HPP
#define TRAVERSAL_HPP


#include <tuple>
#include <unordered_set>
#include <set>
#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <forward_list>
#include <queue>
#include "utils-gtsi.h"
// #include "GTSI.h"
#include <fstream>
#include <assert.h>
using namespace std;

extern "C" {
#include "symbol.h"
#include "graph.h"
}

// #include "libGTSI.h"

extern char* labCharToString(vsize_t label);

enum TypeMotParcours {
 TYPE_M1, 
 TYPE_M2
};

class MotParcours{
public:
  char type;
  bool has_symbol;
  symb_t symbol;
  int i;
  bool alpha_is_R;
  int k;
  MotParcours();
  string toString();
  bool equals(MotParcours* m);
};


class Parcours{
public:
  bool complete;
  int size;
  MotParcours** mots;
  Parcours();
  string toString();
  void addMot(MotParcours* m);
  bool parcourirDepuisSommet(graph_t*, vsize_t root, vsize_t W);
  bool parcourir(graph_t*, vsize_t W);
  bool equals(Parcours*);
};

typedef std::tuple<node_t*, uint8_t, node_t*> TupleQueue;
Parcours* parcoursLargeur(graph_t* graph, vsize_t root, vsize_t W);
Parcours* newParcours();
MotParcours* newMotParcours();
std::unordered_set<Parcours*> parcoursFromGraph(graph_t*, vsize_t);

class ParcoursNode{
public:
    uint64_t id;
    bool feuille;
    MotParcours* mot;
    std::list<ParcoursNode*> fils;
    ParcoursNode();
    ParcoursNode(std::list< ParcoursNode* > fils, MotParcours* mot, uint64_t id);
    vsize_t addGraph(graph_t*, vsize_t W, vsize_t maxLearn);
    bool addParcours(Parcours* p, int index);
    void saveParcoursNodeToDot();
    string toDotPartiel();
    string toDot();
    string toString();
    vsize_t parcourir(graph_t* gr, vsize_t W);
    typedef std::tuple<bool, vsize_t> RetourParcourir;
    list< vsize_t > parcourirDepuisSommetRec(bool racine, graph_t* gr, node_t* r, vsize_t W, node_t** numeros, vsize_t max_numeros, std::unordered_set< node_t* > numerotes);
    list<vsize_t> parcourirDepuisSommet(graph_t*, vsize_t r, vsize_t W);
    typedef std::tuple<bool, node_t*, node_t**, vsize_t,  unordered_set<node_t*>> RetourEtape;
    RetourEtape etape(MotParcours* m, node_t*, graph_t*, node_t**, vsize_t,  unordered_set<node_t*>);
    vsize_t countLeaves();
    vsize_t countFinal();
};


#endif
