#include <pebble.h>
#include "tictactoe_instruction_window.h"
#include "botselect_window.h"

Window *instruction_window;
TextLayer *instructionTitle;

void instruction_select_single_click_handler(ClickRecognizerRef recognizer, void *context) {
    botselect_window_create();
    tictactoe_instruction_window_destroy();    
}

void instruction_back_single_click_handler(ClickRecognizerRef recognizer, void *context) {
    window_stack_pop_all(true);
    tictactoe_instruction_window_destroy();
}

void instruction_config_provider(Window *window) {
    window_single_click_subscribe(BUTTON_ID_SELECT, instruction_select_single_click_handler);
    window_single_click_subscribe(BUTTON_ID_BACK, instruction_back_single_click_handler);
}

static void instruction_window_load(Window *window) {
    Layer *window_layer = window_get_root_layer(window);
    GRect bounds = layer_get_bounds(window_layer);
    instructionTitle = text_layer_create(GRect(0, 0, bounds.size.w, bounds.size.h));
    text_layer_set_text(instructionTitle, "Use UP and DOWN button for field selection. Choose the selected field with the SELECT button. If you don't want the first move, press the SELECT button at first and the computer will move.\n\n Press SELECT to go on.");
    text_layer_set_text_alignment(instructionTitle, GTextAlignmentCenter);/*center*/
    layer_add_child(window_layer, text_layer_get_layer(instructionTitle));
}

void instruction_window_unload(Window *window) {
    text_layer_destroy(instructionTitle);
}

void tictactoe_instruction_window_create() {
  instruction_window = window_create();
  window_set_window_handlers(instruction_window, (WindowHandlers) {
    .load = instruction_window_load,
    .unload = instruction_window_unload,
  });
  window_set_click_config_provider(instruction_window, (ClickConfigProvider) instruction_config_provider);
  window_stack_push(instruction_window, true);
}

/*call from main*/
void tictactoe_instruction_window_destroy() {
  window_destroy(instruction_window);
}
