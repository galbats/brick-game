#include "tetris_cli.h"

void runTetrisGame(void) {
  initializeColors();  // Инициализация цветовых пар

  WINDOW *field =
      createWindow(22, 22, 0, 0);  // Создание окна для игрового поля
  WINDOW *current_game = createWindow(
      12, 20, 0, 24);  // Создание окна для информации о текущей игре

  initCurrentGameInfo();  // Инициализация текущей информации о игре

  gameLoop(field, current_game);  // Запуск игрового цикла

  cleanupGame(field, current_game);
}

void initializeColors() {
  start_color();  // Инициализация цветового режима
  init_pair(1, COLOR_GREEN, COLOR_GREEN);
  init_pair(2, COLOR_BLACK, COLOR_BLACK);
}

WINDOW *createWindow(int height, int width, int starty, int startx) {
  return newwin(height, width, starty, startx);
}

// Обработка ввода пользователя
void processUserInput() {
  int c = getch();
  UserAction_t user_act = getAction(c);
  bool hold = (user_act == Down) ? true : false;
  userInput(user_act, hold);
}

// Игровой цикл, продолжающийся до завершения игры
void gameLoop(WINDOW *field, WINDOW *current_game) {
  while (*initGameState() != GameOver) {
    processUserInput();  // Обработка ввода пользователя
    GameInfo_t display = updateCurrentState();  // Обновление состояния игры
    renderGame(field, current_game, display);  // Отображение игры на экране
  }
}

// Освобождение игровых ресурсов
void cleanupGame(WINDOW *field, WINDOW *current_game) {
  freeGameResources();
  delwin(field);
  delwin(current_game);
}

// Отображение всей игры
void renderGame(WINDOW *field, WINDOW *current_game, GameInfo_t game) {
  renderField(field, game);
  renderNextPiece(current_game, game);
  renderGameInfo(current_game, game);
}
// Отображение игрового поля
void renderField(WINDOW *field, GameInfo_t game) {
  werase(field);     // Очистка окна поля
  box(field, 0, 0);  // Отрисовка рамки вокруг поля

  int **position = game.field;
  if (!position) {
    wrefresh(field);  // Обновление окна поля, если нет позиции
    return;
  }

  // Заполнение поля фигурами
  for (int i = 0; i < 20; i++) {
    for (int j = 0; j < 10; j++) {
      if (position[i][j]) {
        wattron(field, COLOR_PAIR(1));
        // Отрисовка фигуры
        mvwaddch(field, i + 1, j * 2 + 1, ' ');
        mvwaddch(field, i + 1, j * 2 + 2, ' ');
        wattroff(field, COLOR_PAIR(1));
      }
    }
  }

  wrefresh(field);  // Обновление окна поля
}

// Отображение следующей фигуры
void renderNextPiece(WINDOW *current_game, GameInfo_t game) {
  werase(current_game);     // Очистка окна
  box(current_game, 0, 0);  // Отрисовка рамки

  int **next = game.next;
  if (!next) {
    wrefresh(current_game);  // Обновление окна, если нет следующей фигуры
    return;
  }

  int size = initCurrInfo()->next_piece->size;
  // Заполнение окна следующей фигурой
  for (int i = 0; i < size; i++) {
    for (int j = 0; j < size; j++) {
      if (next[i][j]) {
        wattron(current_game, COLOR_PAIR(1));
        mvwaddch(current_game, i + 3, j * 2 + 5, ' ');
        mvwaddch(current_game, i + 3, j * 2 + 6, ' ');
        wattroff(current_game, COLOR_PAIR(1));
      }
    }
  }

  wrefresh(current_game);  // Обновление окна
}

// Отображение информации о текущей игре
void renderGameInfo(WINDOW *current_game, GameInfo_t game) {
  wattron(current_game, COLOR_PAIR(2));
  mvwprintw(current_game, 1, 2, "Next piece");
  mvwprintw(current_game, 6, 2, "Score %d", game.score);
  mvwprintw(current_game, 7, 2, "Level %d", game.level);
  mvwprintw(current_game, 8, 2, "High score %d", game.high_score);
  wattroff(current_game, COLOR_PAIR(2));

  if (game.pause) {
    mvwprintw(current_game, 10, 2, "PAUSE");
  }
  wrefresh(current_game);
}

// Получение действия пользователя на основе ввода
UserAction_t getAction(int user_input) {
  initGameState();
  UserAction_t action = -1;
  switch (user_input) {
    case '\n':
      action = Start;
      break;
    case 'p':
    case 'P':
      action = Pause;
      break;
    case 'q':
    case 'Q':
      action = Terminate;
      break;
    case KEY_LEFT:
      action = Left;
      break;
    case KEY_RIGHT:
      action = Right;
      break;
    case KEY_UP:
      action = Up;
      break;
    case KEY_DOWN:
      action = Down;
      break;
    case 'r':
    case 'R':
      action = Action;
      break;
    default:
      break;
  }
  return action;
}
