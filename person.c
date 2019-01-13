// Urheber: Theresa Schuettig s79136

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "person.h"
#include "list.h"

sPerson *getPers(char *sur, char *first, char *tel){
	if (!sur || !first || !tel) return NULL;
	sPerson *newPers = malloc(sizeof(sPerson));
	newPers->surName = malloc(strlen(sur)+1);
	strcpy(newPers->surName, sur);
	newPers->firstName = malloc(strlen(first)+1);
	strcpy(newPers->firstName, first);
	newPers->telephonNr = malloc(strlen(tel)+1);
	strcpy(newPers->telephonNr, sur);
	return newPers;
}

int saveData(sList *pl, FILE *pf){
	int savedall = OK;
	sPerson *pp;
	for (pp=back(pl);pp;pp=previous(pl)){
		if (!fprintf(pf, "%s,%s,%s\n", pp->surName, pp->firstName, pp->telephonNr)) savedall=FAIL;
	}
	return savedall;
}

int readData(sList *pl, FILE *pf){
	char buf[500], *p;
	while (fgets(buf, 500, pf)){
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
return 0;
}

int freePers(sPerson* pPers){
	if (!pPers) return FAIL;
	free(pPers->surName);
	free(pPers->firstName);
	free(pPers->telephonNr);
	return OK;
}
/*
int insertSortedBySurName(sList *pList, sPerson *pPerson){
	sPerson *x = getFirst(pList);
	// leere Liste
	if (!x)
		return (insertHead(pList, pPerson)) ? OK:FAIL;

	for (; x; x=getNext(pList)){
		// gleicher Nachname, Sortierung erfolgt nach Vornamen
		if (strcmp(pPerson->surName, x->surName)==0){
			for (;x; x=getNext(pList)){
				if (strcmp(pPerson->firstName, x->firstName)<0)
					return (insertBehindNode(pList->pCurr->pPrv, pPerson)) ? OK:FAIL;
			}
			return (insertBehindNode(pList->pCurr, pPerson)) ? OK:FAIL;
		}
		if (strcmp(pPerson->surName, x->surName)<0)
			return (insertBehindNode(pList->pCurr->pPrv, pPerson))? OK:FAIL;
	}
	// Person wird als Letztes eingefuegt
	return (insertTail(pList, pPerson)) ? OK:FAIL;
}

int insertSortedByFirstName(sList *pList, sPerson *pPerson){
	sPerson *x = getFirst(pList);
	// leere Liste
	if (!x){
		return (insertHead(pList, pPerson)) ? OK:FAIL;
	}

	for (; x; x=getNext(pList)){
		// gleicher Vorname, Sortierung erfolgt nach Nachnamen
		if (strcmp(pPerson->surName, x->surName)==0){
			for (;x; x=getNext(pList)){
				if (strcmp(pPerson->surName, x->surName)<0)
					return (insertBehindNode(pList->pCurr->pPrv, pPerson)) ? OK:FAIL;
			}
			return (insertBehindNode(pList->pCurr, pPerson)) ? OK:FAIL;
		}
		if (strcmp(pPerson->firstName, x->firstName)<0)
			return (insertBehindNode(pList->pCurr->pPrv, pPerson)) ? OK:FAIL;
	}
	return (insertTail(pList, pPerson)) ? OK:FAIL;
}

int insertSortedByNr(sList *pList, sPerson *pPerson){
	sPerson *x = getFirst(pList);
	// leere Liste
	if (!x){
		if (!insertHead(pList, pPerson))return FAIL;
		return OK;
	}

	for (; x; x=getNext(pList)){
		if (strcmp(pPerson->telephonNr, x->telephonNr)<0){
			if (!insertBehindNode(pList->pCurr->pPrv, pPerson)) return FAIL;
			return OK;
		}
	}
	if (!insertTail(pList, pPerson)) return FAIL;
	return OK;
}
*/
void putList(sList *pList){
	sPerson *pers;
	for (pers=front(pList); pers; pers=next(pList)) putPers(pers);
}
/*
int cmpString(char *s1, char *s2){
	if (s1[0] == 0) return 1;
	int i, j;
	for (i = 0; s2[i]; i++) {
		if (tolower(s2[i]) == tolower(s1[0])) {
			i++;
			for (j=1; s1[j]; j++, i++) {
				if (!s2[i]) return 0;
				if (tolower(s1[j]) != tolower(s2[i])) break;
			}
			if (!s1[j]) return 1;
		}
	}
	return 0;
}

char *strcnnct(char *s1, char *s2) {
	char *connected = malloc(strlen(s1) + strlen(s2) + 2);
	strcpy(connected, s1);
	int i;
	for (i = 0; connected[i]; i++);
	connected[i] = ' ';
	connected[i + 1] = 0;
	strcat(connected, s2);
	return connected;
}

void searchName(char *name, sList *pList){
	sPerson *x;
	for (x=getFirst(pList); x; x=getNext(pList)){
		if (cmpString(name, strcnnct(x->firstName, x->surName))) putPers(x);
	}
}

void searchNumber(char *number, sList *pList){
	sPerson *x;
	for (x=getFirst(pList); x; x=getNext(pList)){
		if (cmpString(number, x->telephonNr)) putPers(x);
	}
}

int delPers(char *telNr, sList *pList[], int listsNum){
	int i;
	for (i=0; i<listsNum; i++){
		sPerson *x = getFirst(pList[i]);
		for (;x;x=getNext(pList[i])){
			if (strcmp(telNr, x->telephonNr)==0){
				sNode *n  = pList[i]->pCurr;
				pList[i]->pCurr->pPrv->pNxt = pList[i]->pCurr->pNxt;
				pList[i]->pCurr->pNxt->pPrv = pList[i]->pCurr->pPrv;
				if (i==listsNum-1) free(pList[i]->pCurr->pItem);
				free(pList[i]->pCurr);
				pList[i]->pCurr = pList[i]->pHead;
				break;
			}
		}
	}

}
*/
