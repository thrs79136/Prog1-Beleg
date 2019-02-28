// © by Theresa Schuettig, s79136

#include <gtk/gtk.h>
#include <stdlib.h>
#include "person.h"
#include "list.h"
#include "gtk.h"

int main(int argc, char *argv[]){
	GtkWidget *mainWindow;
	gtk_init(&argc, &argv);

	// Speicherreservierung fuer Pointer auf Widgets und Aehnlichem
	widgets = g_slice_new(app_widgets);
	if (!widgets){
		printf("Fehler bei Speicherreservierung\n");
		exit(-1);
	}

	widgets->builder = gtk_builder_new();
	if (!widgets->builder){
		printf("Builder konnte nicht erstellt werden\n");
		exit(-1);
	}
	gtk_builder_add_from_file(widgets->builder, "interface.xml", NULL);
		// Einrichten der Signalhandler
	gtk_builder_connect_signals(widgets->builder, widgets);

	widgets->list = createList();
	widgets->sortedList = createList();
	if (!widgets->list || !widgets->sortedList) error("Fehler beim Erstellen der Listen", widgets->builder);
	
	FILE *pf = fopen("Daten.csv", "rt");
	if (pf){
		if (!readData(widgets->list, widgets->sortedList, pf, cmpName)) error ("Fehler beim Lesen der Daten", widgets->builder);
		fclose(pf);
	}

	mainWindow = GTK_WIDGET(gtk_builder_get_object(widgets->builder, "mainWindow"));
	widgets->store = GTK_LIST_STORE(gtk_builder_get_object(widgets->builder, "data"));
	widgets->treeview = GTK_TREE_VIEW(gtk_builder_get_object(widgets->builder,"treeview"));
	widgets->model = gtk_tree_view_get_model(GTK_TREE_VIEW(widgets->treeview));
	widgets->sortButton = GTK_WIDGET(gtk_builder_get_object(widgets->builder, "sortButton"));
	widgets->searchEntry = GTK_WIDGET(gtk_builder_get_object(widgets->builder, "searchEntry"));
	// Liste ist bei Start chronologisch sortiert
	widgets->chronSorted = 1;

	gtk_widget_show_all(mainWindow);
	// Anzeigen der Eintraege
	print_list(widgets->list, widgets);
	gtk_main();
	g_object_unref(widgets->builder);

	return 0;
}
