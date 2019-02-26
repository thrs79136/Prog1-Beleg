// © by Theresa Schuettig, s79136

#ifndef GTK_H
#define GTK_H

struct sList;
typedef struct sList sList;
struct sPerson;
typedef struct sPerson sPerson;

enum{
	SURNAME=0,
	FIRSTNAME,
	TELNR,
	NUM
};

typedef struct{
	sList *list;
	sList *sortedList;
	GtkBuilder *builder;
	GtkWidget *window;
	GtkWidget *button;
	GtkWidget *label;
	GtkListStore *store;
	GtkTreeView *treeview;
	GtkTreeModel *model;
	GtkTreeSelection *selection;
	GtkTreeIter iter;
	GtkWidget *surname;
	GtkWidget *firstname;
	GtkWidget *telnr;
	GtkWidget *sortButton;
	GtkWidget *searchEntry;
	int chronSorted;
}app_widgets;

app_widgets *widgets;

// zeigt alle Suchergebnisse an
void print_search_results(GtkWidget *widget, app_widgets *widgets);
// oeffnet Fenster zum Erstellen eines neuen Eintrags
void on_addButton_clicked(GtkWidget *widget, app_widgets *widgets);
// speichert erstellten Eintrag
void on_addButton2_clicked(GtkWidget *widget, app_widgets *widgets);
// oeffnet Fenster zum Bearbeiten eines Eintrags
void on_editButton_clicked(GtkWidget *widget, app_widgets *widgets);
// speichert veraenderten Eintrag
void on_saveButton_clicked(GtkWidget *widget, app_widgets *widgets);
// oeffnet Fenster, das Bestaetigung zum Loeschen vom Nutzer abfragt
void on_deleteButton_clicked(GtkWidget *widget, app_widgets *widgets);
// loescht Eintrag
void on_yesButton_clicked(GtkWidget *widget, app_widgets *widgets);
// gibt Eintraege sortiert aus
void on_sortButton_clicked(GtkWidget *widget, app_widgets *widgets);
// speichert Eintraege in csv-Datei, loescht die verketteten Listen
void on_mainWindow_destroy(GtkWidget *widget, app_widgets *widgets);
// zeigt Liste im Fenster an
void print_list(sList *pList, app_widgets *widgets);
/* fuegt einen Eintrag in die TreeView ein
 * append = 0: 	Eintrag wird am Anfang eingefuegt
   sonst:		Eintrag wird angehaengt
 */
void add_item(sPerson *pers, app_widgets *widgets, int append);
/* prueft, ob keine Eingabe im Fenster leer ist, verhindert Eingabe von
 * anderen Zeichen als Ziffern bei der Telefonnummer
 */
void checkValidInput(GtkWidget *widget, app_widgets *widgets);
// gibt Fehlermeldung aus
void error(char *errmsg, GtkBuilder *builder);

#endif