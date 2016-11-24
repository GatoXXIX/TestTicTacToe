#include <stdlib.h>
#include <pebble.h>
#include "tictactoe_bots.h"
bot computer = INTELLIGENT_BOT;

void setbot(bot newBot) {
    computer = newBot;
}

int getNthFreeField(int n, int *field) {
    int i = 0;
    while(n > 0 && i<9) {        
        if(field[i] == 0) {
            n--;
        }
        i++;
    }
    return i;
}
int move_drunken_bot(int *field, int movesLeft) {
    int rnd = rand() % (movesLeft+1);
    /*cannot use 0*/
    if(rnd == 0) {
        rnd = 1;
        APP_LOG(APP_LOG_LEVEL_DEBUG,"Set random to 1");
    }
    int f = getNthFreeField(rnd, field);
    return f;
}
int move_defense_bot(int *field, int movesLeft) {
    /*check for 2*/
    int i;
    if(movesLeft > 0) {
        int h_top = field[0] + field[1] + field[2];
        int h_middle = field[3] + field[4] + field[5];
        int h_bottom = field[6] + field[7] + field[8];
        int v_left = field[0] + field[3] + field[6];
        int v_middle = field[1] + field[4] + field[7];
        int v_right = field[2] + field[5] + field[8];
        int d_lr = field[0] + field[4] + field[8];
        int d_rl = field[2] + field[4] + field[6];
        if(h_top == 2) {
            for(i=0;i<3;i++) {
                if(field[i] == 0) {
                    return i+1;
                }
            }
        }
        if(h_middle == 2) {
            for(i=3;i<6;i++) {
                if(field[i] == 0) {
                    return i+1;
                }
            }
        }
        if(h_bottom == 2) {
            for(i=6;i<9;i++) {
                if(field[i] == 0) {
                    return i+1;
                }
            }
        }
        if(v_left == 2) {
            for(i=0;i<=6;i+=3) {
                if(field[i] == 0) {
                    return i+1;
                }
            }
        }
        if(v_middle == 2) {
            for(i=1;i<=7;i+=3) {
                if(field[i] == 0) {
                    return i+1;
                }
            }
        }
        if(v_right == 2) {
            for(i=2;i<=8;i+=3) {
                if(field[i] == 0) {
                    return i+1;
                }
            }
        }
        if(d_lr == 2) {
            for(i=0;i<=8;i+=4) {
                if(field[i] == 0) {
                    return i+1;
                }
            }
        }
        if(d_rl == 2) {
            for(i=2;i<=6;i+=2) {
                if(field[i] == 0) {
                    return i+1;
                }
            }
        }
    }
    return move_drunken_bot(field, movesLeft);
}
int move_intelligent_bot(int *field, int movesLeft) {
    /*check for 2*/
    int i;
    if(movesLeft > 0) {
        int h_top = field[0] + field[1] + field[2];
        int h_middle = field[3] + field[4] + field[5];
        int h_bottom = field[6] + field[7] + field[8];
        int v_left = field[0] + field[3] + field[6];
        int v_middle = field[1] + field[4] + field[7];
        int v_right = field[2] + field[5] + field[8];
        int d_lr = field[0] + field[4] + field[8];
        int d_rl = field[2] + field[4] + field[6];
        if(h_top == -2) {
            for(i=0;i<3;i++) {
                if(field[i] == 0) {
                    return i+1;
                }
            }
        }
        if(h_middle == -2) {
            for(i=3;i<6;i++) {
                if(field[i] == 0) {
                    return i+1;
                }
            }
        }
        if(h_bottom == -2) {
            for(i=6;i<9;i++) {
                if(field[i] == 0) {
                    return i+1;
                }
            }
        }
        if(v_left == -2) {
            for(i=0;i<=6;i+=3) {
                if(field[i] == 0) {
                    return i+1;
                }
            }
        }
        if(v_middle == -2) {
            for(i=1;i<=7;i+=3) {
                if(field[i] == 0) {
                    return i+1;
                }
            }
        }
        if(v_right == -2) {
            for(i=2;i<=8;i+=3) {
                if(field[i] == 0) {
                    return i+1;
                }
            }
        }
        if(d_lr == -2) {
            for(i=0;i<=8;i+=4) {
                if(field[i] == 0) {
                    return i+1;
                }
            }
        }
        if(d_rl == -2) {
            for(i=2;i<=6;i+=2) {
                if(field[i] == 0) {
                    return i+1;
                }
            }
        }
    }
    return move_defense_bot(field, movesLeft);
}
int bot_get_next_field(int *field) {
    int movesLeft=0;
    int i;
    int number = 0;
    for(i=0;i<9;i++){
        if(field[i] == 0) {
            movesLeft++;
        }
    }
    if(computer == DRUNKEN_BOT) {
        number = move_drunken_bot(field, movesLeft);
        APP_LOG(APP_LOG_LEVEL_DEBUG,"Drunken Bot returns %i", number);
    } else if(computer == DEFENSE_BOT) {
        number = move_defense_bot(field, movesLeft);
        APP_LOG(APP_LOG_LEVEL_DEBUG,"Defense Bot returns %i", number);
    } else if(computer == INTELLIGENT_BOT) {
        number = move_intelligent_bot(field, movesLeft);
        APP_LOG(APP_LOG_LEVEL_DEBUG,"Intelligent Bot returns %i", number);
    }
    return number;
}