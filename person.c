// © by Theresa Schuettig, s79136

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "person.h"
#include "list.h"

void showList(sList *pList){
	for (front(pList); current(pList); next(pList)){
		putPers((sPerson*)current(pList));
	}
}

sPerson *getPers(char *sur, char *first, char *tel){
	if (!sur || !first || !tel) return NULL;
	sPerson *newPers = malloc(sizeof(sPerson));
	newPers->surName = malloc(strlen(sur)+1);
	strcpy(newPers->surName, sur);
	newPers->firstName = malloc(strlen(first)+1);
	strcpy(newPers->firstName, first);
	newPers->telephonNr = malloc(strlen(tel)+1);
	strcpy(newPers->telephonNr, tel);
	return newPers;
}

int saveData(sList *pl, FILE *pf){
	int savedall = OK;
	sPerson *pp;
	for (pp=front(pl);pp;pp=next(pl)){
		if (!fprintf(pf, "%s,%s,%s\n", pp->surName, pp->firstName, pp->telephonNr)) savedall=FAIL;
	}
	return savedall;
}

int readData(sList *pl, FILE *pf){
	char buf[512], *p;
	while (fgets(buf, 512, pf)){
		sPerson *ps = malloc(sizeof(sPerson));
		if (!ps) return FAIL;
		p = strtok(buf, ",");
		ps->surName = malloc(strlen(p)+1);
		if (!ps->surName) return FAIL;
		strcpy(ps->surName, (char*)p);
		p = strtok(NULL, ",");
		ps->firstName = malloc(strlen(p)+1);
		if (!ps->firstName) return FAIL;
		strcpy(ps->firstName, (char*)p);
		p = strtok(NULL, ",");
		p[strlen(p)-1] = 0;
		ps->telephonNr = malloc(strlen(p)+1);
		if (!ps->telephonNr) return FAIL;
		strcpy(ps->telephonNr, (char*)p);
		push_front(pl, ps);
	}
	return OK;
}

int putPers(sPerson *pers){
printf("%s %s %s\n", pers->surName, pers->firstName, pers->telephonNr);
return OK;
}

int editPers(sPerson *pPers, char *sur, char *first, char *tel){
	if (!pPers) return FAIL;
	if (sur){
			pPers->surName = realloc(pPers->surName, strlen(sur)+1);
			strcpy(pPers->surName, sur);
	}
	if (first){
			pPers->firstName = realloc(pPers->firstName, strlen(first)+1);
			strcpy(pPers->firstName, first);
	}
	if (tel){
			pPers->surName = realloc(pPers->surName, strlen(sur)+1);
			strcpy(pPers->telephonNr, tel);
	}
	return OK;
}

int freePers(sPerson* pPers){
	if (!pPers) return FAIL;
	free(pPers->surName);
	free(pPers->firstName);
	free(pPers->telephonNr);
	return OK;
}

int cmpTelephonNr(void* number, void* pPerson){
	return strcmp((char*)number, ((sPerson*)pPerson)->telephonNr);
}

int cmpName(void *pPers1, void *pPers2){
	int a= strcasecmp(((sPerson*)pPers1)->surName, ((sPerson*)pPers2)->surName);
	printf("%d\n", a);
	return a;
}