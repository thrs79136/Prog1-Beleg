// © by Theresa Schuettig, s79136

#include <gtk/gtk.h>
#include <glib.h>
#include <glib/gprintf.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include "person.h"
#include "list.h"
#include "gtk.h"

// zeigt alle Suchergebnisse an
void print_search_results(GtkWidget *widget, app_widgets *widgets){
    // TreeView wird geleert
	if (gtk_tree_model_get_iter_first(widgets->model, &widgets->iter) == TRUE){
		gtk_list_store_clear(widgets->store);
	}
	GtkWidget *searchEntry = GTK_WIDGET(gtk_builder_get_object(widgets->builder, "searchEntry"));
	sPerson *pPers;
    // Festlegung, welche Liste angezeigt werden soll
	sList *pList = (widgets->chronSorted) ? widgets->list : widgets->sortedList;
	for (pPers=front(pList); pPers; pPers=next(pList)){
		char *searchKey = (char*)gtk_entry_get_text(GTK_ENTRY(searchEntry));
        // Eintrag wird angezeigt, wenn er auf die Eingabe in der Suchleiste passt
		if (cmpEntry((void*)searchKey, pPers)) add_item(pPers, widgets, 1);
	}
}

// oeffnet Fenster zum Erstellen eines neuen Eintrags
void on_addButton_clicked(GtkWidget *widget, app_widgets *widgets){
	widgets->label = GTK_WIDGET(gtk_builder_get_object(widgets->builder, "numberExistsLabel"));
	widgets->button = GTK_WIDGET(gtk_builder_get_object(widgets->builder, "addButton2"));
	gtk_widget_set_sensitive (widgets->button, FALSE);
	widgets->window  = GTK_WIDGET(gtk_builder_get_object(widgets->builder, "newEntryWin"));
	widgets->surname = GTK_WIDGET(gtk_builder_get_object(widgets->builder, "surnameEntry"));
	widgets->firstname = GTK_WIDGET(gtk_builder_get_object(widgets->builder, "firstNameEntry"));
	widgets->telnr = GTK_WIDGET(gtk_builder_get_object(widgets->builder, "telNrEntry"));

    // Fenster zur Eingabe eines neuen Eintrags wird angezeigt
	gtk_widget_show_all(widgets->window);
	gtk_widget_hide(widgets->label);
}

// speichert erstellten Eintrag
void on_addButton2_clicked(GtkWidget *widget, app_widgets *widgets){
	char *tel = (char*)gtk_entry_get_text(GTK_ENTRY(widgets->telnr));
	// Pruefung, ob Telefonnummer bereits existiert
	int number_exists = 0;
	sPerson *pPers;
	for (pPers=front(widgets->list); pPers; pPers=next(widgets->list))
		if (cmpTelephonNr((void*)tel, pPers) ==0) number_exists = 1;
	// existiert sie noch nicht, so wird ein neuer Eintrag erstellt
	if (!number_exists){
		char *sur = (char*)gtk_entry_get_text(GTK_ENTRY(widgets->surname));
		char *first = (char*)gtk_entry_get_text(GTK_ENTRY(widgets->firstname));	
		sPerson *pp = getPers(sur, first, tel);

		if (!push_front(widgets->list, pp)) error("Fehler beim Erstellen des Kontakts", widgets->builder);
		if (!insertSorted(widgets->sortedList, pp, cmpName)) error("Fehler beim Erstellen des Kontakts", widgets->builder);
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
		gtk_widget_show(widgets->label);
	}
}
 //oeffnet Fenster zum Bearbeiten eines Eintrags
void on_editButton_clicked(GtkWidget *widget, app_widgets *widgets){
    widgets->label = GTK_WIDGET(gtk_builder_get_object(widgets->builder, "numberExistsLabel2"));
	widgets->button = GTK_WIDGET(gtk_builder_get_object(widgets->builder, "saveButton"));
	widgets->selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(widgets->treeview));
	if (gtk_tree_model_get_iter_first(widgets->model, &widgets->iter)==FALSE) return;
	if (gtk_tree_selection_get_selected(GTK_TREE_SELECTION(widgets->selection), &widgets->model, &widgets->iter)){
		char *cSur, *cFirst, *cTelNr;
		// Daten des ausgewaehlten Eintrags werden in den chars gespeichert
		gtk_tree_model_get(	widgets->model, &widgets->iter,
							SURNAME, &cSur,
							FIRSTNAME, &cFirst,
							TELNR, &cTelNr, -1);
		widgets->window = GTK_WIDGET(gtk_builder_get_object(widgets->builder, "editEntryWin"));

        widgets->surname = GTK_WIDGET(gtk_builder_get_object(widgets->builder, "changeSurnameEntry"));
		widgets->firstname = GTK_WIDGET(gtk_builder_get_object(widgets->builder, "changeFirstNameEntry"));
		widgets->telnr = GTK_WIDGET(gtk_builder_get_object(widgets->builder, "changeTelNrEntry"));
		// Namen und Telefonnummer werden in Eingabefelder eingefuegt
		gtk_entry_set_text(GTK_ENTRY(widgets->surname), cSur);
		gtk_entry_set_text(GTK_ENTRY(widgets->firstname), cFirst);
		gtk_entry_set_text(GTK_ENTRY(widgets->telnr), cTelNr);
		g_free(cSur); g_free(cFirst); g_free(cTelNr);

        // Fenster zum Bearbeiten wird angezeigt
		gtk_widget_show_all(widgets->window);
		gtk_widget_hide(widgets->label);		
	}
}

// speichert veraenderten Eintrag
void on_saveButton_clicked(GtkWidget *widget, app_widgets *widgets){
	char *cSur, *cFirst, *cTelNr, *cOldNr;
	cSur  = (char*)gtk_entry_get_text(GTK_ENTRY(widgets->surname));
	cFirst = (char*)gtk_entry_get_text(GTK_ENTRY(widgets->firstname));
	cTelNr = (char*)gtk_entry_get_text(GTK_ENTRY(widgets->telnr));
	gtk_tree_model_get(widgets->model, &widgets->iter, TELNR, &cOldNr, -1);

	// prueft, ob die Telefonnummer bereits existiert, falls sie veraendert wurde
	int number_exists=0;
	if (strcmp(cTelNr, cOldNr) != 0){
		sPerson *pPers;
		for (pPers=next(widgets->list); pPers; pPers=next(widgets->list))
			if ( cmpTelephonNr((void*)cTelNr, pPers) == 0 ) number_exists=1;
	}
	if (!number_exists){
		// Eintrag wird aus Listen entfernt, bearbeitet und neu eingefuegt
		sPerson *pPers;
		for (pPers=front(widgets->list); pPers; pPers=next(widgets->list)){
			if ( cmpTelephonNr(cOldNr, pPers) == 0 ){
					if (!rmCurr(widgets->list)) error("Fehler beim Bearbeiten des Kontakts", widgets->builder);;
					if (!removeItem(widgets->sortedList, cOldNr, cmpTelephonNr)) error("Fehler beim Bearbeiten des Kontakts", widgets->builder);;
					if (!editPers(pPers, cSur, cFirst, cTelNr)) error("Fehler beim Bearbeiten des Kontakts", widgets->builder);
					if (!push_front(widgets->list, pPers)) error("Fehler beim Bearbeiten des Kontakts", widgets->builder);
					if (!insertSorted(widgets->sortedList, pPers, cmpName)) error("Fehler beim Bearbeiten des Kontakts", widgets->builder);
					break;
			}
		}
		// zeigt Liste aktualisiert an
		if (widgets->chronSorted) print_list(widgets->list, widgets);
		else print_list(widgets->sortedList, widgets);
		gtk_widget_hide(widgets->window);
	}
    // zeigt Warnung an, falls Nummer bereits existiert
	else gtk_widget_show(widgets->label);
}

// oeffnet Fenster, das Bestaetigung zum Loeschen vom Nutzer abfragt
void on_deleteButton_clicked(GtkWidget *widget, app_widgets *widgets){
	widgets->selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(widgets->treeview));
	if (gtk_tree_model_get_iter_first(widgets->model, &widgets->iter)==FALSE) return;
	if (gtk_tree_selection_get_selected(GTK_TREE_SELECTION(widgets->selection), &widgets->model, &widgets->iter)){
		widgets->window  = GTK_WIDGET(gtk_builder_get_object(widgets->builder, "queryWin"));
        // Label, das Name und Nummer des asgewaehlten Kontakts anzeigt
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

// loescht Eintrag
void on_yesButton_clicked(GtkWidget *widget, app_widgets *widgets){
		// Telefonnummer des zu loeschenden Eintrags wird ermittelt...
		char *telNr;
		gtk_tree_model_get(widgets->model, &widgets->iter, TELNR, &telNr, -1);
		// ...und die dazugehoerige Person aus den Listen entfernt
		if (!removeItem(widgets->list, (void*)telNr, cmpTelephonNr)) error("Fehler beim Loeschen des Kontakts", widgets->builder);
		void *pData = removeItem(widgets->sortedList, (void*)telNr, cmpTelephonNr);
		if (!pData) error("Fehler beim Loeschen des Kontakts", widgets->builder);
		deletePers((sPerson*)pData);
		// Eintrag wird aus TreeView entfernt
		gtk_list_store_remove(widgets->store, &widgets->iter);
		gtk_widget_hide(widgets->window);
}

// gibt Eintraege sortiert aus
void on_sortButton_clicked(GtkWidget *widget, app_widgets *widgets){
	if (gtk_tree_model_get_iter_first(widgets->model, &widgets->iter)==FALSE)
		return;

	widgets->chronSorted ^=1;
	char *label[] = {"Sortieren (nach Erstelldatum)", "Sortieren (nach Namen)"};
    // Beschriftung des Buttons wird gewechselt
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

// speichert Eintraege in csv-Datei, loescht die verketteten Listen
void on_mainWindow_destroy(GtkWidget *widget, app_widgets *widgets){
	FILE *pf = fopen("Daten.csv", "wt");
	if (!pf) error("Fehler beim Speichern der Daten", widgets->builder); // Fehlermeldung
	// Daten werden in Datei gespeichert
	if (!saveData(widgets->list, pf)) error("Fehler beim Speichern der Daten", widgets->builder);
	// nach Namen sortierte Liste wird geloescht
	deleteList(widgets->sortedList);
	sPerson *pPers;
	for (pPers=front(widgets->list); pPers; pPers=next(widgets->list))
		free(pPers);
		// Fuer Kontakte resrvierter Speicher wird freigegeben
	// chronologisch sortierete Liste wird entfernt
	deleteList(widgets->list);
	// Datei wird geschlossen
	fclose(pf);
    // Beendigung von gtk
	gtk_main_quit();
}

// zeigt Liste im Fenster an
void print_list(sList *pList, app_widgets *widgets){
    // TreeView wird geleert
	if (gtk_tree_model_get_iter_first(widgets->model, &widgets->iter) == TRUE){
		gtk_list_store_clear(widgets->store);
	}
    // gesamte Liste wird der TreeView inzugefuegt
	sPerson *pPers;
	for (pPers=front(pList); pPers; pPers=next(pList)){
		add_item(pPers, widgets, 1);
	}
}

/* fuegt einen Eintrag in die TreeView ein
 * append = 0: 	Eintrag wird am Anfang eingefuegt
   sonst:		Eintrag wird angehaengt
 */
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

/* prueft, ob keine Eingabe im Fenster leer ist, verhindert Eingabe von
 * anderen Zeichen als Ziffern bei der Telefonnummer
 */
void checkValidInput(GtkWidget *widget, app_widgets *widgets){
	char *sur = (char*)gtk_entry_get_text(GTK_ENTRY(widgets->surname));
	char *first =(char*)gtk_entry_get_text(GTK_ENTRY(widgets->firstname));
	char *tel =(char*)gtk_entry_get_text(GTK_ENTRY(widgets->telnr));
	static gboolean isvalid = FALSE;
	if (strlen(tel)>0){
        // wurde ein ungueltiges Zeichen eingegeben, wird dieses wieder entfernt
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
    // legt fest, ob Button betaetigt werden darf
	gtk_widget_set_sensitive (widgets->button, isvalid);
}

// gibt Fehlermeldung aus
void error(char *errmsg, GtkBuilder *builder){
	char *x = "Interner Fehler: ";
	GtkWidget *win = GTK_WIDGET(gtk_builder_get_object(widgets->builder, "errorWindow"));
	GtkWidget *label = GTK_WIDGET(gtk_builder_get_object(widgets->builder, "errorLabel"));
	char *labeling = malloc(strlen(x) + strlen(errmsg) +1);
	strcpy(labeling, x);
	labeling = strcat(labeling, errmsg);
	gtk_label_set_text(GTK_LABEL(label), labeling);
	gtk_widget_show_all(win);
}
