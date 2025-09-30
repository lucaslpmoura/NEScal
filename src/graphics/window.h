#ifndef WINDOW_H
#define WINDOW_H

#include "PPU.h"
#include "types.h";

#include <gtk/gtk.h>


#define APP_TITLE "NEScal"
extern void(*on_run)();
extern void(*on_load)(char *path);

extern GtkWidget *window;

extern GtkFileFilter *dotNesFilter;
extern GListModel *fileFilters;

void on_activate (GtkApplication *app);
void initGraphics(  PPU *ppu,
                    void (*on_run_func)(), 
                    void (*on_load_func)());

void initFileFilters();

int startGraphics(int argc, char **argv);

#endif 