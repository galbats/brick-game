#ifndef S21_SNAKE_CLI_H
#define S21_SNAKE_CLI_H

#include <ncurses.h>

#include "../../brick_game/snake/controller_snake.h"

namespace s21 {

class View {
 private:
  Controller *snake;

 public:
  View(Controller *snake);
  ~View();

  void runSnakeGame(void);
  UserAction_t getAction(int user_input);

  void renderGame(WINDOW *field, WINDOW *player, GameInfo_t game);
  void renderField(WINDOW *field, GameInfo_t game);
  void renderInfo(WINDOW *player, GameInfo_t game);

 private:
  void initializeColors();
  WINDOW *createWindow(int height, int width, int starty, int startx);
  void processInput();
};

}  // namespace s21

#endif
