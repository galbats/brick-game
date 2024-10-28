#include "controller_snake.h"

namespace s21 {

Controller::Controller(GameLogic *game) : game{game} {}
Controller::~Controller() {}

void Controller::userInput(UserAction_t action, bool hold) {
  auto game_state = game->game_state;
  auto pause = game->game_info.pause;
  switch (action) {
    case Start:
      game->is_started = true;
      break;
    case Pause:
      if (game_state != GameStart) game->game_info.pause = !pause;
      break;
    case Terminate:
      game->game_state = GameOver;
      break;
    case Left:
      if (game->current_direction != ToRight) {
        game->next_direction = ToLeft;
      }
      break;
    case Right:
      if (game->current_direction != ToLeft) {
        game->next_direction = ToRight;
      }
      break;
    case Up:
      if (game->current_direction != ToDown) {
        game->next_direction = ToUp;
      }
      break;
    case Down:
      if (game->current_direction != ToUp) {
        game->next_direction = ToDown;
      }
      break;
    case Action:
      if (hold) game->moveSnake();
      break;
    default:
      break;
  }
}

GameInfo_t Controller::updateCurrentState() {
  // Проверяем, не стоит ли игра на паузе и начата ли она
  if (!game->game_info.pause && game->is_started) {
    struct timespec current_time;
    double move_interval;

    switch (game->game_state) {
      case GameStart:
        // Инициализируем начальные данные игры и переходим в состояние движения
        game->initializeGameInfo();
        game->game_state = Moving;
        break;

      case Spawn:
        // Генерируем случайное яблоко и переходим в состояние движения
        game->generateRandomApple();
        game->game_state = Moving;
        break;

      case Rotate:
        // Двигаем змейку
        game->moveSnake();
        break;

      case Moving:
        // Проверяем, прошло ли достаточно времени для следующего хода
        clock_gettime(CLOCK_REALTIME, &current_time);
        move_interval =
            (current_time.tv_sec - game->last_move_time.tv_sec) * 1000 +
            (current_time.tv_nsec - game->last_move_time.tv_nsec) / 1000000;
        if (move_interval >= game->game_info.speed) {
          game->game_state = Rotate;
        }
        break;

      case GameOver:
        // Очищаем поле и тело змейки при завершении игры
        game->clearGameField();
        game->snake_body.clear();
        break;

      default:
        break;
    }
  }

  // Обновляем положение фигур на игровом поле
  game->updateFiguresOnField();

  return game->game_info;
}

}  // namespace s21
