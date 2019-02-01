// © by Theresa Schuettig, s79136

/* TO DO
 * Suchen ermoeglichen (ueber Telnr)
 * Tabelle muss nach Hinzufuegen und Bearbeiten eines Eintrags entfernt und wieder neu angezeigt werden
 * ideal: auch in der TreeView sortiert einfuegen oder nur Selection aendern
 * Sortieren im Fenster ermoeglichen
*/


#include <gtk/gtk.h>
#include <glib.h>
#include <glib/gprintf.h>
#include <ctype.h>
#include "person.h"
#include "list.h"

enum{
	SURNAME=0,
	FIRSTNAME,
	TELNR,
	NUM
};

// evtl. GtkTreeView hinzufuegen

typedef struct{
	sList *list;
	GtkBuilder *builder;
	GtkWidget *window;
	GtkListStore *store;
	GtkTreeIter iter;
	GtkWidget *surname;
	GtkWidget *firstname;
	GtkWidget *telnr;
}app_widgets;

void append_item(sPerson *pers, app_widgets *widgets){
	gtk_list_store_append(widgets->store, &(widgets->iter));
	gtk_list_store_set(	widgets->store, &(widgets->iter),
						SURNAME, (gchar*)pers->surName,
						FIRSTNAME, (gchar*)pers->firstName,
						TELNR, (gchar*)pers->telephonNr, -1);
}

void print_list(GtkWidget *widget, app_widgets *widgets){
	sPerson *x;
	for (x=front(widgets->list); x; x=next(widgets->list)){
		append_item(x, widgets);
	}
}

void on_saveButton_clicked(GtkWidget *widget, app_widgets *widgets){
	gchar *gSur, *gFirst, *gTelNr;
	gSur  = (gchar*)gtk_entry_get_text(GTK_ENTRY(widgets->surname));
	gFirst = (gchar*)gtk_entry_get_text(GTK_ENTRY(widgets->firstname));
	gTelNr = (gchar*)gtk_entry_get_text(GTK_ENTRY(widgets->telnr));
	g_print("%s %s %s\n", gSur, gFirst, gTelNr);
	int edited = 0;
	for (next(widgets->list); current(widgets->list) && !edited; next(widgets->list)){
		if ( cmpTelephonNr((void*)gTelNr, current(widgets->list)) == 0 ){
			editPers(current(widgets->list), (char*)gSur, (char*)gFirst, (gchar*)gTelNr);
			edited = 1;
		}
	}
	showList(widgets->list);
	gtk_widget_hide(widgets->window);
}

// Bearbeiten der Eintraege
void on_editButton_clicked(GtkWidget *widget, app_widgets *widgets){
	GtkTreeView *treeview = GTK_TREE_VIEW(gtk_builder_get_object(widgets->builder,"treeview"));
	GtkTreeSelection *selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(treeview));
	GtkTreeModel *model = gtk_tree_view_get_model(GTK_TREE_VIEW(treeview));
	widgets->store = GTK_LIST_STORE(gtk_builder_get_object(widgets->builder, "data"));
	if (gtk_tree_model_get_iter_first(model, &widgets->iter)==FALSE) return;
	if (gtk_tree_selection_get_selected(GTK_TREE_SELECTION(selection), &model, &widgets->iter)){
		gchar *gSur, *gFirst, *gTelNr;
		// Daten des ausgewaehlten Datensatzes werden in den gchars gespeichert
		gtk_tree_model_get(	model, &widgets->iter,
							SURNAME, &gSur, FIRSTNAME,
							&gFirst, TELNR,
							&gTelNr, -1);
		widgets->window = GTK_WIDGET(gtk_builder_get_object(widgets->builder, "editEntryWin"));
		gtk_widget_show_all(widgets->window);

		widgets->surname = GTK_WIDGET(gtk_builder_get_object(widgets->builder, "changeSurnameEntry"));
		widgets->firstname = GTK_WIDGET(gtk_builder_get_object(widgets->builder, "changeFirstNameEntry"));
		widgets->telnr = GTK_WIDGET(gtk_builder_get_object(widgets->builder, "changeTelNrEntry"));
		// Namen und Telefonnummer werden in Eingabefelder eingefuegt
		gtk_entry_set_text(GTK_ENTRY(widgets->surname), gSur);
		gtk_entry_set_text(GTK_ENTRY(widgets->firstname), gFirst);
		gtk_entry_set_text(GTK_ENTRY(widgets->telnr), gTelNr);
	}
}

void on_deleteButton_clicked(GtkWidget *widget, app_widgets *widgets){
	GtkTreeView *treeview  = GTK_TREE_VIEW(gtk_builder_get_object(widgets->builder, "treeview"));
	widgets->store = GTK_LIST_STORE(gtk_builder_get_object(widgets->builder, "data"));
	GtkTreeSelection *selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(treeview));
	GtkTreeModel *model = gtk_tree_view_get_model(GTK_TREE_VIEW(treeview));
	if (gtk_tree_model_get_iter_first(model, &widgets->iter)==FALSE) return;
	if (gtk_tree_selection_get_selected(GTK_TREE_SELECTION(selection), &model, &widgets->iter)){
		// Telefonnummer des zu loeschenden Eintrags wird ermittelt...
		char *telNr;
		gtk_tree_model_get(model, &widgets->iter, TELNR, &telNr, -1);
		g_print("%s\n", telNr);
		// ...und die dazugehoerige Person aus der List entfernt
		removeItem(widgets->list, (void*)telNr, cmpTelephonNr);
		
		// Eintrag wird aus TreeView entfernt
		gtk_list_store_remove(widgets->store, &widgets->iter);

		// Liste wird aktualisiert angezeigt
		showList(widgets->list);
	}
		
}

/*void append_item(GtkWidget *widget, app_widgets *app_wdgts){
  GtkListStore *store;
  GtkTreeIter iter;
  g_print("Aufgerufen");

  const gchar *sur = "Reitel";
  const gchar *first = "Charly";
  const gchar *tel = "102831293";

  store = GTK_LIST_STORE(gtk_builder_get_object(app_wdgts->builder, "daten"));

  gtk_list_store_append(store, &iter);
  gtk_list_store_set(store, &iter, SURNAME, sur, -1);
  gtk_list_store_set(store, &iter, FIRSTNAME, first, -1);
  gtk_list_store_set(store, &iter, TELNR, tel, -1);
  }
  */

// prueft, ob Eingabe des neuen Eintrags zulaessig ist
// Telefonnr. existiert noch nicht
// Eingabefelder sind nicht leer, Telefonnr. beinhaltet nur Ziffern und Namen nur Buchstaben
void checkValidInput(GtkWidget *widget, app_widgets *widgets){
	GtkWidget *button = GTK_WIDGET(gtk_builder_get_object(widgets->builder, "addButton2"));
	const gchar *sur = gtk_entry_get_text(GTK_ENTRY(widgets->surname));
	const gchar *first =gtk_entry_get_text(GTK_ENTRY(widgets->firstname));
	const gchar *tel =gtk_entry_get_text(GTK_ENTRY(widgets->telnr));
	gboolean isvalid = TRUE;
	int i;
	// ueberprueft, ob eines der Eingabefelder leer ist
	if (sur[0]==0 || first[0]==0 || tel[0]==0) isvalid=FALSE;
	for (i=0; sur[i]; i++){
		gunichar c = g_utf8_get_char(sur+i);
		if (!g_unichar_isalpha(c) && !g_ascii_isspace(sur[i])) isvalid = FALSE;
	}
	for (i=0; first[i]; i++){
		gunichar c = g_utf8_get_char(first+i);
		if (!g_unichar_isalpha(c) && !g_ascii_isspace(first[i])) isvalid = FALSE;
	}
	for (i=0; tel[i]; i++) if (!g_ascii_isdigit(tel[i])) isvalid=0;
	gtk_widget_set_sensitive (button, isvalid);
}

// Oeffnet Fenster, das das Erstellen eines neuen Eintrags ermoeglicht
void on_addButton_clicked(GtkWidget *widget, app_widgets *widgets){
	GtkWidget *button = GTK_WIDGET(gtk_builder_get_object(widgets->builder, "addButton2"));
	gtk_widget_set_sensitive (button, FALSE);
	widgets->window  = GTK_WIDGET(gtk_builder_get_object(widgets->builder, "newEntryWin"));
	widgets->surname = GTK_WIDGET(gtk_builder_get_object(widgets->builder, "surnameEntry"));
	widgets->firstname = GTK_WIDGET(gtk_builder_get_object(widgets->builder, "firstNameEntry"));
	widgets->telnr = GTK_WIDGET(gtk_builder_get_object(widgets->builder, "telNrEntry"));

	gtk_widget_show_all(widgets->window);
}

// Neuer Eintrag wurde bestaetigt und wird gespeichert
void on_addButton2_clicked(GtkWidget *widget, app_widgets *widgets){

	char *sur = (char*)gtk_entry_get_text(GTK_ENTRY(widgets->surname));
	char *first = (char*)gtk_entry_get_text(GTK_ENTRY(widgets->firstname));
	char *tel = (char*)gtk_entry_get_text(GTK_ENTRY(widgets->telnr));
	sPerson *pp = getPers(sur, first, tel);

	putPers(pp);
	insertSorted(widgets->list, pp, cmpName);
	showList(widgets->list);
	append_item(pp, widgets);
	gtk_widget_hide(widgets->window);
	gtk_entry_set_text(GTK_ENTRY(widgets->surname), "");
	gtk_entry_set_text(GTK_ENTRY(widgets->firstname), "");
	gtk_entry_set_text(GTK_ENTRY(widgets->telnr), "");
}

void on_mainWindow_destroy(GtkWidget *widget, app_widgets *widgets){

	FILE *pf = fopen("Daten.csv", "wt");
	if (!pf); // Warnung ausgeben, Abfragen ob Fenster dennoch geschlossen werden soll
	printf("%d\n", saveData(widgets->list, pf));
	sPerson *x;
	for (x=front(widgets->list); x; x=next(widgets->list))
		free(x);
	deleteList(widgets->list);
	fclose(pf);
	gtk_main_quit();
}

int main(int argc, char *argv[]){


	GtkWidget *mainWindow;
	gtk_init(&argc, &argv);
	app_widgets *widgets = g_slice_new(app_widgets);
	// g_free?
	widgets->list = createList();
	FILE *pf = fopen("Daten.csv", "rt");
	if (pf){
		readData(widgets->list, pf);
		fclose(pf);
	}

	widgets->builder = gtk_builder_new();
	gtk_builder_add_from_file(widgets->builder, "interface.xml", NULL);
	mainWindow = GTK_WIDGET(gtk_builder_get_object(widgets->builder, "mainWindow"));
	widgets->store = GTK_LIST_STORE(gtk_builder_get_object(widgets->builder, "data"));
	gtk_builder_connect_signals(widgets->builder, widgets);

	gtk_widget_show_all(mainWindow);
	print_list(NULL, widgets);
	gtk_main();
	g_object_unref(widgets->builder);

	return 0;
}

