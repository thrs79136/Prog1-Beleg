// © by Theresa Schuettig, s79136

#ifndef PERSON_H
#define PERSON_H

#define OK 1
#define FAIL 0

struct sList;
typedef struct sList sList;

typedef struct{
	char *surName;
	char *firstName;
	char *telephonNr;
} sPerson;

typedef int (*cmp)(void*,void*);

void showList(sList *pList);

sPerson *getPers(char *sur, char *first, char *tel);
// speichert Daten einer Liste in einer csv-Datei
int saveData(sList *pl, FILE *pf);
// liest Daten aus csv-Datei und fuegt sie in Liste ein
int readData(sList *pList1, sList *pList2, FILE *pf, cmp cmpData);
int putPers(void* pPers);
int editPers(sPerson *pPers, char *sur, char *first, char *tel);
int freePers(sPerson* pPers); 

void searchName(char *name, sList *pList);
void searchNumber(char *number, sList *pList);
int delPers(char *telNr, sList *pList, int listsNum);

int cmpTelephonNr(void *number, void *pPers);
int cmpName(void *pPers1, void *pPers2);
// prueft, ob Sucheingabe auf einen Eintrag passt
int cmpEntry(void *searchKey, void *pPers);

#endif

