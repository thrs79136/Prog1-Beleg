// © by Theresa Schuettig, s79136

#ifndef PERSON_H
#define PERSON_H

struct sList;
typedef struct sList sList;

typedef struct sPerson{
	char *surName;
	char *firstName;
	char *telephonNr;
} sPerson;

typedef int (*cmp)(void*,void*);

// erstellt neuen Kontakt und gibt Pointer auf diesen zurueck
sPerson *getPers(char *sur, char *first, char *tel);
// loescht Kontakt
int deletePers(sPerson* pPers); 
// verandert einen Kontakt
int editPers(sPerson *pPers, char *sur, char *first, char *tel);

// speichert Daten einer Liste in einer csv-Datei
int saveData(sList *pl, FILE *pf);
/* liest Daten aus csv-Datei und fuegt diese in Listen ein
 * in pList 1 wird nach Reihenfolge in der Datei eingefuegt
 * Uebergabe von pList2 ist optional, Einfuegen erfolgt abhaengig von cmpData
 */
int readData(sList *pList1, sList *pList2, FILE *pf, cmp cmpData);

// vergleicht 2 Telefonnummern
int cmpTelephonNr(void *number, void *pPers);
// vergleicht 2 Nachnamen, sind diese gleich werden deren Vornamen verglichen
int cmpName(void *pPers1, void *pPers2);
// zur Ueberpruefung, ob Sucheingabe auf einen Eintrag passt
int cmpEntry(void *searchKey, void *pPers);

#endif

