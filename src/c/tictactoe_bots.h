#ifndef tictactoe_bots_h
#define tictactoe_bots_h
typedef enum {DRUNKEN_BOT, DEFENSE_BOT, INTELLIGENT_BOT} bot;
int bot_get_next_field(int *field);
void setbot(bot newBot);
#endif
