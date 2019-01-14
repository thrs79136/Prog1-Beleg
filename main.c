// Urheber: Theresa Schuettig s79136

#include <gtk/gtk.h>
#include "person.h"
#include "list.h"
#include <glib.h>
#include <glib/gstdio.h>

enum{
	SURNAME=0,
	FIRSTNAME,
	TELNR,
	NUM
};


typedef struct{
	GtkBuilder *builder;
	GtkWidget *window;
	GtkListStore *store;
	GtkTreeIter iter;
	sList *list;
	GtkWidget *surname;
	GtkWidget *firstname;
	GtkWidget *telnr;
}app_widgets;

void append_item(sPerson *pers, app_widgets *widgets){
	gtk_list_store_append(widgets->store, &(widgets->iter));
	gtk_list_store_set(widgets->store, &(widgets->iter), SURNAME, (gchar*)pers->surName, -1);
	gtk_list_store_set(widgets->store, &(widgets->iter), FIRSTNAME, (gchar*)pers->firstName, -1);
	gtk_list_store_set(widgets->store, &(widgets->iter), TELNR, (gchar*)pers->telephonNr, -1);
}

void print_list(GtkWidget *widget, app_widgets *widgets){
	sPerson *x;
	for (x=front(widgets->list); x; x=next(widgets->list)){
		append_item(x, widgets);
	}
}

void on_editButton_clicked(GtkWidget *widget, app_widgets *widgets){

}

void on_deleteButton_clicked(GtkWidget *widget, app_widgets *widgets){
/*	GtkTreeSelection *selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(widgets->list));
	GtkTreeModel *model;
	model = gtk_tree_view_get_model(GTK_TREE_VIEW(widgets->list));
	if (gtk_tree_model_get_iter_first(model, &(widgets->iter)==FALSE) return;
	if (gtk_tree_selection_get_selected(GTK_TREE_SELECTION(selection)), model, &(widgets->iter))){
		gtk_list_store_remove(widgets->store, &(widgets->iter));
	}
*/		
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
void on_addButton_clicked(GtkWidget *widget, app_widgets *widgets){
	widgets->window  = GTK_WIDGET(gtk_builder_get_object(widgets->builder, "newEntryWin"));
	widgets->surname = GTK_WIDGET(gtk_builder_get_object(widgets->builder, "surnameEntry"));
	widgets->firstname = GTK_WIDGET(gtk_builder_get_object(widgets->builder, "firstNameEntry"));
	widgets->telnr = GTK_WIDGET(gtk_builder_get_object(widgets->builder, "telNrEntry"));

	gtk_widget_show_all(widgets->window);
}

void on_addButton2_clicked(GtkWidget *widget, app_widgets *widgets){

	char *sur = (char*)gtk_entry_get_text(GTK_ENTRY(widgets->surname));
	char *first = (char*)gtk_entry_get_text(GTK_ENTRY(widgets->firstname));
	char *tel = (char*)gtk_entry_get_text(GTK_ENTRY(widgets->telnr));
	sPerson *pp = getPers(sur, first, tel);

	putPers(pp);
	push_front(widgets->list, pp);
	append_item(pp, widgets);
	gtk_widget_hide(widgets->window);
	gtk_entry_set_text(GTK_ENTRY(widgets->surname), "");
	gtk_entry_set_text(GTK_ENTRY(widgets->firstname), "");
	gtk_entry_set_text(GTK_ENTRY(widgets->telnr), "");
}

void on_mainWindow_destroy(GtkWidget *widget, app_widgets *widgets){

	FILE *pf = g_fopen("Daten.csv", "wt");
	if (!pf); // Warnung ausgeben, Abfragen ob Fenster dennoch geschlossen werden soll
	printf("%d\n", saveData(widgets->list, pf));
	fclose(pf);
	gtk_main_quit();
}

int main(int argc, char *argv[]){


	GtkWidget *mainWindow;
	gtk_init(&argc, &argv);
	app_widgets *widgets = g_slice_new(app_widgets);
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

