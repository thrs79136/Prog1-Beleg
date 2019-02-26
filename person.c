// © by Theresa Schuettig, s79136
// zur Verwendung von strcasestr
#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "person.h"
#include "list.h"

// erstellt neuen Kontakt und gibt Pointer auf diesen zurueck
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

// loescht Kontakt
int deletePers(sPerson* pPers){
	if (!pPers) return 0;
	free(pPers->surName);
	free(pPers->firstName);
	free(pPers->telephonNr);
	return 1;
}

// veraendert einen Kontakt
int editPers(sPerson *pPers, char *sur, char *first, char *tel){
	if (!pPers) return 0;
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
	return 1;
}

// speichert Daten einer Liste in csv-Datei
int saveData(sList *pl, FILE *pf){
	int savedeverything = 1;
	sPerson *pPers;
	for (pPers=front(pl);pPers;pPers=next(pl)){
		if (!fprintf(pf, "%s,%s,%s\n", pPers->surName, pPers->firstName, pPers->telephonNr)) savedeverything=0;
	}
	return savedeverything;
}

/* liest Daten aus csv-Datei und fuegt diese in Listen ein
 * in pList 1 wird nach Reihenfolge in der Datei eingefuegt
 * Uebergabe von pList2 ist optional, Einfuegen erfolgt abhaengig von cmpData
 */
int readData(sList *pList1, sList *pList2, FILE *pf, cmp cmpData){
	if (!pf || !pList1) return 0;
	char buf[512], *first, *sur, *tel;
	// Datensaetze werden ausgelesen und neue Personen erstellt
	while (fgets(buf, 512, pf)){
		buf[strlen(buf)-1] = 0;
		sur = strtok(buf, ",");
		first = strtok(NULL, ",");
		tel = strtok(NULL, ",");
		sPerson *pPers = getPers(sur, first, tel);
		if (!pPers) return 0;
		// 
		push_back(pList1, pPers);
		if(cmpData && pList2) insertSorted(pList2, pPers, cmpData);	
	}
	return 1;
}

// vergleicht 2 Telefonnummern
int cmpTelephonNr(void* number, void* pPers){
	return strcmp((char*)number, ((sPerson*)pPers)->telephonNr);
}

// vergleicht 2 Nachnamen, sind diese gleich wird der Vorname verglichen
int cmpName(void *pPers1, void *pPers2){
	int ret;
	ret = strcasecmp(((sPerson*)pPers1)->surName, ((sPerson*)pPers2)->surName);
	if (ret) return ret;
	// haben beide Kontakte denselben Nachnamen, so werden die Vornamen verglichen
	ret = strcasecmp(((sPerson*)pPers1)->firstName, ((sPerson*)pPers2)->firstName);
	return ret;
}

// zur Ueberpruefung, ob Sucheingabe auf einen Eintrag passt
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
