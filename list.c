// © by Theresa Schuettig, s79136

#include <stdio.h>
#include <stdlib.h>
#include "list.h"

sList *createList(){
	sList *pl = malloc(sizeof(sList));
	if (!pl) return NULL;

	sNode *pn = malloc(sizeof(sNode));
	if (!pn) return NULL;

	pn->pData = NULL;
	pn->pNxt = pn;
	pn->pPrv = pn;
	pl->pFront = pn;
	pl->pCurr = pn;
	return pl;
}

void deleteList(sList *pl){
	for (front(pl); current(pl); next(pl)){
		rmCurr(pl);
	}
	free(pl);
}

int insertBeforeCurr(sList *pl, void *pData){
	sNode *pn = malloc(sizeof(sNode));
	if (!pn) return -1;
	pn->pData = pData;
	pn->pPrv = pl->pCurr->pPrv;
	pn->pNxt = pl->pCurr;
	pl->pCurr->pPrv->pNxt = pn;
	pl->pCurr->pPrv = pn;
	return 0;
}

int insertBehindCurr(sList *pl, void *pData){
	sNode *pn = malloc(sizeof(sNode));
	if (!pn) return -1;
	pn->pData = pData;
	pn->pPrv = pl->pCurr;
	pn->pNxt = pl->pCurr->pNxt;
	pl->pCurr->pNxt->pPrv = pn;
	pl->pCurr->pNxt = pn;
	return 0;
}

void rmCurr(sList *pl){
	if (pl->pCurr->pData==NULL) return;
	sNode *pn = pl->pCurr;
	pl->pCurr->pPrv->pNxt = pl->pCurr->pNxt;
	pl->pCurr->pNxt->pPrv = pl->pCurr->pPrv;
	pl->pCurr = pl->pCurr->pPrv;
	free(pn);
}

int push_front(sList *pl, void *pData){
	pl->pCurr = pl->pFront;
	return insertBehindCurr(pl, pData);
}

int push_back(sList *pl, void *pData){
	pl->pCurr = pl->pFront;
	return insertBeforeCurr(pl, pData);
}
void pop_front(sList *pl){
	pl->pCurr = pl->pFront->pNxt;
	rmCurr(pl);
}

void pop_back(sList *pl){
	pl->pCurr = pl->pFront->pPrv;
	rmCurr(pl);
}

void *removeItem(sList *pl, void *pValue, cmp cmpValue){
	void *pData;
	for (pData=front(pl); pData; pData=next(pl)){
		if (cmpValue(pValue, pData)==0) {
			rmCurr(pl);
			return pData;
		}
	}
	return NULL;
}

void insertSorted(sList *pl, void *pData, cmp cmpData){
	int inserted = 0;
	for (front(pl); current(pl); next(pl)){
		if (cmpData(pData, current(pl)) < 0) {
			insertBeforeCurr(pl, pData);
			inserted = 1;
			break;
		}
	}
	if (!current(pl) && !inserted) push_back(pl, pData);
}

void *front(sList *pl){
	pl->pCurr = pl->pFront->pNxt;
	return pl->pCurr->pData;
}

void *back(sList *pl){
	pl->pCurr = pl->pFront->pPrv;
	return pl->pCurr->pData;
}

void *next(sList *pl){
	pl->pCurr = pl->pCurr->pNxt;
	return pl->pCurr->pData;
}

void *current(sList *pl){
	return pl->pCurr->pData;
}

void *previous(sList *pl){
	pl->pCurr = pl->pCurr->pPrv;
	return pl->pCurr->pData;
}

int empty(sList *pl){
	return (pl->pFront->pNxt==pl->pFront) ? 1 : 0;
}
