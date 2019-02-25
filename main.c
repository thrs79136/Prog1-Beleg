// © by Theresa Schuettig, s79136

// Loeschen und Bearbeiten funktioniert noch nicht richtig

#include <gtk/gtk.h>
#include <glib.h>
#include <glib/gprintf.h>
#include <ctype.h>
#include "person.h"
#include "list.h"
#include "gtk.h"

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

// fuegt Datensaetze an das Ende (wenn append=1) oder den Anfang der TreeView 
void add_item(sPerson *pers, app_widgets *widgets, int append){
	if (append)
		gtk_list_store_append(widgets->store, &widgets->iter);
	else
		gtk_list_store_prepend(widgets->store, &widgets->iter);
	gtk_list_store_set(	widgets->store, &(widgets->iter),
						SURNAME, (gchar*)pers->surName,
						FIRSTNAME, (gchar*)pers->firstName,
						TELNR, (gchar*)pers->telephonNr, -1);
}

// zeigt Suchresultate an
void print_search_results(GtkWidget *widget, app_widgets *widgets){
	if (gtk_tree_model_get_iter_first(widgets->model, &widgets->iter) == TRUE){
		gtk_list_store_clear(widgets->store);
	}
	GtkWidget *searchEntry = GTK_WIDGET(gtk_builder_get_object(widgets->builder, "searchEntry"));
	sPerson *pPers;
	sList *pList = (widgets->chronSorted) ? widgets->list : widgets->sortedList;
	for (pPers=front(pList); pPers; pPers=next(pList)){
		char *searchKey = (char*)gtk_entry_get_text(GTK_ENTRY(searchEntry));
		if (cmpEntry((void*)searchKey, pPers)) add_item(pPers, widgets, 1);
	}
}

void print_list(sList *pList, app_widgets *widgets){
	if (gtk_tree_model_get_iter_first(widgets->model, &widgets->iter) == TRUE){
		gtk_list_store_clear(widgets->store);
	}
	sPerson *pPers;
	for (pPers=front(pList); pPers; pPers=next(pList)){
		add_item(pPers, widgets, 1);
	}
}

// wird aufgerufen, wenn Veraenderung bestaetigt wurde
void on_saveButton_clicked(GtkWidget *widget, app_widgets *widgets){
	char *cSur, *cFirst, *cTelNr, *cOldNr;
	cSur  = (char*)gtk_entry_get_text(GTK_ENTRY(widgets->surname));
	cFirst = (char*)gtk_entry_get_text(GTK_ENTRY(widgets->firstname));
	cTelNr = (char*)gtk_entry_get_text(GTK_ENTRY(widgets->telnr));
	gtk_tree_model_get(widgets->model, &widgets->iter, TELNR, &cOldNr, -1);

	// prueft, ob die Telefonnummer bereits existiert
	int number_exists=0;
	if (strcmp(cTelNr, cOldNr) != 0){
		sPerson *pPers;
		for (pPers=next(widgets->list); pPers; pPers=next(widgets->list))
			if ( cmpTelephonNr((void*)cTelNr, pPers) == 0 ) number_exists=1;
	}
	if (!number_exists){
		// Eintrag in der Liste wird geaendert und neu eingefuegt
		sPerson *pPers;
		for (pPers=front(widgets->list); pPers; pPers=next(widgets->list)){
			if ( cmpTelephonNr(cOldNr, pPers) == 0 ){
					rmCurr(widgets->list);
					removeItem(widgets->sortedList, cOldNr, cmpTelephonNr);
					editPers(pPers, cSur, cFirst, cTelNr);
					push_front(widgets->list, pPers);
					insertSorted(widgets->sortedList, pPers, cmpName);
					break;
			}
		}
		// zeigt Liste neu an
		if (widgets->chronSorted) print_list(widgets->list, widgets);
		else print_list(widgets->sortedList, widgets);
		gtk_widget_hide(widgets->window);
	}
	else gtk_widget_show(widgets->label);
}

// Oeffnet Fenster zum Bearbeiten der Eintraege
void on_editButton_clicked(GtkWidget *widget, app_widgets *widgets){
	widgets->label = GTK_WIDGET(gtk_builder_get_object(widgets->builder, "numberExistsLabel2"));
	widgets->button = GTK_WIDGET(gtk_builder_get_object(widgets->builder, "saveButton"));
	widgets->selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(widgets->treeview));
	if (gtk_tree_model_get_iter_first(widgets->model, &widgets->iter)==FALSE) return;
	if (gtk_tree_selection_get_selected(GTK_TREE_SELECTION(widgets->selection), &widgets->model, &widgets->iter)){
		char *cSur, *cFirst, *cTelNr;
		// Daten des ausgewaehlten Datensatzes werden in den gchars gespeichert
		gtk_tree_model_get(	widgets->model, &widgets->iter,
							SURNAME, &cSur,
							FIRSTNAME, &cFirst,
							TELNR, &cTelNr, -1);
		widgets->window = GTK_WIDGET(gtk_builder_get_object(widgets->builder, "editEntryWin"));
		gtk_widget_show_all(widgets->window);
		gtk_widget_hide(widgets->label);

		widgets->surname = GTK_WIDGET(gtk_builder_get_object(widgets->builder, "changeSurnameEntry"));
		widgets->firstname = GTK_WIDGET(gtk_builder_get_object(widgets->builder, "changeFirstNameEntry"));
		widgets->telnr = GTK_WIDGET(gtk_builder_get_object(widgets->builder, "changeTelNrEntry"));
		// Namen und Telefonnummer werden in Eingabefelder eingefuegt
		gtk_entry_set_text(GTK_ENTRY(widgets->surname), cSur);
		gtk_entry_set_text(GTK_ENTRY(widgets->firstname), cFirst);
		gtk_entry_set_text(GTK_ENTRY(widgets->telnr), cTelNr);
		g_free(cSur); g_free(cFirst); g_free(cTelNr);
	}
}

// oeffnet Fenster, das Bestaetigung zum Loeschen abfragt
void on_deleteButton_clicked(GtkWidget *widget, app_widgets *widgets){
	widgets->selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(widgets->treeview));
	if (gtk_tree_model_get_iter_first(widgets->model, &widgets->iter)==FALSE) return;
	if (gtk_tree_selection_get_selected(GTK_TREE_SELECTION(widgets->selection), &widgets->model, &widgets->iter)){
		widgets->window  = GTK_WIDGET(gtk_builder_get_object(widgets->builder, "queryWin"));
		GtkWidget *entryLabel = GTK_WIDGET(gtk_builder_get_object(widgets->builder, "entryLabel"));
		char *cSur, *cFirst, *cTelNr;
		gtk_tree_model_get(	widgets->model, &widgets->iter,
							SURNAME, &cSur,
							FIRSTNAME, &cFirst,
							TELNR, &cTelNr, -1);
		// Speicherplatz fuer Vorname, Leerzeichen, Nachname, Zeilenumbruch und \0
		char *entry = malloc(strlen(cSur)+strlen(cFirst)+strlen(cTelNr)+3);
		// fuegt alle Strings zusammen
		sprintf(entry, "%s %s\n%s", cFirst, cSur, cTelNr);
		gtk_label_set_text(GTK_LABEL(entryLabel), entry);
		gtk_widget_show_all(widgets->window);
	}
}

void on_yesButton_clicked(GtkWidget *widget, app_widgets *widgets){
		// Telefonnummer des zu loeschenden Eintrags wird ermittelt...
		char *telNr;
		gtk_tree_model_get(widgets->model, &widgets->iter, TELNR, &telNr, -1);
		// ...und die dazugehoerige Person aus den Listen entfernt
		removeItem(widgets->list, (void*)telNr, cmpTelephonNr);
		void *pData = removeItem(widgets->sortedList, (void*)telNr, cmpTelephonNr);
		if (pData) freePers((sPerson*)pData);
		// Eintrag wird aus TreeView entfernt
		gtk_list_store_remove(widgets->store, &widgets->iter);
		gtk_widget_hide(widgets->window);
}

// prueft, ob Eingabe des neuen Eintrags zulaessig ist, indem bei jeder Aenderung im Eingabefeld das letzte Zeichen ueberprueft wird
// Eingabefelder sind nicht leer, Telefonnr. beinhaltet nur Ziffern und Namen nur Buchstaben und Punkte
void checkValidInput(GtkWidget *widget, app_widgets *widgets){
	char *sur = (char*)gtk_entry_get_text(GTK_ENTRY(widgets->surname));
	char *first =(char*)gtk_entry_get_text(GTK_ENTRY(widgets->firstname));
	char *tel =(char*)gtk_entry_get_text(GTK_ENTRY(widgets->telnr));
	static gboolean isvalid = FALSE;
	if (strlen(tel)>0){
		if (!isdigit(tel[strlen(tel)-1])){
			char *tel2 = malloc(strlen(tel));
			strcpy(tel2, tel);
			tel2[strlen(tel2)-1] = 0;
			gtk_entry_set_text(GTK_ENTRY(widgets->telnr), tel2);
		}
	}	
	// ueberprueft, ob eines der Eingabefelder leer ist
	if (sur[0]==0 || first[0]==0 || tel[0]==0) isvalid = FALSE;
	else isvalid = TRUE;
	gtk_widget_set_sensitive (widgets->button, isvalid);

}

// Oeffnet Fenster, das das Erstellen eines neuen Eintrags ermoeglicht
void on_addButton_clicked(GtkWidget *widget, app_widgets *widgets){
	widgets->label = GTK_WIDGET(gtk_builder_get_object(widgets->builder, "numberExistsLabel"));
	widgets->button = GTK_WIDGET(gtk_builder_get_object(widgets->builder, "addButton2"));
	gtk_widget_set_sensitive (widgets->button, FALSE);
	widgets->window  = GTK_WIDGET(gtk_builder_get_object(widgets->builder, "newEntryWin"));
	widgets->surname = GTK_WIDGET(gtk_builder_get_object(widgets->builder, "surnameEntry"));
	widgets->firstname = GTK_WIDGET(gtk_builder_get_object(widgets->builder, "firstNameEntry"));
	widgets->telnr = GTK_WIDGET(gtk_builder_get_object(widgets->builder, "telNrEntry"));

	gtk_widget_show_all(widgets->window);
	gtk_widget_hide(widgets->label);
}

// char zu gSur aendern
// Neuer Eintrag wurde bestaetigt und wird gespeichert
void on_addButton2_clicked(GtkWidget *widget, app_widgets *widgets){
	char *tel = (char*)gtk_entry_get_text(GTK_ENTRY(widgets->telnr));
	GtkWidget *warningLabel = GTK_WIDGET(gtk_builder_get_object(widgets->builder, "numberExistsLabel"));
	// Pruefung, ob Telefonnummer bereits existiert
	int number_exists = 0;
	sPerson *x;
	for (x=front(widgets->list); x; x=next(widgets->list))
		if (cmpTelephonNr((void*)tel, x) ==0) number_exists = 1;
	// existiert sie noch nicht, so wird ein neuer Eintrag erstellt
	if (!number_exists){
		char *sur = (char*)gtk_entry_get_text(GTK_ENTRY(widgets->surname));
		char *first = (char*)gtk_entry_get_text(GTK_ENTRY(widgets->firstname));	
		sPerson *pp = getPers(sur, first, tel);

		push_front(widgets->list, pp);
		insertSorted(widgets->sortedList, pp, cmpName);
		gtk_list_store_clear(widgets->store);
		if (widgets->chronSorted)
			print_list(widgets->list, widgets);
		else
			print_list(widgets->sortedList, widgets);

		gtk_widget_hide(widgets->window);
		gtk_entry_set_text(GTK_ENTRY(widgets->surname), "");
		gtk_entry_set_text(GTK_ENTRY(widgets->firstname), "");
		gtk_entry_set_text(GTK_ENTRY(widgets->telnr), "");
	}	
	else {
		// gibt Warnung aus, dass Nummer bereits existiert
		gtk_widget_show(warningLabel);
	}
}

// sortiert Datensaetze
void on_sortButton_clicked(GtkWidget *widget, app_widgets *widgets){
	if (gtk_tree_model_get_iter_first(widgets->model, &widgets->iter)==FALSE)
		return;

	widgets->chronSorted ^=1;
	char *label[] = {"Sortieren (nach Erstelldatum)", "Sortieren (nach Namen)"};
	gtk_button_set_label(GTK_BUTTON(widgets->sortButton), label[widgets->chronSorted]);

	char *searchKey = (char*)gtk_entry_get_text(GTK_ENTRY(widgets->searchEntry));
	// wenn etwas in der Suchleiste steht, werden nur die Suchergebnisse angezeigt
	if (strlen(searchKey)) print_search_results(widget, widgets);
	else{
		gtk_list_store_clear(widgets->store);
		sList *pList;
		if (widgets->chronSorted){
			pList = widgets->list;
		}
		else{
			pList = widgets->sortedList;
		}	
		print_list(pList, widgets);
	}
}

void on_mainWindow_destroy(GtkWidget *widget, app_widgets *widgets){
	FILE *pf = fopen("Daten.csv", "wt");
	if (!pf); // Fehlermeldung
	// Datensaetze werden in Datei gespeichert
	saveData(widgets->list, pf);
	// nach Namen sortierte Liste wird geloescht
	deleteList(widgets->sortedList);
	sPerson *x;
	for (x=front(widgets->list); x; x=next(widgets->list))
		free(x);
		// Speicher der Datensaetze wird freigegeben
	// chronologisch sortierete Liste wird entfernt
	deleteList(widgets->list);
	// Datei wird geschlossen
	fclose(pf);
	gtk_main_quit();
}

int main(int argc, char *argv[]){

	GtkWidget *mainWindow;
	gtk_init(&argc, &argv);

	app_widgets *widgets = g_slice_new(app_widgets);
	widgets->list = createList();
	widgets->sortedList = createList();
	FILE *pf = fopen("Daten.csv", "rt");
	if (pf){
		readData(widgets->list, widgets->sortedList, pf, cmpName);
		fclose(pf);
	}

	widgets->builder = gtk_builder_new();
	gtk_builder_add_from_file(widgets->builder, "interface.xml", NULL);
	mainWindow = GTK_WIDGET(gtk_builder_get_object(widgets->builder, "mainWindow"));
	widgets->store = GTK_LIST_STORE(gtk_builder_get_object(widgets->builder, "data"));
	widgets->treeview = GTK_TREE_VIEW(gtk_builder_get_object(widgets->builder,"treeview"));
	widgets->model = gtk_tree_view_get_model(GTK_TREE_VIEW(widgets->treeview));
	widgets->sortButton = GTK_WIDGET(gtk_builder_get_object(widgets->builder, "sortButton"));
	widgets->searchEntry = GTK_WIDGET(gtk_builder_get_object(widgets->builder, "searchEntry"));
	// Liste ist bei Start chronologisch sortiert
	widgets->chronSorted = 1;

	// Einrichten der Signalhandler
	gtk_builder_connect_signals(widgets->builder, widgets);

	gtk_widget_show_all(mainWindow);
	print_list(widgets->list, widgets);
	gtk_main();
	g_object_unref(widgets->builder);

	return 0;
}
