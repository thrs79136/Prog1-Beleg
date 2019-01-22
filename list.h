// Urheber: Theresa Schuettig s79136

#ifndef LIST_H
#define LIST_H

typedef struct sNode{
   struct sNode *pNxt;
   struct sNode *pPrv;
   void *pData;
}sNode;

typedef struct sList{
   sNode *pFront;
   sNode *pCurr;
}sList;

// erstellt eine Liste, Rückgabewert: Pointer auf Liste, wenn erfolgreich, sonst NULL
sList *createList();
// fuegt Item vor pCurr ein
int insertBeforeCurr(sList *pl, void *pData);
// fügt Item hinter pCurr ein
int insertBehindCurr(sList *pl, void *pData);
// entfernt pCurr aus der Liste
void rmCurr(sList *pl);
// fügt pData als erstes Listenelement ein
int push_front(sList *pl, void *pData);
// fügt pData als letztes Listenelement ein
int push_back(sList *pl, void *pData);
// entfernt das erste Listenelement
void pop_front(sList *pl);
// entfernt das letzte Listenelement
void pop_back(sList *pl);
// entfernt Listenelemente, die pValue enthalten
void removeItem(sList *pl, void *pValue);

// gibt einen Pointer auf das erste Listenelement zurück
void *front(sList *pl);
// gibt einen Pointer auf das letzte Listenelement zurück
void *back(sList *pl);
// gibt einen Pointer auf das nächste Listenelemnt zurück (pCurr->pNxt)
void *next(sList *pl);
// gibt einen Pointer auf das letzte Listenelement zurück (pCurr->pPrv)
void *previous(sList *pl);
// gibt 1 zurück, wenn die Liste leer ist, sonst 0
int empty(sList *pl);

#endif

