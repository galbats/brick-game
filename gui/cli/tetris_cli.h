#ifndef S21_TETRIS_CLI_H
#define S21_TETRIS_CLI_H

#include <ncurses.h>

#include "../../brick_game/tetris/tetris.h"

void runTetrisGame(void);
UserAction_t getAction(int user_input);

void renderGame(WINDOW *field, WINDOW *player, GameInfo_t game);
void initializeColors();
WINDOW *createWindow(int height, int width, int starty, int startx);
void processUserInput();
void gameLoop(WINDOW *field, WINDOW *current_game);
void cleanupGame(WINDOW *field, WINDOW *current_game);

void renderField(WINDOW *field, GameInfo_t game);
void renderNextPiece(WINDOW *player, GameInfo_t game);
void renderGameInfo(WINDOW *player, GameInfo_t game);

#endif
