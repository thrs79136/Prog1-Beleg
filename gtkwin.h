void add_item(sPerson *pers, app_widgets *widgets, int append){
void print_search_results(GtkWidget *widget, app_widgets *widgets){
	if (gtk_tree_model_get_iter_first(widgets->model, &widgets->iter) == TRUE){
	for (pPers=front(pList); pPers; pPers=next(pList)){
void print_list(sList *pList, app_widgets *widgets){
	if (gtk_tree_model_get_iter_first(widgets->model, &widgets->iter) == TRUE){
	for (pPers=front(pList); pPers; pPers=next(pList)){
void on_saveButton_clicked(GtkWidget *widget, app_widgets *widgets){
	if (strcmp(cTelNr, cOldNr) != 0){
	if (!number_exists){
		for (pPers=front(widgets->list); pPers; pPers=next(widgets->list)){
			if ( cmpTelephonNr(cOldNr, pPers) == 0 ){
void on_editButton_clicked(GtkWidget *widget, app_widgets *widgets){
	if (gtk_tree_selection_get_selected(GTK_TREE_SELECTION(widgets->selection), &widgets->model, &widgets->iter)){
void on_deleteButton_clicked(GtkWidget *widget, app_widgets *widgets){
	if (gtk_tree_selection_get_selected(GTK_TREE_SELECTION(widgets->selection), &widgets->model, &widgets->iter)){
void on_yesButton_clicked(GtkWidget *widget, app_widgets *widgets){
int isValidChar(char *c){
void checkValidInput(GtkWidget *widget, app_widgets *widgets){
void on_addButton_clicked(GtkWidget *widget, app_widgets *widgets){
void on_addButton2_clicked(GtkWidget *widget, app_widgets *widgets){
	if (!number_exists){
void on_sortButton_clicked(GtkWidget *widget, app_widgets *widgets){
		if (widgets->chronSorted){
void on_mainWindow_destroy(GtkWidget *widget, app_widgets *widgets){
int main(int argc, char *argv[]){
	if (pf){
