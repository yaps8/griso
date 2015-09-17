#include "prog.h"

prog* newProg(char* msg, int n){
	prog* prg=malloc(sizeof(prog));
	prg->message=msg;
	prg->index=n;
	prg->nMatchedSites=0;
	prg->sites=NULL;
	return prg;
}

site* newSite(char size){
	site* sit=malloc(sizeof(site));
	sit->size=size;
	sit->nProgs=0;
	sit->progs=NULL;
	return sit;
}

void addProg(site* sit, prog* prg){
	sit->nProgs++;
	sit->progs=realloc(sit->progs, sit->nProgs*sizeof(prog*));
//	printf("prog: %d %s\n", sit->nProgs-1, prg->message);
	sit->progs[sit->nProgs-1]=prg;
}

void addSite(prog* prg, site* sit){
	prg->nMatchedSites++;
	prg->sites=realloc(prg->sites, prg->nMatchedSites*sizeof(site*));
	prg->sites[prg->nMatchedSites-1]=sit;
}

