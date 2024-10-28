#include "model_snake.h"

namespace s21 {

// class Entities
Entities::Entities() : x{}, y{} {}
Entities::Entities(int x, int y) : x{x}, y{y} {}
Entities::Entities(const Entities &other) : x{other.x}, y{other.y} {}

Entities Entities::generateRandomApple(std::vector<Entities> &snake_body) {
  std::random_device random_device;
  std::mt19937 random_generator(random_device());
  std::uniform_int_distribution<> x_pos(0, WIDTH - 1);
  std::uniform_int_distribution<> y_pos(0, HEIGTH - 1);

  Entities new_apple;
  bool valid_position = false;

  while (!valid_position) {
    new_apple.x = x_pos(random_generator);
    new_apple.y = y_pos(random_generator);

    // Проверяем, что новое яблоко не пересекается с телом змейки
    valid_position = true;
    for (const auto &segment : snake_body) {
      if (segment.x == new_apple.x && segment.y == new_apple.y) {
        valid_position = false;
        break;
      }
    }
  }

  return new_apple;
}

Entities &Entities::operator=(const Entities &other) {
  if (this != &other) {
    x = other.x;
    y = other.y;
  }
  return *this;
}

// class GameLogic
GameLogic::GameLogic() {
  // Инициализация игрового поля
  game_info.field = new int *[HEIGTH];
  for (int i{}; i < HEIGTH; ++i) {
    game_info.field[i] = new int[WIDTH]{};
  }

  game_state = GameStart;  // Начальное состояние игры
  current_direction = ToDown;
  next_direction = ToDown;
  game_info.score = game_info.level = 0;
  game_info.high_score = getScore();
  game_info.pause = is_started = false;
  game_info.speed = 600;
  initializeSnake();  // Инициализация змейки
  clock_gettime(CLOCK_REALTIME,
                &last_move_time);  // Установка времени последнего движения
}

GameLogic::~GameLogic() {
  // Освобождение памяти, выделенной под каждую строку игрового поля
  for (int i = 0; i < HEIGTH; ++i) {
    delete[] game_info.field[i];
    game_info.field[i] =
        nullptr;  // Дополнительно обнуляем указатель для безопасности
  }

  // Освобождение памяти, выделенной под массив указателей на строки
  delete[] game_info.field;
  game_info.field =
      nullptr;  // Дополнительно обнуляем указатель для безопасности
}

int GameLogic::getScore() {
  int high_score = 0;
  FILE *file = fopen("./snake_top.txt", "r");
  if (file != nullptr) {
    if (fscanf(file, "%d", &high_score) != 1) {
      high_score = 0;  // В случае ошибки чтения, установим значение в 0
    }
    fclose(file);
  }
  return high_score;
}

void GameLogic::setScore(int high_score) {
  FILE *file = fopen("./snake_top.txt", "w");
  if (file != nullptr) {
    fprintf(file, "%d", high_score);
    fclose(file);
  }
}

void GameLogic::generateRandomApple() {
  Entities new_apple = unit.generateRandomApple(snake_body);
  unit.x = new_apple.x;
  unit.y = new_apple.y;
}

// Инициализация игровых данных
void GameLogic::initializeGameInfo(void) {
  clearGameField();
  snake_body.clear();
  initializeSnake();
  generateRandomApple();
  current_direction = ToDown;
  next_direction = ToDown;
  updateFiguresOnField();
};

// Начальная инициализация змейки
void GameLogic::initializeSnake() {
  snake_body.clear();
  snake_body.push_back(Entities(WIDTH / 2, HEIGTH / 2 + 1));
  snake_body.push_back(Entities(WIDTH / 2, HEIGTH / 2));
  snake_body.push_back(Entities(WIDTH / 2, HEIGTH / 2 - 1));
  snake_body.push_back(Entities(WIDTH / 2, HEIGTH / 2 - 2));
}

void GameLogic::clearGameField() {
  // Итерация по каждому элементу игрового поля и установка его в 0
  for (int i = 0; i < HEIGTH; ++i) {
    std::fill(game_info.field[i], game_info.field[i] + WIDTH, 0);
  }
}

void GameLogic::updateSnakeOnField() {
  // Итерация по каждому сегменту тела змейки и обновление игрового поля
  for (const auto &segment : snake_body) {
    game_info.field[segment.y][segment.x] = 1;
  }
}

void GameLogic::updateAppleOnField() {
  // Установка значения на игровом поле в позиции яблока
  game_info.field[unit.y][unit.x] = 2;
}

GameInfo_t GameLogic::updateFiguresOnField(void) {
  clearGameField();
  updateSnakeOnField();
  updateAppleOnField();
  return game_info;
}

bool isOutOfField(int x, int y) {
  // Проверка на выход за границы игрового поля
  bool out_of_field = (x < 0 || x >= WIDTH || y < 0 || y >= HEIGTH);
  return out_of_field;
}

bool isSnakeCollision(int x, int y, std::vector<Entities> &snake_body) {
  // Переменная для хранения результата проверки
  bool collision = false;

  // Проверка на столкновение с телом змейки
  for (auto it = snake_body.begin() + 1; it != snake_body.end(); ++it) {
    if (it->x == x && it->y == y) {
      collision = true;
      break;  // Если найдено совпадение, выходим из цикла
    }
  }

  return collision;
}

bool GameLogic::checkMove() {
  // Переменная для хранения текущей позиции головы змейки
  Entities head = snake_body.front();
  // Переменная для хранения результата проверки
  bool is_invalid_move = isOutOfField(head.x, head.y) ||
                         isSnakeCollision(head.x, head.y, snake_body);
  return is_invalid_move;
}

void GameLogic::refreshGameInfo() {
  // Увеличиваем текущий счет
  game_info.score++;

  // Обновляем рекордный счет, если текущий счет выше
  if (game_info.high_score < game_info.score) {
    game_info.high_score = game_info.score;
  }

  // Обновляем уровень игры, если он меньше 10
  if (game_info.level < 10) {
    game_info.level = game_info.score / 5;
  }

  // Обновляем скорость игры в зависимости от уровня
  game_info.speed = 600 - 50 * game_info.level;
}

// Обновляет позицию головы змейки в зависимости от направления
void updateHeadPosition(Entities &head, SnakeDir next_direction) {
  switch (next_direction) {
    case ToUp:
      head.y -= 1;
      break;
    case ToDown:
      head.y += 1;
      break;
    case ToRight:
      head.x += 1;
      break;
    case ToLeft:
      head.x -= 1;
      break;
    default:
      break;
  }
}

// Обрабатывает поедание яблока
void handleAppleEaten(GameLogic *game) {
  game->game_state = Spawn;
  game->refreshGameInfo();
}

// Обрабатывает столкновение
void handleCollision(GameLogic *game) {
  game->clearGameField();
  game->snake_body.clear();
  game->game_info.score = game->game_info.level = 0;
  game->game_info.speed = 600;
  game->setScore(game->game_info.high_score);
  game->game_state = GameStart;
  game->is_started = false;
}

void GameLogic::moveSnake() {
  if (game_info.pause || !is_started)
    return;  // Проверка на паузу и начало игры

  Entities new_head{snake_body.front()};  // Новая голова змейки

  updateHeadPosition(new_head, next_direction);  // Обновление позиции головы

  snake_body.insert(snake_body.begin(), new_head);  // Вставка новой головы

  if (new_head.x == unit.x && new_head.y == unit.y) {
    handleAppleEaten(this);  // Обработка поедания яблока
  } else if (checkMove()) {
    handleCollision(this);  // Обработка столкновения
  } else {
    snake_body.pop_back();  // Удаление хвоста змейки
    game_state = Moving;
  }

  current_direction = next_direction;  // Обновление текущего направления
  clock_gettime(CLOCK_REALTIME,
                &last_move_time);  // Обновление времени движения
  updateFiguresOnField();
}

}  // namespace s21
