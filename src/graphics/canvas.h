#include "types.h"
#include "PPU.h"
#include <gtk/gtk.h>

extern PPU *ppu;

extern GtkWidget *canvas;
extern GdkRGBA color;


void setupCanvas(PPU *ppu);
void draw(GtkDrawingArea *area,
               cairo_t        *cr,
               int             width,
               int             height,
               gpointer        data);
void drawPatternTable(cairo_t *cr, byte *chrrom);