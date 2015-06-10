#include "decisionTree.h"

decisionTree* newDecisionTree(int siteSize, char siteValence){
	decisionTree* dt=malloc(sizeof(decisionTree));
	dt->siteSize=siteSize;
	dt->siteValence=siteValence;
	dt->root=NULL;
	dt->nSites=0;
	dt->sites=NULL;
	dt->nProgs=0;
	dt->progs=NULL;
	dt->nLearntSites=0;

	return dt;
}

void addProgToDT(decisionTree* dt, prog* prg){
	dt->nProgs++;
	dt->progs=realloc(dt->progs, dt->nProgs*sizeof(prog*));
	dt->progs[dt->nProgs-1]=prg;
}

void addSiteToDT(decisionTree* dt, site* sit){
	dt->nSites++;
	dt->sites=realloc(dt->sites, dt->nSites*sizeof(site*));
	dt->sites[dt->nSites-1]=sit;
}

int learnSite(decisionTree* dt, NGraph* site, prog* prg, char withLabels){
	learnSiteR(dt, &(dt->root), site, 1, prg, 1, withLabels);
}

int findSite(decisionTree* dt, NGraph* site, char withLabels){
	return learnSiteR(dt, &(dt->root), site, 1, NULL, 0, withLabels);
}

int learnSiteR(decisionTree* dt, decisionNode** dn, NGraph* siteG, int i, prog* prg, char learn, char withLabels){
	if (*dn==NULL){
		if (!learn) return 0; // decisionNode does not exist, the site cannot be found
		// adding corresponding node

		*dn=newDecisionNode();
		if (i==dt->siteSize+1){
			// creating new site
			site* sit=newSite(dt->siteValence);
			addSiteToDT(dt, sit);
			addProg(sit, prg);
			addSite(prg, sit);
			(*dn)->i=dt->nSites; // 0 means no site
      /*printf("2\n");*/
			return 0;
		}
		else {
			// creating the matching in hashTable
			(*dn)->i=i;
			(*dn)->ht=newHashTable(2+i*dt->siteValence); //1+i*dt->siteValence+1 (k=2)
			decisionNode** dnp=malloc(sizeof(decisionNode*));
			*dnp=NULL;
			addElement((*dn)->ht, withLabels, siteG->symbols[i-1], siteG->NMatrix[(i-1)*dt->siteValence], siteG->NMatrix[(i-1)*dt->siteValence+1], learn, dnp);
			// continue browsing down
			return learnSiteR(dt, dnp, siteG, i+1, prg, learn, withLabels);
		}
	}
	else {
		if (i==dt->siteSize+1){
			if ((*dn)->i!=0){
				// found existing site
				if (learn){
					// is the prg already having this site ?
					site* sit=dt->sites[(*dn)->i-1];
					int j;
					char addPrg=1;
					for (j=0; j<sit->nProgs; j++){
						if (sit->progs[j]==prg){
							addPrg=0;
							break;
						}
					}

					if (addPrg){
						addProg(sit, prg);
						addSite(prg, sit);
					}
				}
			}
			//will return the number of the site found, 0 if no site
      /*printf("1, %d\n", (*dn)->i);*/
			return (*dn)->i;
		}
		else {
			// looks for an existing matching in hashTable
			decisionNode** dnp=malloc(sizeof(decisionNode*));
			*dnp=NULL;
			decisionNode** searchDn = addElement((*dn)->ht, withLabels, siteG->symbols[i-1], siteG->NMatrix[(i-1)*dt->siteValence], siteG->NMatrix[(i-1)*dt->siteValence+1], learn, dnp);
			if (!learn && searchDn==NULL){ // in this case the search does not succeed and not in learn mode : abort
				free(dnp);
        /*printf("2\n");*/
				return 0;
			}

			if (*searchDn!=NULL) { // in that case the child already exists : dnp is useless
				free(dnp);
			}

			// continue browsing down
			return learnSiteR(dt, searchDn, siteG, i+1, prg, learn, withLabels);
		}
	}
}

int countNodesDT(decisionTree* dt){
	if (dt!=NULL) return countNodesDN(dt->root);
	else return 0;
}

int countLeavesDT(decisionTree* dt){
  if (dt!=NULL) return countLeavesDN(dt->root);
  else return 0;
}

void freeDecisionTree(decisionTree* dt){
	if (dt->root != NULL) {
		freeDecisionNode(dt->root);
	}
//	else {
//		free(dt->root);
//		free(&(dt->root));
//	}

	int i, k;
	for (i=0; i<dt->nSites; i++){
//		for (k=0; k<dt->sites[k]->nProgs; k++){
//			free(dt->sites[i]->progs[k]);
//		}
		free(dt->sites[i]->progs);
		free(dt->sites[i]);
	}

	for (i=0; i<dt->nProgs; i++){
//		for (k=0; k<dt->progs[k]->nSites; k++){
//			free(dt->progs[i]->sites[k]);
//		}
		free(dt->progs[i]->message);
		free(dt->progs[i]->sites);
		free(dt->progs[i]);
	}

	free(dt->sites);
	free(dt->progs);
	free(dt);
}
