#include "tetris.h"

GameInfo_t updateCurrentState(void) {  // Cостояние игры
  // Инициализация текущей информации о игре и состояния игры
  CurrentGameInfo *game = initCurrInfo();
  GameState_t *state = initGameState();

  if (game->current_game.pause || !game->current_game.start) {
    return createGameInfo(game);  // Возврат текущего состояния игры
  }

  struct timespec cur_time = {0};  // Переменная для хранения текущего времени
  double drop_interval = 0;  // Переменная для хранения разницы во времени

  // Обработка текущего состояния игры
  switch (*state) {
    case GameStart:
      *state = Spawn;
      break;
    case Spawn:
      spawnNewPiece(game);
      *state = Moving;
      break;
    case Moving:
      clock_gettime(CLOCK_REALTIME, &cur_time);  // Получение текущего времени
      // Вычисление разницы во времени в миллисекундах
      drop_interval =
          (cur_time.tv_sec - game->drop_interval.tv_sec) * 1000 +
          (cur_time.tv_nsec - game->drop_interval.tv_nsec) / 1000000;
      // Проверка, прошло ли время, достаточное для перемещения фигуры
      if (drop_interval >= game->current_game.speed) {
        clock_gettime(CLOCK_REALTIME,
                      &game->drop_interval);  // Обновление времени последнего
                                              // перемещения
        *state = Rotate;
      }
      break;
    case Rotate:
      // Попытка переместить фигуру вниз, если не удалось, переход к Fixing
      if (!movePieceDown(game)) {
        *state = Fixing;
      } else {
        *state = Moving;
      }
      break;
    case Fixing:
      // Проверка на окончание игры
      if (isGameOver(game)) {
        addHighScore(game->current_game.high_score);
        initCurrentGameInfo();  // Инициализация новой игры
        *state = GameStart;
      } else {
        fixPiece(game);
        updateCurrentGameInfo(game);
        *state = Spawn;
      }
      break;
    default:
      break;
  }

  return createGameInfo(game);  // Возврат текущего состояния игры
}

void userInput(UserAction_t action, bool hold) {
  CurrentGameInfo *game =
      initCurrInfo();  // Инициализация текущей информации о игре
  GameState_t *state = initGameState();  // Инициализация состояния игры
  bool pause = game->current_game.pause;  // Получение текущего состояния паузы

  if (!hold) {
    switch (action) {
      case GameStart:
        if (*state == GameStart) game->current_game.start = 1;  // Запуск игры
        break;
      case Pause:
        if (*state != GameStart)
          game->current_game.pause =
              !game->current_game.pause;  // Переключение паузы
        break;
      case Terminate:
        addHighScore(game->current_game
                         .high_score);  // Добавление очков в таблицу рекордов
        *state = GameOver;
        break;
      case Left:
        if (*state == Moving && !pause) movePieceLeft(game);
        break;
      case Right:
        if (*state == Moving && !pause) movePieceRight(game);
        break;
      case Up:
        break;
      case Down:
        break;
      case Action:
        if (*state == Moving && !pause) rotatePiece(game);
        break;
      default:
        break;
    }
  } else {
    if (*state == Moving && !pause) {
      while (movePieceDown(game)) {  // Перемещение фигуры вниз до упора
        movePieceDown(game);
      }
    }
  }
}

Tetromino *generateNextPiece(void) {
  Tetromino *piece = (Tetromino *)calloc(1, sizeof(Tetromino));
  srand(clock() + rand());
  rand();  // Пропуск первого значения rand() для улучшения случайности
  int tetromino_type = 1 + rand() % 7;
  piece->size = getSize(tetromino_type);
  piece->position = getPiece(tetromino_type, piece->size);
  piece->x = 5 - piece->size / 2;  // Установка начальной координаты x фигуры
  piece->y = -1;  // Установка начальной координаты y фигуры
  return piece;
}

// Освобождение памяти, занятой фигурой
void freePiece(Tetromino *piece) {
  freeField(piece->position,
            piece->size);  // Освобождение памяти, занятой положением фигуры
  free(piece);  // Освобождение памяти, занятой самой фигурой
}

bool movePieceLeft(CurrentGameInfo *game) {
  bool flag = true;
  Tetromino *piece = game->current_piece;
  piece->x -= 1;
  if (canMove(*game)) {
    flag = false;
    piece->x += 1;
  }
  return flag;
}

bool movePieceRight(CurrentGameInfo *game) {
  bool flag = true;
  Tetromino *piece = game->current_piece;
  piece->x += 1;
  if (canMove(*game)) {
    flag = false;
    piece->x -= 1;
  }
  return flag;
}

bool movePieceDown(CurrentGameInfo *game) {
  bool flag = true;
  Tetromino *piece = game->current_piece;
  piece->y += 1;
  if (canMove(*game)) {
    piece->y -= 1;
    flag = false;
  }
  return flag;
}

void rotatePiece(CurrentGameInfo *game) {
  Tetromino *piece = game->current_piece;
  int size = piece->size;
  int **position = piece->position;
  int **new_tetro_matrix = initField(
      size, size);  // Инициализация нового массива для повернутой фигуры

  // Поворот фигуры на 90 градусов по часовой стрелке
  for (int i = 0; i < size; i++) {
    for (int j = 0; j < size; j++) {
      new_tetro_matrix[j][size - i - 1] = position[i][j];
    }
  }

  piece->position = new_tetro_matrix;

  // Проверка, можно ли разместить повернутую фигуру в текущей позиции
  if (canMove(*game)) {
    piece->position = position;
    freeField(new_tetro_matrix, size);
  } else {
    freeField(position, size);
  }
}

// Фиксация текущей фигуры на игровом поле
void fixPiece(CurrentGameInfo *info) {
  int **field = info->field;
  Tetromino *piece = info->current_piece;
  int x = piece->x;
  int y = piece->y;

  // Копирование фигуры на игровое поле
  for (int i = 0; i < piece->size; i++) {
    for (int j = 0; j < piece->size; j++) {
      if (piece->position[i][j]) {
        field[y + i][x + j] = 1;
      }
    }
  }
}

// Появление новой фигуры
void spawnNewPiece(CurrentGameInfo *game) {
  if (game->current_piece != NULL) {
    freePiece(game->current_piece);  // Освобождение памяти текущей фигуры
  }
  if (game->next_piece == NULL) {
    game->next_piece = generateNextPiece();  // Генерация следующей фигуры
  }
  game->current_piece = game->next_piece;
  game->next_piece = generateNextPiece();  // Подготовка новой следующей фигуры
}

// Получение размера фигуры на основе ее типа
int getSize(int tetromino_type) {
  int size = 3;
  if (tetromino_type == 1) {
    size = 5;
  } else if (tetromino_type == 2) {
    size = 2;
  }
  return size;
}

// Получение матрицы фигуры на основе её типа и размера
int **getPiece(int tetromino_type, int size) {
  int **tetro_matrix =
      initField(size, size);  // Инициализация пустой матрицы фигуры
  switch (tetromino_type) {
    case 1:  // Линия
      for (int j = 1; j < size; j++) tetro_matrix[1][j] = 1;
      break;
    case 2:  // Квадрат
      for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++) tetro_matrix[i][j] = 1;
      break;
    case 3:  // L-образная фигура
      for (int j = 0; j < size; j++) tetro_matrix[1][j] = 1;
      tetro_matrix[0][0] = 1;
      break;
    case 4:  // J-образная фигура
      for (int j = 0; j < size; j++) tetro_matrix[1][j] = 1;
      tetro_matrix[0][2] = 1;
      break;
    case 5:  // Z-образная фигура
      for (int j = 0; j < size - 1; j++) {
        tetro_matrix[0][j + 1] = 1;
        tetro_matrix[1][j] = 1;
      }
      break;
    case 6:  // S-образная фигура
      for (int j = 0; j < size - 1; j++) {
        tetro_matrix[0][j] = 1;
        tetro_matrix[1][j + 1] = 1;
      }
      break;
    case 7:  // T-образная фигура
      for (int j = 0; j < size; j++) tetro_matrix[1][j] = 1;
      tetro_matrix[0][1] = 1;
      break;
  }
  return tetro_matrix;  // Возврат матрицы фигуры
}

// Инициализация игрового поля заданного размера
int **initField(int rows, int cols) {
  int **field =
      (int **)calloc(rows, sizeof(int *));  // Выделение памяти для строк
  if (field) {
    for (int i = 0; i < rows; i++) {
      field[i] = (int *)calloc(
          cols, sizeof(int));  // Выделение памяти для каждого столбца
    }
  }
  return field;  // Возврат указателя на поле
}

// Освобождение памяти, занятой игровым полем
void freeField(int **field, int rows) {
  for (int i = 0; i < rows; i++)
    free(field[i]);  // Освобождение памяти для каждой строки
  free(field);  // Освобождение памяти для массива строк
}

// Инициализация состояния игры
GameState_t *initGameState(void) {
  static GameState_t state = 0;
  return &state;  // Возврат указателя на состояние игры
}

// Инициализация текущей информации о игре
CurrentGameInfo *initCurrInfo(void) {
  static CurrentGameInfo game = {NULL, NULL, NULL, {0}, {0}};
  return &game;  // Возврат указателя на текущую информацию о игре
}

// Инициализация информации о состоянии игры
GameInfo_t *initGameInfo(void) {
  static GameInfo_t game_info = {NULL, NULL, 0, 0, 0, 0, 0};
  return &game_info;  // Возврат указателя на информацию о состоянии игры
}

// Создание информации о текущем состоянии игры
GameInfo_t createGameInfo(CurrentGameInfo *game) {
  GameInfo_t *game_info_state = initGameInfo();
  GameState_t *state = initGameState();

  // Освобождение старого поля, если оно существует
  if (game_info_state->field != NULL) freeField(game_info_state->field, 20);
  game_info_state->field = initField(20, 10);  // Инициализация нового поля
  int **new_field = game_info_state->field;
  int **current_field = game->field;
  Tetromino *piece = game->current_piece;

  // Обновление поля с текущей фигурой, если она существует
  if (piece && *state != GameStart && *state != Spawn) {
    int **position = piece->position;
    int x = piece->x;
    int y = piece->y;
    for (int i = 0; i < 20; i++) {
      int size = piece->size;
      for (int j = 0; j < 10; j++) {
        bool flag = 0;
        // Проверка позиции фигуры на поле
        if (i >= y && j >= x && (i - y < size) && (j - x < size)) {
          if (position[i - y][j - x]) {
            flag = 1;
          } else {
            flag = (current_field[i][j]) ? 1 : 0;
          }
        } else {
          flag = (current_field[i][j]) ? 1 : 0;
        }
        if (flag) {
          new_field[i][j] = 1;  // Обновление нового поля
        }
      }
    }
  } else if (current_field) {
    // Копирование текущего поля
    for (int i = 0; i < 20; i++) {
      for (int j = 0; j < 10; j++) {
        if (current_field[i][j]) {
          new_field[i][j] = 1;
        }
      }
    }
  }

  // Обновление информации о следующей фигуре
  if (game->next_piece)
    game_info_state->next = game->next_piece->position;
  else
    game_info_state->next = NULL;

  // Копирование остальных параметров состояния игры
  game_info_state->high_score = game->current_game.high_score;
  game_info_state->level = game->current_game.level;
  game_info_state->pause = game->current_game.pause;
  game_info_state->score = game->current_game.score;
  game_info_state->speed = game->current_game.speed;

  return *game_info_state;  // Возврат текущего состояния игры
}

// Чтение из файла рекорда
int takeHighScore(void) {
  int high_score = 0;
  FILE *f = fopen("./tetris_top.txt", "r");
  if (f != NULL) {
    if (fscanf(f, "%d", &high_score)) {
      fclose(f);
    }
  }
  return high_score;
}

// Запись в файл рекорда
void addHighScore(int high_score) {
  FILE *f = fopen("./tetris_top.txt", "w");
  if (f != NULL) {
    fprintf(f, "%d", high_score);
    fclose(f);
  }
}

// Подсчет очков
int countScores(int **field) {
  int count_full_rows = 0;
  int rows = 19;
  while (rows > 0) {
    bool flag = true;
    for (int j = 0; j < 10; j++) {
      if (!field[rows][j]) {
        flag = false;
        break;
      }
    }

    if (flag) {
      count_full_rows++;
      removeFullRow(field, rows);
    } else {
      rows--;
    }
  }
  int score = 0;
  switch (count_full_rows) {
    case 1:
      score = 100;
      break;
    case 2:
      score = 300;
      break;
    case 3:
      score = 700;
      break;
    case 4:
      score = 1500;
      break;
    default:
      break;
  }
  return score;
}

// Проверка на окончание игры
bool isGameOver(CurrentGameInfo *game) {
  bool flag = false;
  Tetromino *piece = game->current_piece;
  int **position = piece->position;
  int size = piece->size;
  int y = piece->y;
  if (y <= 0) {
    for (int i = 0; i < size; i++) {
      if (position[-y][i]) {
        flag = true;
        break;
      }
    }
  }
  return flag;
}

// Удаление заполненных строк
void removeFullRow(int **field, int row) {
  for (int i = row; i > 0; i--) {
    for (int j = 0; j < 10; j++) {
      field[i][j] = field[i - 1][j];
    }
  }
}

// Инициализация текущей игры
void initCurrentGameInfo(void) {
  CurrentGameInfo *game =
      initCurrInfo();  // Получение указателя на текущую игру

  // Освобождение памяти текущего игрового поля, если оно существует
  if (game->field) freeField(game->field, 20);
  game->field = initField(20, 10);

  // Установка текущего времени для интервала падения
  clock_gettime(CLOCK_REALTIME, &game->drop_interval);

  // Инициализация текущих игровых параметров
  game->current_game.high_score = takeHighScore();
  game->current_game.level = 0;
  game->current_game.pause = 0;
  game->current_game.score = 0;
  game->current_game.start = 0;
  game->current_game.speed = 650 - 40 * game->current_game.level;
}

// Обновление текущих игровых параметров
void updateCurrentGameInfo(CurrentGameInfo *game) {
  game->current_game.score += countScores(game->field);
  if (game->current_game.high_score < game->current_game.score) {
    game->current_game.high_score = game->current_game.score;
  }
  if (game->current_game.level < 10) {
    game->current_game.level = game->current_game.score / 600;
    if (game->current_game.level > 10) {
      game->current_game.level = 10;
    }
  }
  game->current_game.speed = 650 - 40 * game->current_game.level;
}

// Освобождение ресурсов
void freeGameResources(void) {
  CurrentGameInfo *game = initCurrInfo();
  GameInfo_t *game_info_state = initGameInfo();
  if (game_info_state->field) {
    freeField(game_info_state->field, 20);
    game_info_state->field = NULL;
  }
  if (game->field) {
    freeField(game->field, 20);
    game->field = NULL;
  }
  if (game->current_piece) {
    freePiece(game->current_piece);
    game->current_piece = NULL;
  }
  if (game->next_piece) {
    freePiece(game->next_piece);
    game->next_piece = NULL;
  }
  *initGameState() = GameStart;
}

// Проверка возможности перемещения
bool canMove(CurrentGameInfo game) {
  int **field = game.field;  // Получение текущего состояния игрового поля
  int **piece =
      game.current_piece->position;  // Получение текущего положения фигуры
  int x = game.current_piece->x;
  int y = game.current_piece->y;
  int size = game.current_piece->size;
  bool flag = false;  // Флаг для проверки возможности перемещения

  // Цикл для проверки каждой клетки фигуры
  for (int i = 0; i < size; i++) {
    for (int j = 0; j < size; j++) {
      if (flag) break;  // Если флаг установлен, прервать дальнейшую проверку

      int piece_cell = piece[i][j];

      // Проверка, находится ли текущая клетка фигуры в пределах игрового поля
      // по вертикали
      if (y + i >= 0) {
        // Проверка, находится ли текущая клетка фигуры в пределах игрового поля
        // по горизонтали и вертикали
        if (x + j >= 0 && x + j < 10 && y + i < 20) {
          int field_cell =
              field[i + y][j + x];  // Значение текущей клетки игрового поля
          // Если обе клетки заняты, установка флага в true
          if (field_cell == 1 && piece_cell == 1) {
            flag = true;
            break;
          }
        } else {
          // Если клетка фигуры выходит за границы игрового поля, установка
          // флага в true
          if (piece_cell == 1) {
            flag = true;
            break;
          }
        }
      } else if (x + j < 0 || x + j >= 10) {
        // Если клетка фигуры выходит за границы игрового поля по горизонтали,
        // установка флага в true
        flag = true;
        break;
      }
    }
  }

  return flag;
}
