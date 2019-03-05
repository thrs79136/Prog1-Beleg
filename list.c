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
	void *pData;
	for (pData = front(pl); pData; pData=next(pl)){
		rmCurr(pl);
	}
	rmCurr(pl);
	free(pl);
}

int insertBeforeCurr(sList *pl, void *pData){
	sNode *pn = malloc(sizeof(sNode));
	if (!pn) return 0;
	pn->pData = pData;
	pn->pPrv = pl->pCurr->pPrv;
	pn->pNxt = pl->pCurr;
	pl->pCurr->pPrv->pNxt = pn;
	pl->pCurr->pPrv = pn;
	return 1;
}

int insertBehindCurr(sList *pl, void *pData){
	sNode *pn = malloc(sizeof(sNode));
	if (!pn) return 0;
	pn->pData = pData;
	pn->pPrv = pl->pCurr;
	pn->pNxt = pl->pCurr->pNxt;
	pl->pCurr->pNxt->pPrv = pn;
	pl->pCurr->pNxt = pn;
	return 1;
}

int rmCurr(sList *pl){
	if (pl->pCurr->pData==NULL) return 0;
	sNode *pn = pl->pCurr;
	pl->pCurr->pPrv->pNxt = pl->pCurr->pNxt;
	pl->pCurr->pNxt->pPrv = pl->pCurr->pPrv;
	pl->pCurr = pl->pCurr->pPrv;
	free(pn);
	return 1;
}

int push_front(sList *pl, void *pData){
	pl->pCurr = pl->pFront;
	return insertBehindCurr(pl, pData);
}

int push_back(sList *pl, void *pData){
	pl->pCurr = pl->pFront;
	return insertBeforeCurr(pl, pData);
}
int pop_front(sList *pl){
	pl->pCurr = pl->pFront->pNxt;
	return rmCurr(pl);
}

int pop_back(sList *pl){
	pl->pCurr = pl->pFront->pPrv;
	return rmCurr(pl);
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

int insertSorted(sList *pl, void *pData, cmp cmpData){
	for (front(pl); current(pl); next(pl)){
		if (cmpData(pData, current(pl)) < 0) {
			break;
		}
	}
	return insertBeforeCurr(pl, pData);
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
