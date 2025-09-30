#include "canvas.h"


GtkWidget *canvas = NULL;
GdkRGBA color = {.red = 1.0, .green = 1.0, .blue = 1.0, .alpha = 1.0};

byte *canvasData = NULL;

void setupCanvas(PPU *ppu) {
    printf("a\n");
    canvas = gtk_drawing_area_new();
    gtk_drawing_area_set_content_width(canvas, 200);
    printf("a\n");
    gtk_drawing_area_set_content_height(canvas, 200);
    printf("a\n");
    gtk_drawing_area_set_draw_func(GTK_DRAWING_AREA(canvas), draw, NULL, NULL);
    printf("a\n");
    canvasData = ppu->CHRROM;
}


// Draws to the canvas
void draw(GtkDrawingArea *area,
               cairo_t        *cr,
               int             width,
               int             height,
               gpointer        data)
{
  drawPatternTable(cr, canvasData);
}


static void setColorValue(float r, float g, float b, float a){
  color.red = r;
  color.green = g;
  color.blue = b;
  color.alpha = a;
}


void drawPatternTable(cairo_t *cr, byte *chrrom){
  int pixelSize = 1;
  for(int table = 0; table < 2; table++){
    for(int row = 0; row < 16; row++){
      for(int column = 0; column < 16; column++){
        for(int y = 0; y < 8; y++){
          for(int x = 0 ; x < 8; x++){
            byte lowByte = chrrom[y + column*16 + row * 256 + table * 4096];
            byte highByte = chrrom[8 + y + column*16 + row * 256 + table * 4096];
            for(int x = 0; x < 8; x++){
              int TwoBit = ((lowByte >> (7-x)) & 1) == 1 ? 1 : 0;
              TwoBit += ((highByte >> (7-x)) & 1) == 1 ? 2 : 0;

              double gray = TwoBit / 3.0;
              cairo_set_source_rgb(cr, gray, gray, gray);
              int px = x + column * 8 + table*128;
              int py = y + row * 8;

              cairo_rectangle(cr, px*pixelSize, py*pixelSize, pixelSize, pixelSize);
              cairo_fill(cr); 
            }
          }
        }
      }
    }
  }
}