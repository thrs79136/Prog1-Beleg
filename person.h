// Urheber: Theresa Schuettig s79136

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

sPerson *getPers(char *sur, char *first, char *tel);
// speichert Daten einer Liste in einer csv-Datei
int saveData(sList *pl, FILE *pf);
// liest Daten aus csv-Datei und fuegt sie in Liste ein
int readData(sList *pl, FILE *pf);
int putPers(sPerson* pPers);
int freePers(sPerson* pPers);

int insertSortedBySurName(sList *pList, sPerson *pPerson);
int insertSortedByFirstName(sList *pList, sPerson *pPerson);
int insertSortedByNr(sList *pList, sPerson *pPerson);
void putList(sList *pList);
void searchName(char *name, sList *pList);
void searchNumber(char *number, sList *pList);
int delPers(char *telNr, sList *pList[], int listsNum);

int savePerson(FILE *pf, sPerson *ps);
sPerson *readPerson(FILE *pf);

// gibt 1 zurueck, wenn s1 sich in s2 befindet, 0 sonst
int cmpString(char *s1, char *s2);
char* strcnnct(char *s1, char *s2);

#endif

