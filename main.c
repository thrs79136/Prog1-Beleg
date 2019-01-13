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
	sList *list;
	GtkWidget *surname;
	GtkWidget *firstname;
	GtkWidget *telnr;
	GtkWidget *window;
}app_widgets;

void append_item(sPerson *pers, app_widgets *app_widgets){
	GtkListStore *store;
	GtkTreeIter iter;
	store = GTK_LIST_STORE(gtk_builder_get_object(app_widgets->builder, "daten"));
	gtk_list_store_append(store, &iter);
	gtk_list_store_set(store, &iter, SURNAME, (gchar*)pers->surName, -1);
	gtk_list_store_set(store, &iter, FIRSTNAME, (gchar*)pers->firstName, -1);
	gtk_list_store_set(store, &iter, TELNR, (gchar*)pers->telephonNr, -1);
}

void print_list(GtkWidget *widget, app_widgets *app_widgets){
	sPerson *x;
	for (x=front(app_widgets->list); x; x=next(app_widgets->list)){
		append_item(x, app_widgets);
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
void on_addButton_clicked(GtkWidget *widget, app_widgets *app_widgets){
	app_widgets->window  = GTK_WIDGET(gtk_builder_get_object(app_widgets->builder, "newEntryWin"));
	app_widgets->surname = GTK_WIDGET(gtk_builder_get_object(app_widgets->builder, "surnameEntry"));
	app_widgets->firstname = GTK_WIDGET(gtk_builder_get_object(app_widgets->builder, "firstNameEntry"));
	app_widgets->telnr = GTK_WIDGET(gtk_builder_get_object(app_widgets->builder, "telNrEntry"));

	gtk_widget_show_all(app_widgets->window);
}

void on_addButton2_clicked(GtkWidget *widget, app_widgets *app_widgets){

	char *sur = (char*)gtk_entry_get_text(GTK_ENTRY(app_widgets->surname));
	char *first = (char*)gtk_entry_get_text(GTK_ENTRY(app_widgets->firstname));
	char *tel = (char*)gtk_entry_get_text(GTK_ENTRY(app_widgets->telnr));
	sPerson *pp = getPers(sur, first, tel);

	putPers(pp);
	push_front(app_widgets->list, pp);
	append_item(pp, app_widgets);
	gtk_widget_destroy(app_widgets->window);
}

void on_mainWindow_destroy(GtkWidget *widgets, app_widgets *app_widgets){

	FILE *pf = g_fopen("Daten.csv", "wt");
	if (!pf); // Warnung ausgeben, Abfragen ob Fenster dennoch geschlossen werden soll
	printf("%d\n", saveData(app_widgets->list, pf));
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

	gtk_builder_connect_signals(widgets->builder, widgets);

	gtk_widget_show_all(mainWindow);
	print_list(NULL, widgets);
	gtk_main();
	g_object_unref(widgets->builder);

	return 0;
}

