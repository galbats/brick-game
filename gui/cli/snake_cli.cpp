#include "snake_cli.h"

namespace s21 {

View::View(Controller *c) : snake{c} {};
View::~View(){};

void View::runSnakeGame() {
  initializeColors();  // Инициализация цветов

  WINDOW *field =
      createWindow(22, 22, 0, 0);  // Создание окна для игрового поля
  WINDOW *player =
      createWindow(7, 18, 0, 24);  // Создание окна для информации игрока

  while (snake->game->game_state != s21::GameOver) {
    processInput();  // Обработка пользовательского ввода

    GameInfo_t display =
        snake->updateCurrentState();  // Обновление состояния игры

    renderGame(field, player,
               display);  // Отображение игрового поля и информации игрока
  }

  delwin(field);
  delwin(player);
}

void View::initializeColors() {
  start_color();
  init_pair(1, COLOR_GREEN, COLOR_GREEN);
  init_pair(2, COLOR_RED, COLOR_RED);
  init_pair(3, COLOR_BLACK, COLOR_BLACK);
}

WINDOW *View::createWindow(int height, int width, int starty, int startx) {
  return newwin(height, width, starty, startx);
}

void View::processInput() {
  timeout(50);
  int c = getch();
  bool hold = false;
  UserAction_t user_act = getAction(c);
  if (user_act == Action) hold = true;
  snake->userInput(user_act, hold);
}

UserAction_t View::getAction(int user_input) {
  UserAction_t action = None;

  switch (user_input) {
    case KEY_UP:
      action = Up;
      break;
    case KEY_LEFT:
      action = Left;
      break;
    case KEY_RIGHT:
      action = Right;
      break;
    case KEY_DOWN:
      action = Down;
      break;
    case 'r':
    case 'R':
      action = Action;
      break;
    case '\n':
      action = Start;
      break;
    case 'q':
    case 'Q':
      action = Terminate;
      break;
    case 'p':
    case 'P':
      action = Pause;
      break;
    default:
      break;
  }
  return action;
}

void View::renderGame(WINDOW *field, WINDOW *player, GameInfo_t game) {
  renderField(field, game);
  renderInfo(player, game);
}

void View::renderField(WINDOW *field, GameInfo_t game) {
  werase(field);
  box(field, 0, 0);
  int **coordinates = game.field;
  if (coordinates) {
    for (int i = 0; i < 20; i++) {
      for (int j = 0; j < 10; j++) {
        if (coordinates[i][j] == 1) {  // Если это тело змейки
          wattron(field, COLOR_PAIR(1));
          mvwaddch(field, i + 1, j * 2 + 1, ' ');
          mvwaddch(field, i + 1, j * 2 + 2, ' ');
          wattroff(field, COLOR_PAIR(1));
        } else if (coordinates[i][j] == 2) {  // Если это яблоко
          wattron(field, COLOR_PAIR(2));
          mvwaddch(field, i + 1, j * 2 + 1, ' ');
          mvwaddch(field, i + 1, j * 2 + 2, ' ');
          wattroff(field, COLOR_PAIR(2));
        }
      }
    }
  }
  wrefresh(field);
}

void View::renderInfo(WINDOW *info, GameInfo_t game) {
  werase(info);
  box(info, 0, 0);
  wattron(info, COLOR_PAIR(3));
  mvwprintw(info, 1, 1, "Level %d", game.level);
  mvwprintw(info, 2, 1, "Score %d", game.score);
  mvwprintw(info, 3, 1, "High score %d", game.high_score);
  wattroff(info, COLOR_PAIR(3));

  if (game.pause) mvwprintw(info, 5, 1, "PAUSE");
  wrefresh(info);
}

}  // namespace s21
