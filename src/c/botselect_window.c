#include <pebble.h>
#include "botselect_window.h"
#include "tictactoe_bots.h"
#include "tictactoe_window.h"

#define NUM_MENU_ITEMS 3
#define NUM_MENU_SECTIONS 1

Window *botselect_window;
static SimpleMenuLayer *s_simple_menu_layer;

static SimpleMenuItem menu_items[NUM_MENU_ITEMS];
static SimpleMenuSection menu_sections[NUM_MENU_SECTIONS];

static void menu_select_callback(int index, void *ctx) {
    /*s_first_menu_items[index].subtitle = "You've hit select here!";*/
    switch(index) {
        case 0: 
            setbot(DRUNKEN_BOT);
            break;
        case 1:
            setbot(DEFENSE_BOT);
            break;
        case 2:
            setbot(INTELLIGENT_BOT);
            break;
    }
    game_window_create();
    botselect_window_destroy();
    /*layer_mark_dirty(simple_menu_layer_get_layer(s_simple_menu_layer));*/
    
}


static void botselect_window_load(Window *window) {

  // Although we already defined NUM_FIRST_MENU_ITEMS, you can define
  // an int as such to easily change the order of menu items later
  int num_a_items = 0;

  menu_items[num_a_items++] = (SimpleMenuItem) {
    .title = "DRUNKEN BOT",
    .subtitle = "all fortune",
    .callback = menu_select_callback,
  };
  menu_items[num_a_items++] = (SimpleMenuItem) {
    .title = "DEFENCE BOT",
    .subtitle = "adamantly defend",
    .callback = menu_select_callback,
  };
  menu_items[num_a_items++] = (SimpleMenuItem) {
    .title = "INTELLIGENT BOT",
    .subtitle = "still not invincible",
    .callback = menu_select_callback,
  };

    menu_sections[0] = (SimpleMenuSection) {
        .num_items = NUM_MENU_ITEMS,
        .items = menu_items,
    };
    
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_frame(window_layer);

  s_simple_menu_layer = simple_menu_layer_create(bounds, window, menu_sections, NUM_MENU_SECTIONS, NULL);

  layer_add_child(window_layer, simple_menu_layer_get_layer(s_simple_menu_layer));
}



void botselect_window_unload(Window *window) {
  simple_menu_layer_destroy(s_simple_menu_layer);
}

void botselect_window_create() {
  botselect_window = window_create();
  window_set_window_handlers(botselect_window, (WindowHandlers) {
    .load = botselect_window_load,
    .unload = botselect_window_unload,
  });
  window_stack_push(botselect_window, true);
}

/*call from main*/
void botselect_window_destroy() {
  window_destroy(botselect_window);
}