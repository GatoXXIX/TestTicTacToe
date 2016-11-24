#include <pebble.h>
#include "tictactoe_window.h"
#include "tictactoe_bots.h"

Window *gameWindow;
static Layer *grid_layer;
static Layer *game_layer;
int selectd_field_num=0;
Layer *selection_layers[9];
int displayed_selection_layer=0;

/*ttt vars*/
int tictactoe_array[9];
char player = 'x';
char playerComp = 'o';
char lastPlayer = ' ';
int moves = 0;
int won = 0;
int winLine[2];

void check_game_state();

/*draw functions*/
static void draw_grid(Layer *layer, GContext *ctx) {
    Layer *window_layer = window_get_root_layer(gameWindow);
    GRect bounds = layer_get_bounds(window_layer);
    graphics_context_set_stroke_color(ctx, GColorBlack);
    graphics_draw_line(ctx, GPoint((bounds.size.w/3), 0), GPoint((bounds.size.w/3), (bounds.size.h)));
    graphics_draw_line(ctx, GPoint(2*(bounds.size.w/3), 0), GPoint(2*(bounds.size.w/3), (bounds.size.h)));
    graphics_draw_line(ctx, GPoint(0, (bounds.size.h/3)), GPoint((bounds.size.w), (bounds.size.h/3)));
    graphics_draw_line(ctx, GPoint(0, 2*(bounds.size.h/3)), GPoint((bounds.size.w), 2*(bounds.size.h/3)));
}
GPoint get_field_center(int i) {
    int x;
    int y;
    if(i>0 && i<10) {
        Layer *window_layer = window_get_root_layer(gameWindow);
        GRect bounds = layer_get_bounds(window_layer);
        i--;
        x = (i%3)*(bounds.size.w/3) + (bounds.size.w/3)/2;
        y = (i/3)*(bounds.size.h/3) + (bounds.size.h/3)/2;
        return GPoint(x, y);
    }
    APP_LOG(APP_LOG_LEVEL_ERROR,"Field %i doesn't exist", i);
    return GPoint(0, 0);    
}
GRect get_field_rect(int i) {
    int x;
    int y;
    int x_offset;
    int y_offset;
    if(i>0 && i<10) {
        i--;
        Layer *window_layer = window_get_root_layer(gameWindow);
        GRect bounds = layer_get_bounds(window_layer);
        x = (i%3)*(bounds.size.w/3);
        y = (i/3)*(bounds.size.h/3);
        x_offset = (bounds.size.w/3);
        y_offset = (bounds.size.h/3);
        return GRect(x, y, x_offset, y_offset);
    }
    APP_LOG(APP_LOG_LEVEL_ERROR,"Field %i doesn't exist", i);
    return GRect(0, 0, 0, 0);
}
void graphics_draw_line_wide(GContext *ctx, GPoint p0, GPoint p1) {
  for (int x_offset = -1; x_offset < 2; x_offset++) {
    graphics_draw_line(ctx, GPoint(p0.x + x_offset, p0.y), GPoint(p1.x + x_offset, p1.y));
  }
}
void draw_circle_player(GContext* ctx, GPoint center) {
  graphics_context_set_fill_color(ctx, GColorBlack);
  graphics_fill_circle(ctx, center, 15);
  graphics_context_set_fill_color(ctx, GColorWhite);
  graphics_fill_circle(ctx, center, 13);
}
void draw_cross_player(GContext* ctx, GPoint center) {
  graphics_context_set_stroke_color(ctx, GColorBlack);
  graphics_draw_line_wide(ctx, GPoint(center.x - 12, center.y - 12), GPoint(center.x+12, center.y + 12));
  graphics_draw_line_wide(ctx, GPoint(center.x - 12, center.y + 12), GPoint(center.x+12, center.y - 12));
}

void draw_selectors() {
    int i;
    for(i=1; i<10; i++) {
        TextLayer *selected_field = text_layer_create(get_field_rect(i));
        text_layer_set_text_color(selected_field, GColorWhite);
        text_layer_set_background_color(selected_field, GColorBlack);
        selection_layers[i-1] = text_layer_get_layer(selected_field);
    }
}

void display_field_selector(int field) {
    if(displayed_selection_layer != 0) {
        layer_remove_from_parent(selection_layers[displayed_selection_layer-1]);
    }
    if(field > 0 && field < 10) {
        Layer *window_layer = window_get_root_layer(gameWindow);
        layer_add_child(window_layer, selection_layers[field-1]);
        displayed_selection_layer=field;
    }
}
/*draws full field, cause no param for field*/
static void draw_game_state(Layer *layer, GContext *ctx) {
    /*1 == x; -1 == o*/
    int i = 0;
    for(i=0;i<9;i++){
        if(tictactoe_array[i] == 1) {
            draw_cross_player(ctx, get_field_center(i+1));
        } else if(tictactoe_array[i] == -1) {
            draw_circle_player(ctx, get_field_center(i+1));
        }
    }
    if(won == true) {
        graphics_draw_line_wide(ctx, get_field_center(winLine[0]), get_field_center(winLine[1]));
    }
    APP_LOG(APP_LOG_LEVEL_INFO,"Field redrawn");
}
void game_window_update(int field, char player) {
    /*1 == x; -1 == o*/
    if(field>0 && field<10 && moves<9 && won!=1 && tictactoe_array[field-1]==0) {
        if(player == 'x' && lastPlayer != 'x') {
            lastPlayer = 'x';
            tictactoe_array[field-1] = 1;
            moves++;
        } else if(player == 'o' && lastPlayer != 'o') {
            lastPlayer = 'o';
            tictactoe_array[field-1] = -1;
            moves++;
        }
        check_game_state();
    }
}
/*draw functions end*/
/*ttt functions*/
void check_game_state() {
    int h_top;
    int h_middle;
    int h_bottom;
    int v_left;
    int v_middle;
    int v_right;
    int d_lr;
    int d_rl;
    
    h_top = tictactoe_array[0] + tictactoe_array[1] + tictactoe_array[2];
    h_middle = tictactoe_array[3] + tictactoe_array[4] + tictactoe_array[5];
    h_bottom = tictactoe_array[6] + tictactoe_array[7] + tictactoe_array[8];
    v_left = tictactoe_array[0] + tictactoe_array[3] + tictactoe_array[6];
    v_middle = tictactoe_array[1] + tictactoe_array[4] + tictactoe_array[7];
    v_right = tictactoe_array[2] + tictactoe_array[5] + tictactoe_array[8];
    d_lr = tictactoe_array[0] + tictactoe_array[4] + tictactoe_array[8];
    d_rl = tictactoe_array[2] + tictactoe_array[4] + tictactoe_array[6];
    if(h_top == 3 || h_top == -3) {
        won = 1;
        winLine[0] = 1;
        winLine[1] = 3;
    }
    if(h_middle == 3 || h_middle == -3) {
        won = 1;
        winLine[0] = 4;
        winLine[1] = 6;
    }
    if(h_bottom == 3 || h_bottom == -3) {
        won = 1;
        winLine[0] = 7;
        winLine[1] = 9;
    }
    if(v_left == 3 || v_left == -3) {
        won = 1;
        winLine[0] = 1;
        winLine[1] = 7;
    }
    if(v_middle == 3 || v_middle == -3) {
        won = 1;
        winLine[0] = 2;
        winLine[1] = 8;
    }
    if(v_right == 3 || v_right == -3) {
        won = 1;
        winLine[0] = 3;
        winLine[1] = 9;
    }
    if(d_lr == 3 || d_lr == -3) {
        won = 1;
        winLine[0] = 1;
        winLine[1] = 9;
    }
    if(d_rl == 3 || d_rl == -3) {
        won = 1;
        winLine[0] = 3;
        winLine[1] = 7;
    }
    layer_set_update_proc(game_layer, draw_game_state);
    
}
void resetGame() {
    int i;
    for(i=0; i<9; i++) {
        tictactoe_array[i] = 0;
    }
    moves = 0;
    selectd_field_num = 0;
    won = 0;
    /*toggle field selector for window refresh
    display_field_selector(1);*/
    layer_mark_dirty(game_layer);
    display_field_selector(selectd_field_num);
    lastPlayer = ' ';
    check_game_state();
}
/*ttt functions end*/
/*buttons*/
void setNext() {
    selectd_field_num++;
    if(selectd_field_num >9 || selectd_field_num< 1){
        selectd_field_num = 1;
    }
}
void setPrev() {
    selectd_field_num--;
    if(selectd_field_num < 1 || selectd_field_num> 9){
        selectd_field_num = 9;
    }
}
void setNextFree() {
    setNext();
    if(moves < 9) {
        while(tictactoe_array[selectd_field_num-1] != 0) {
            setNext();
        }
    } else {
        selectd_field_num = 0;
    }
}
void setPrevFree() {
    setPrev();
    if(moves < 9) {
        while(tictactoe_array[selectd_field_num-1] != 0) {
            setPrev();
        }
    } else {
        selectd_field_num = 0;
    }
}
void down_single_click_handler(ClickRecognizerRef recognizer, void *context) {
  /*Window *window = (Window *)context;*/
    setNextFree();
    display_field_selector(selectd_field_num);    
}
void select_single_click_handler(ClickRecognizerRef recognizer, void *context) {
  /*Window *window = (Window *)context;*/
    if(won == 0 && moves < 9) {
        game_window_update(selectd_field_num, player);
    
        /*trigger bot move*/
        if(won == 0 && moves < 9) {
            game_window_update(bot_get_next_field(tictactoe_array), playerComp);
        }
        setNextFree();
        display_field_selector(selectd_field_num);
    } else {
        resetGame();
    }
    APP_LOG(APP_LOG_LEVEL_INFO,"Moves %i", moves);
}
void up_single_click_handler(ClickRecognizerRef recognizer, void *context) {
  /*Window *window = (Window *)context;*/
    setPrevFree();
    display_field_selector(selectd_field_num);
}
void back_single_click_handler(ClickRecognizerRef recognizer, void *context) {
    window_stack_pop_all(true);
    game_window_destroy();
}


void config_provider(Window *window) {
    window_single_repeating_click_subscribe(BUTTON_ID_DOWN, 500, down_single_click_handler);
    window_single_click_subscribe(BUTTON_ID_SELECT, select_single_click_handler);
    window_single_repeating_click_subscribe(BUTTON_ID_UP, 500, up_single_click_handler);
     window_single_click_subscribe(BUTTON_ID_BACK, back_single_click_handler);
}
/*buttons end*/
/*Window functions*/
void game_window_load(Window *window){
    Layer *window_layer = window_get_root_layer(window);
    GRect bounds = layer_get_bounds(window_layer);
    
    /*display Grid*/
    grid_layer = layer_create(bounds); 
    layer_set_update_proc(grid_layer, draw_grid);
    layer_add_child(window_layer, grid_layer);
    
    /*display Game Layer(empty)*/
    game_layer = layer_create(bounds);
    layer_add_child(window_layer, game_layer);
    draw_selectors();
}
void game_window_unload(Window *window){
}
void game_window_create(){
	gameWindow = window_create();
    window_set_background_color(gameWindow, GColorWhite);
    window_set_window_handlers(gameWindow, (WindowHandlers) {
        .load = game_window_load,
        .unload = game_window_unload
    });
    window_set_click_config_provider(gameWindow, (ClickConfigProvider) config_provider);
    window_stack_push(gameWindow, true);
}
void game_window_destroy(){
    int i;
    layer_destroy(grid_layer);
    layer_destroy(game_layer);
    for(i=0;i<9;i++){
        layer_destroy(selection_layers[i]);
    }
    window_destroy(gameWindow);
}
Window *game_window_get_window(){
    return gameWindow;
}