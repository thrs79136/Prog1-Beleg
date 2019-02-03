// © by Theresa Schuettig, s79136

/* TO DO
 * Suchen ermoeglichen (ueber Telnr)
 * Tabelle muss nach Hinzufuegen und Bearbeiten eines Eintrags entfernt und wieder neu angezeigt werden
 * ideal: auch in der TreeView sortiert einfuegen oder nur Selection aendern
 * Sortieren im Fenster ermoeglichen
*/

#include <signal.h>
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
	GtkWidget *button;
	GtkWidget *label;
	GtkListStore *store;
	GtkTreeIter iter;
	GtkWidget *surname;
	GtkWidget *firstname;
	GtkWidget *telnr;
}app_widgets;

void prepend_item(sPerson *pers, app_widgets *widgets){
	gtk_list_store_prepend(widgets->store, &(widgets->iter));
	gtk_list_store_set(	widgets->store, &(widgets->iter),
						SURNAME, (gchar*)pers->surName,
						FIRSTNAME, (gchar*)pers->firstName,
						TELNR, (gchar*)pers->telephonNr, -1);
}

void print_list(GtkWidget *widget, app_widgets *widgets){
	sPerson *x;
	for (x=front(widgets->list); x; x=next(widgets->list)){
		prepend_item(x, widgets);
	}
}

void on_saveButton_clicked(GtkWidget *widget, app_widgets *widgets){
	gchar *gSur, *gFirst, *gTelNr;
	gSur  = (gchar*)gtk_entry_get_text(GTK_ENTRY(widgets->surname));
	gFirst = (gchar*)gtk_entry_get_text(GTK_ENTRY(widgets->firstname));
	gTelNr = (gchar*)gtk_entry_get_text(GTK_ENTRY(widgets->telnr));
	g_print("%s %s %s\n", gSur, gFirst, gTelNr);
	for (next(widgets->list); current(widgets->list); next(widgets->list)){
		if ( cmpTelephonNr((void*)gTelNr, current(widgets->list)) == 0 ){
			editPers(current(widgets->list), (char*)gSur, (char*)gFirst, (gchar*)gTelNr);
			break;
		}
	}
	gtk_widget_hide(widgets->window);
	g_free(gSur); g_free(gFirst); g_free(gTelNr);
}

// Bearbeiten der Eintraege
// ! Eintrag muss noch innerhalb des Fensters geandert werden!
void on_editButton_clicked(GtkWidget *widget, app_widgets *widgets){
	widgets->label = GTK_WIDGET(gtk_builder_get_object(widgets->builder, "numberExistsLabel2"));
	widgets->button = GTK_WIDGET(gtk_builder_get_object(widgets->builder, "saveButton"));
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
		gtk_widget_hide(widgets->label);

		widgets->surname = GTK_WIDGET(gtk_builder_get_object(widgets->builder, "changeSurnameEntry"));
		widgets->firstname = GTK_WIDGET(gtk_builder_get_object(widgets->builder, "changeFirstNameEntry"));
		widgets->telnr = GTK_WIDGET(gtk_builder_get_object(widgets->builder, "changeTelNrEntry"));
		// Namen und Telefonnummer werden in Eingabefelder eingefuegt
		gtk_entry_set_text(GTK_ENTRY(widgets->surname), gSur);
		gtk_entry_set_text(GTK_ENTRY(widgets->firstname), gFirst);
		gtk_entry_set_text(GTK_ENTRY(widgets->telnr), gTelNr);
		g_free(gSur); g_free(gFirst); g_free(gTelNr);
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
	}		
}

// passenderer Name muss noch gesucht werden
// prueft, ob es sich um einen Punkt, ein Leerzeichen oder einen Buchstaben der deutschen Sprache handelt
int isletter(char *c){
	if (isalpha(c[1]) || c[1] =='.' || c[1] ==' '|| c[1]==0 ) return 1;
	if (c[0]==-61 && (c[1]==-92 || c[1]==-74 || c[1]==-68 ||c[1]==-97 ||c[1]==-124 ||c[1]==-100 ||c[1]==-106)) return 1;
	return 0;
}

// prueft, ob Eingabe des neuen Eintrags zulaessig ist
// Eingabefelder sind nicht leer, Telefonnr. beinhaltet nur Ziffern und Namen nur Buchstaben und Punkte
void checkValidInput(GtkWidget *widget, app_widgets *widgets){
	widgets->button = GTK_WIDGET(gtk_builder_get_object(widgets->builder, "addButton2"));
	char *sur = (char*)gtk_entry_get_text(GTK_ENTRY(widgets->surname));
	puts("a");
	char *first =(char*)gtk_entry_get_text(GTK_ENTRY(widgets->firstname));
	puts("b");
	char *tel =(char*)gtk_entry_get_text(GTK_ENTRY(widgets->telnr));
	puts("c");
	static gboolean isvalid = FALSE;
	// ueberprueft, ob eines der Eingabefelder leer ist
	if (sur[0]==0 || first[0]==0 || tel[0]==0) isvalid = FALSE;
	else if (strlen(sur) == 1 && ( !isalpha(sur[strlen(sur)-1]) || sur[strlen(sur)-1]=='.' )) isvalid = FALSE;
	else if (!isletter(&sur[strlen(sur)-2])) isvalid = FALSE;
	else if (strlen(first) == 1 && ( !isalpha(first[strlen(first)-1]) || first[strlen(first)-1] == '.' )) isvalid = FALSE;
	else if (!isletter(&first[strlen(first)-2])) isvalid = FALSE;
	else if (!isdigit(tel[strlen(tel)-1])) isvalid = FALSE;
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
	for (x=front(widgets->list); x; x=next(widgets->list)) if (cmpTelephonNr((void*)tel, x) ==0) number_exists = 1;
	// existiert sie noch nicht, so wird ein neuer Eintrag erstellt
	if (!number_exists){
		char *sur = (char*)gtk_entry_get_text(GTK_ENTRY(widgets->surname));
		char *first = (char*)gtk_entry_get_text(GTK_ENTRY(widgets->firstname));	
		sPerson *pp = getPers(sur, first, tel);

		putPers(pp);
		push_front(widgets->list, pp);
		showList(widgets->list);
		prepend_item(pp, widgets);
		gtk_widget_hide(widgets->window);
		gtk_entry_set_text(GTK_ENTRY(widgets->surname), "");
		gtk_entry_set_text(GTK_ENTRY(widgets->firstname), "");
		gtk_entry_set_text(GTK_ENTRY(widgets->telnr), "");
	}	
	else {
		gtk_widget_show(warningLabel);
		g_print("Nummer existiert bereits\n");
	}
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
	signal(SIGINT, SIG_IGN);
	signal(SIGKILL, SIG_IGN);
	signal(SIGTSTP, SIG_IGN);

	gtk_widget_show_all(mainWindow);
	print_list(NULL, widgets);
	gtk_main();
	g_object_unref(widgets->builder);

	return 0;
}

