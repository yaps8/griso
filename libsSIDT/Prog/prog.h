#ifndef INC_PROG_H
#define INC_PROG_H

typedef struct prog {
	char* message;
	unsigned int index;
	unsigned int nMatchedSites;
	unsigned int nSites;
	struct site** sites;
} prog;

typedef struct site {
	unsigned char size;
	unsigned int nProgs;
	struct prog** progs;
} site;

prog* newProg(char* msg, int n);
site* newSite(char size);
void addProg(site* sit, prog* prg);
void addSite(prog* prg, site* sit);

#endif
