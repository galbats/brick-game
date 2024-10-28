#ifndef S21_SNAKE_MODEL_H
#define S21_SNAKE_MODEL_H

#include <time.h>

#include <algorithm>
#include <random>

#include "../bricks.h"

namespace s21 {

#define WIDTH 10
#define HEIGTH 20

typedef enum SnakeDir { ToUp, ToDown, ToRight, ToLeft } SnakeDir;

typedef enum GameState_t {
  GameStart,
  Spawn,
  Moving,
  Rotate,
  GameOver
} GameState_t;

class Entities {
 public:
  int x;
  int y;
  Entities();
  Entities(int x, int y);
  Entities generateRandomApple(std::vector<Entities>& snake);
  Entities(const Entities& other);
  Entities& operator=(const Entities& other);
};

class GameLogic {
 public:
  GameInfo_t game_info;
  GameState_t game_state;
  std::vector<Entities> snake_body;
  SnakeDir current_direction;
  SnakeDir next_direction;
  Entities unit;
  bool is_started;
  struct timespec last_move_time;

  GameLogic();
  ~GameLogic();

  int getScore(void);
  void setScore(int high_score);
  void initializeGameInfo(void);
  void initializeSnake(void);
  void generateRandomApple(void);
  void clearGameField(void);
  void updateSnakeOnField(void);
  void updateAppleOnField(void);
  GameInfo_t updateFiguresOnField(void);
  bool checkMove(void);
  void refreshGameInfo(void);
  void moveSnake(void);
};
}  // namespace s21

#endif