#include "window.h"

GtkWidget *window = NULL;

GtkFileFilter *dotNesFilter = NULL; 
GListModel *fileFilters = NULL;

GtkFileDialog *fileSelectDialog = NULL;


void (*on_run)() = NULL;
void ((*on_load))(char *path) = NULL;

// Callbacks for the buttons on the Menubar
static void run_action(GSimpleAction *action,
                       GVariant *parameter,
                       gpointer user_data) {
    if (on_run == NULL) {
        g_printerr("You forgot to define on_run.\n");
        return;
    }
    on_run();
}

static void select_file(GObject *source, GAsyncResult *result, gpointer user_data) {
    GFile *selectedFile = gtk_file_dialog_open_finish(GTK_FILE_DIALOG(source), result, NULL);
    if (selectedFile != NULL) {
        char *path = g_file_get_parse_name(selectedFile);
        printf("Selected file: %s\n", path);
        on_load(path);

        g_free(path);
        g_object_unref(selectedFile);

       
    } else {
        printf("No file selected.\n");
    }
}
static void load_action(GSimpleAction *action,
                       GVariant *parameter,
                       gpointer user_data) {
    if (on_load == NULL) {
        g_printerr("You forgot to define on_load.\n");
        return;
    }
    fileSelectDialog = gtk_file_dialog_new();
    gtk_file_dialog_set_title(fileSelectDialog, "Select ROM");
    gtk_file_dialog_set_filters(fileSelectDialog, fileFilters);

    gtk_file_dialog_open(fileSelectDialog, GTK_WINDOW(window), NULL, select_file, NULL);
    
    
}



// Called when the window is created
void on_activate(GtkApplication *app)
{
  window = gtk_application_window_new(app);

  // Main Content - Placeholder
  GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
  gtk_window_set_child(GTK_WINDOW(window), box);

  GtkWidget *emulatorLabel = gtk_label_new("Emulator");
  gtk_box_append(GTK_BOX(box), emulatorLabel);

  // Creating the actions for the Menu
  static const GActionEntry app_entries[] = {
      {"run", run_action, NULL, NULL, NULL},
      {"load", load_action, NULL, NULL, NULL}
  };
  
  g_action_map_add_action_entries(G_ACTION_MAP(app),
                                    app_entries,
                                    G_N_ELEMENTS(app_entries),
                                    app);

  
  GMenu *menu = g_menu_new();
  GMenu *emulatorMenu = g_menu_new();

  g_menu_append(emulatorMenu, "Run...", "app.run");
  g_menu_append(emulatorMenu, "Load File", "app.load");

  g_menu_append_submenu(menu, "Emulator", G_MENU_MODEL(emulatorMenu));

  // Creates the Menubar from the Menu Model
  GtkWidget *menubar = gtk_popover_menu_bar_new_from_model(G_MENU_MODEL(menu));
  gtk_box_prepend(GTK_BOX(box), menubar);

  gtk_window_set_title(GTK_WINDOW(window), APP_TITLE);
  gtk_window_set_default_size(GTK_WINDOW(window), 300, 200);
  gtk_widget_show(window);
}



void initGraphics(
                  void (*on_run_func)(), 
                  void (*on_load_func)())
{

  initFileFilters();

  on_run = on_run_func;
  on_load = on_load_func;


}

void initFileFilters(){
  dotNesFilter = gtk_file_filter_new();
  gtk_file_filter_set_name(dotNesFilter, "NES Cartidge File (.nes)");
  gtk_file_filter_add_suffix(dotNesFilter, "nes");

  fileFilters = g_list_store_new(GTK_TYPE_FILE_FILTER);
  g_list_store_append(fileFilters, dotNesFilter);
}

int startGraphics(int argc, char **argv)
{
  GtkApplication *app = gtk_application_new("com.example.GtkApplication",
                                            G_APPLICATION_FLAGS_NONE);

  g_signal_connect(app, "activate", G_CALLBACK(on_activate), NULL);
  return g_application_run(G_APPLICATION(app), argc, argv);
}




