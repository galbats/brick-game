#ifndef S21_SNAKE_CONTROLLER_H
#define S21_SNAKE_CONTROLLER_H

#include "../bricks.h"
#include "model_snake.h"

namespace s21 {

class Controller {
 public:
  GameLogic *game;

  Controller(GameLogic *game);
  ~Controller();

  void userInput(UserAction_t action, bool hold);
  GameInfo_t updateCurrentState();
};

}  // namespace s21

#endif