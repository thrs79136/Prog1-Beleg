// © by Theresa Schuettig, s79136

#ifndef _GNU_SOURCE
// zur Verwendung von strcasestr
#define _GNU_SOURCE
#endif
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

int readData(sList *pList1, sList *pList2, FILE *pf, cmp cmpData){
	if (!pf || !pList1) return FAIL;
	char buf[512], *first, *sur, *tel;
	// Datensaetze werden ausgelesen und neue Personen erstellt
	while (fgets(buf, 512, pf)){
		buf[strlen(buf)-1] = 0;
		sur = strtok(buf, ",");
		first = strtok(NULL, ",");
		tel = strtok(NULL, ",");
		sPerson *pPers = getPers(sur, first, tel);
		if (!pPers) return FAIL;
		// Person wird in b
		push_back(pList1, pPers);
		if(cmpData && pList2) insertSorted(pList2, pPers, cmpData);	
	}
	showList(pList1); showList(pList2);
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

int cmpTelephonNr(void* number, void* pPers){
	return strcmp((char*)number, ((sPerson*)pPers)->telephonNr);
}

int cmpName(void *pPers1, void *pPers2){
	int ret;
	ret = strcasecmp(((sPerson*)pPers1)->surName, ((sPerson*)pPers2)->surName);
	if (ret) return ret;
	ret = strcasecmp(((sPerson*)pPers1)->firstName, ((sPerson*)pPers2)->firstName);
	return ret;
}

int cmpEntry(void *searchKey, void *pPers){
	sPerson *pPerson = pPers;
	char *entireName = malloc(strlen(pPerson->firstName)+strlen(pPerson->surName)+1);
	strcpy(entireName, pPerson->firstName);
	entireName[strlen(pPerson->firstName)] = ' ';
	entireName[strlen(pPerson->firstName)+1] = 0;
	strcat(entireName, pPerson->surName);

	int ret = (!strcasestr(entireName, (char*)searchKey)) ? 0 : 1;
	if (strstr(pPerson->telephonNr, (char*)searchKey)) ret = 1;
	free(entireName);
	return ret;	
}
