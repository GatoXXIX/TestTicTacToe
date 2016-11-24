#include <pebble.h>
#include "tictactoe_splash_window.h"

Window *splash_window;
TextLayer *splashTitle;

void tictactoe_splash_window_create();
void tictactoe_splash_window_destroy();

static void splash_window_load(Window *window) {
    Layer *window_layer = window_get_root_layer(window);
    GRect bounds = layer_get_bounds(window_layer);
    splashTitle = text_layer_create(GRect(0, 0, bounds.size.w, bounds.size.h));
    text_layer_set_text(splashTitle, "TIC\nTAC\nTOE");
    text_layer_set_text_alignment(splashTitle, GTextAlignmentCenter);/*center*/
    layer_add_child(window_layer, text_layer_get_layer(splashTitle));
}

void splash_window_unload(Window *window) {
    text_layer_destroy(splashTitle);
}

void tictactoe_splash_window_create() {
  splash_window = window_create();
  window_set_window_handlers(splash_window, (WindowHandlers) {
    .load = splash_window_load,
    .unload = splash_window_unload,
  });
  window_stack_push(splash_window, true);
}

/*call from main*/
void tictactoe_splash_window_destroy() {
  window_destroy(splash_window);
}