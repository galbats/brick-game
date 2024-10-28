#ifndef S21_BRICKS_CLI_H
#define S21_BRICKS_CLI_H

#ifdef __cplusplus
extern "C" {
#endif
#include "tetris_cli.h"
#ifdef __cplusplus
}
#endif
#include "snake_cli.h"

void init_ncurses(void);
void end_ncurses(void);
void runRaceGame(void);
#endif
