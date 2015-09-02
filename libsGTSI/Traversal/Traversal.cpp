#ifndef TRAVERSAL_CPP
#define TRAVERSAL_CPP

#include "Traversal.hpp"

MotParcours::MotParcours(){
 
}

string MotParcours::toString(){
  string s = "";
  if (this->type == TYPE_M1){
    s += "1: ";
    char* desc = labCharToString(this->symbol);
    s += string(desc);
  }
  else if (this->type == TYPE_M2){
    s += "-";
    if (this->alpha_is_R){
        s += "R> ";
    }
    else {
        s += i2s(this->k);
        s += "> ";
    }
    s += i2s(this->i);
    if (this->has_symbol){
        s += ": ";
        char* desc = labCharToString(this->symbol);
        s += string(desc);
    }
  }
  else {
    s += "ERR";
    printf("ERROR in MotParcours::toString.\n");
  }
  
  return s;
}

bool MotParcours::equals(MotParcours* m){
  if (this->type == m->type){
    if (this->type == TYPE_M1){
      return this->symbol == m->symbol;
    }
    else {
      if (this->alpha_is_R == m->alpha_is_R){
        if (this->alpha_is_R){
          return (this->i == m->i) && (this->has_symbol == m->has_symbol) && ((not this->has_symbol) or (this->symbol == m->symbol));
        }
        else {
          return (this->k == m->k) && (this->i == m->i) && (this->has_symbol == m->has_symbol) && ((not this->has_symbol) or (this->symbol == m->symbol));
        }
      }
      else {
        return false;
      }
    }
  }
  return false;
}

Parcours::Parcours(){
  this->mots = NULL;
  this->size = 0;
}

bool Parcours::equals(Parcours* p){
  if (this->size != p->size) return false;
  
  vsize_t n;
  for (n=0; n<this->size; n++){
    if (not this->mots[n]->equals(p->mots[n])) return false;
  }

  return true;
}


string Parcours::toString(){
  int i;
  string s = "";
  for (i=0; i<this->size; i++){
    s += this->mots[i]->toString();
    s += " ";
  }
  return s;
}


void Parcours::addMot(MotParcours* m){
  this->mots = (MotParcours**) realloc(this->mots, (this->size+1)*sizeof(MotParcours*));
  this->mots[this->size] = m;
  this->size++;
  assert(m->type == TYPE_M1 or m->type == TYPE_M2);
}



Parcours* parcoursLargeur(graph_t* graph, vsize_t vroot, vsize_t W){
  //WARNING : after calling this function, the ->list_id fields for nodes in inputGraph are broken
  Parcours* p = new Parcours();

  //all inputgraph nodes to unexplored(0):
  node_list_t* listI = &(graph->nodes);

  node_t* nI;

  bool p_is_epsilon = true;
  vsize_t s = 0;
  nI = node_list_item(listI, vroot);
  
  assert(nI != NULL);
  
  nI->list_id=(vsize_t) 1;

  std::queue<TupleQueue> queue3;
  queue3.push(std::make_tuple((node_t*) NULL, (vsize_t) 0, nI));
  s++;
  int a=0;
  node_t* child;
  TupleQueue tq;
  node_t* pere;
  node_t* ss;
  uint8_t k;
  node_t* sc = 0;
  MotParcours* m;
  size_t i = 1;
  size_t k2;
  node_t* f;
  
  unordered_set<node_t*> explored;

  while (not queue3.empty()){
    tq=queue3.front();
    pere = std::get<0>(tq);
    k = std::get<1>(tq);
    ss = std::get<2>(tq);


    
    unordered_set<node_t*>::iterator it = explored.find(ss);
    if ((it != explored.end() or i < W + 1) and sc != pere and not p_is_epsilon){
      m = new MotParcours();
      m->type = TYPE_M2;
      m->alpha_is_R = true;
      m->has_symbol = false;
      m->i = pere->list_id;
      p->addMot(m);
      
      sc = pere;
    }
	
    if (it == explored.end() and i < W + 1){		
        if (p_is_epsilon){
          m = new MotParcours();
          m->type = TYPE_M1;
          m->has_symbol = true;
          m->symbol = ss->symb;
          p->addMot(m);
          p_is_epsilon = false;
        }
        else{
          m = new MotParcours();
          m->type = TYPE_M2;
          m->alpha_is_R = false;
          m->i = i;
          m->k = k;
          m->has_symbol = true;
          m->symbol = ss->symb;
          p->addMot(m);
        }
        
        ss->list_id = i;
        i++;
        sc = ss;
        
        assert(ss->children_nb <= 2);
        for (k2=0; k2<ss->children_nb; k2++){
          f = ss->children[k2];
          queue3.push(std::make_tuple(ss, k2, f));
        }
        
        explored.insert(ss);
    }
    else if (it != explored.end()) {
        m = new MotParcours();
        m->type = TYPE_M2;
        m->alpha_is_R = false;
        m->i = ss->list_id;
        m->k = k;
        m->has_symbol = false;
        p->addMot(m);
        sc = ss;
    }
    queue3.pop();
  }
  
    if (i == W + 1){
      p->complete = true;
    }
    else{
      p->complete = false;  
    }
    return p;
}

bool Parcours::parcourirDepuisSommet(graph_t* graph, vsize_t vroot, vsize_t W){
  node_t* sc;
  node_list_t* listI = &(graph->nodes);
  node_t* nI;
  nI = node_list_item(listI, vroot);
  sc = nI;
  unordered_set<node_t*> numerotes;
  
  node_t** numeros = (node_t**) calloc(W, sizeof(node_t*));
  vsize_t max_numeros = 0;
  
  if (this->size >= 1 and this->mots[0]->type==TYPE_M1 and this->mots[0]->symbol == nI->symb){
    numeros[max_numeros] = sc;
    max_numeros++;
    numerotes.insert(sc);
  }
  else {
    return false;
  }
  
  size_t i;
  for (i=1; i<this->size; i++){
    MotParcours* m = this->mots[i];
    if (m->alpha_is_R){
      if (max_numeros >= m->i){
        sc = numeros[m->i-1];
      }
    }
    else {
      if (m->k < sc->children_nb){
        node_t* f = sc->children[m->k];
        unordered_set<node_t*>::iterator it = numerotes.find(f);
        if (it==numerotes.end()){
          // f n'est pas numéroté
          if (f->symb == m->symbol and max_numeros < m->i){
            numeros[max_numeros] = f;
            max_numeros++;
            numerotes.insert(f);
            sc = f;
          }
          else {
            return false;
          }
        }
        else if (not m->has_symbol) {
          sc = f;
        }
        else {
          return false;
        }
      }
      else{
        return false;
      }
    }
  }
  
  return true;
}

bool Parcours::parcourir(graph_t* gr, vsize_t W){
  vsize_t n;
  for (n=0; n<gr->nodes.size; n++){
   if (this->parcourirDepuisSommet(gr, n, W)){
      return true;
   }
  }
  return false;
}

unordered_set< Parcours* > parcoursFromGraph(graph_t* gr, vsize_t W){
  unordered_set<Parcours*> parcours;
  Parcours* p;
  vsize_t n;
  
  for (n=0; n<gr->nodes.size; n++){
   p = parcoursLargeur(gr, n, W);

   if (p->complete){
     // check if duplicate:
     unordered_set<Parcours*>::iterator it;
     bool new_p = true;
     for (it=parcours.begin(); it!=parcours.end(); it++){
       Parcours* p2 = *it;
       if (p->equals(p2)){
          new_p = false;
          break;
       }
     }
    if (new_p) parcours.insert(p);
   }
  }
  
  return parcours;
}

ParcoursNode::ParcoursNode(){
  this->id = 0;
  this->feuille = false;
}

ParcoursNode::ParcoursNode(std::list< ParcoursNode* > fils, MotParcours* mot, uint64_t id){
  this->fils = fils;
  this->mot = mot;
  this->id = id;
}

vsize_t ParcoursNode::addGraph(graph_t* gr, vsize_t W, vsize_t maxLearn){
  Parcours* p;
  vsize_t n;
  vsize_t added=0;
  
  for (n=0; n<gr->nodes.size; n++){
    if (maxLearn == 0 || added < maxLearn){
      p = parcoursLargeur(gr, n, W);
      
      if (p->size > 1){
	std::cout << p->toString() + "\n";
      }

      if (p->complete){
          if (this->addParcours(p, 0)){
            added++;
          }
        }
    }
    else{
      break;
    }
  }
  return added;
}

string ParcoursNode::toString()
{
  string s;
  s += this->mot->toString();
  list<ParcoursNode*>::iterator it;
  for (it=this->fils.begin(); it!=this->fils.end(); it++){
    s += "fils:";
    s += (*it)->toString();
  }
  return s;
}

string ParcoursNode::toDotPartiel()
{
  string s;
  s += "\"";
  s += i2s(this->id);
  if (not this->feuille) s += "\" [label=\"";
  else s += "\" [label=\"F ";
  s += h2s(this->id);
  s += "\"]\n";
  list<ParcoursNode*>::iterator it;
  for (it=this->fils.begin(); it!=this->fils.end(); it++){
    ParcoursNode* f = (*it);
    s += "\"";
    s += i2s(this->id);
    s += "\" -> \"";
    s += i2s(f->id);
    s += "\" [label=\"";
    s += f->mot->toString();
    s += "\"]\n";
    s += f->toDotPartiel();
  }
  return s;
}

string ParcoursNode::toDot()
{
  string s = "digraph G {\n";
  s += this->toDotPartiel();
  s += "\n}";
  return s;
}

void ParcoursNode::saveParcoursNodeToDot(){
  string path = "t.dot";
  ofstream ofs (path);
  string str = this->toDot();
  ofs << str;
  ofs.close();
}

bool ParcoursNode::addParcours(Parcours* p, int index){
  if (index >= p->size){
	bool b = this->feuille;
	this->feuille = true;
    return not b;
  }
  MotParcours* m = p->mots[index];
  list<ParcoursNode*>::iterator it;
  for (it=this->fils.begin(); it!=this->fils.end(); it++){
    ParcoursNode* f = (*it);
    if (f->mot->equals(m)){
      return f->addParcours(p, index+1);
    }
  }
  
  ParcoursNode* pn = new ParcoursNode();
  pn->mot = m;
  pn->id = (uint64_t) pn;

  this->fils.push_back(pn);
  return pn->addParcours(p, index+1);
}

vsize_t ParcoursNode::parcourir(graph_t* gr, vsize_t W){
  vsize_t count=0;
  vsize_t n;
  std::set<vsize_t> leaves;
  for (n=0; n<gr->nodes.size; n++){
   list<vsize_t> ret = this->parcourirDepuisSommet(gr, n, W);
   list<vsize_t>::iterator it = ret.begin();
   
   for (it = ret.begin(); it!=ret.end(); it++){
	   vsize_t id = *it;
      if (get<1>(leaves.insert(id))){
	    count++;
	  }
   }
  }
  return count;
}

list<vsize_t> ParcoursNode::parcourirDepuisSommet(graph_t* gr, vsize_t v, vsize_t W){
  unordered_set<node_t*> numerotes;
  node_t* r = node_list_item(&gr->nodes, v);
  
  node_t** numeros = (node_t**) calloc(W, sizeof(node_t*));
  vsize_t max_numeros = 0;
  return this->parcourirDepuisSommetRec(true, gr, r, W, numeros, max_numeros, numerotes);
}


list<vsize_t> ParcoursNode::parcourirDepuisSommetRec(bool racine, graph_t* gr, node_t* r, vsize_t W, node_t** numeros, vsize_t max_numeros,  unordered_set<node_t*> numerotes)
{
  list<vsize_t> l;
  
  if (this->feuille){
	  l.push_back(this->id);
  }
 
    
    assert(this->feuille or racine or not this->fils.empty());
    
    list<ParcoursNode*>::iterator it;
    for (it=this->fils.begin(); it!=this->fils.end(); it++){
      ParcoursNode* f = (*it);
      RetourEtape ret = etape(f->mot, r, gr, numeros, max_numeros, numerotes);
      bool possible = get<0>(ret);
      node_t* node = get<1>(ret);
      numeros = get<2>(ret);
      vsize_t max_numeros_r = get<3>(ret);
      unordered_set<node_t*> numerotes_r = get<4>(ret);
      
      if (possible){
        list<vsize_t> l2 = f->parcourirDepuisSommetRec(false, gr, node, W, numeros, max_numeros_r, numerotes_r);
        l.splice(l.begin(), l2);
      }
    }
//   }
  return l;;
}

ParcoursNode::RetourEtape ParcoursNode::etape(MotParcours* m, node_t* s, graph_t* gr, node_t** numeros, vsize_t max_numeros,  unordered_set<node_t*> numerotes){
  if (m->type == TYPE_M1){
    if (s->symb == m->symbol){
      
      assert(max_numeros==0);
      
      numeros[max_numeros] = s;
      max_numeros++;
      numerotes.insert(s);
      return std::make_tuple(true, s, numeros, max_numeros, numerotes);
    }
    else {
      return std::make_tuple(false, s, numeros, max_numeros, numerotes);
    }
  }
  else if (m->type == TYPE_M2){
    if (m->alpha_is_R){
      if (max_numeros >= m->i){
        s = numeros[m->i-1];
        return std::make_tuple(true, s, numeros, max_numeros, numerotes);
      }
      else {
        return std::make_tuple(false, s, numeros, max_numeros, numerotes);
      }
    }
    else {
      if (m->k < s->children_nb){
        node_t* f = s->children[m->k];
        unordered_set<node_t*>::iterator it = numerotes.find(f);
        if (it==numerotes.end()){
          // f n'est pas numéroté
          if (f->symb == m->symbol and max_numeros < m->i){
	    
            assert(max_numeros == m->i - 1);
	    
            numeros[max_numeros] = f;
            max_numeros++;
            numerotes.insert(f);
            return std::make_tuple(true, f, numeros, max_numeros, numerotes);
          }
          else {
            return std::make_tuple(false, s, numeros, max_numeros, numerotes);
          }
        }
        else if (not m->has_symbol) {
          return std::make_tuple(true, f, numeros, max_numeros, numerotes);
        }
        else {
          return std::make_tuple(false, s, numeros, max_numeros, numerotes);
        }
      }
      else{
        return std::make_tuple(false, s, numeros, max_numeros, numerotes);
      }
    }
  }
  else {
    printf("ERR: UNKNOWN TYPE.\n");
    return std::make_tuple(false, s, numeros, max_numeros, numerotes);
  }
}


vsize_t ParcoursNode::countLeaves(){
  if (this->fils.empty()){
    return 1;
  }
  else {
    vsize_t somme = 0;
    list<ParcoursNode*>::iterator it;
    for (it=this->fils.begin(); it!=this->fils.end(); it++){
      ParcoursNode* f = (*it);
      somme += f->countLeaves();
    }
    return somme;
  }
}

vsize_t ParcoursNode::countFinal(){
  vsize_t count = 0;
  if (this->feuille){
    count++;
  }
  
  list<ParcoursNode*>::iterator it;
  for (it=this->fils.begin(); it!=this->fils.end(); it++){
	ParcoursNode* f = (*it);
	count += f->countFinal();
  }
  return count;
}

#endif
