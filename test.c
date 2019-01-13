#include <gtk/gtk.h>
#include <glib.h>
#include <glib/gstdio.h>
int main(int argc, char *argv[]){

	gtk_init(&argc, &argv);

	gchar *x = "asdfasdf";

	FILE *pf = g_fopen("uio.txt", "wt");
	if (!pf) exit(-1);
	fprintf(pf, "%s,%s,%s\n", "adsf", "qwer", "ewrt");

	fclose(pf);

	return 0;

}
