#include "bricks_cli.h"

int main(void) {
  init_ncurses();
  int key = getch();
  while (key != 'q' && key != 'Q') {
    erase();
    mvprintw(1, 1, "==================================");
    mvprintw(2, 5, "WELCOME TO THE BRICK'S GAME");
    mvprintw(3, 1, "==================================");

    mvprintw(5, 5, "Press 't' to play TETRIS");
    mvprintw(6, 5, "Press 's' to play SNAKE");
    mvprintw(7, 5, "Press 'r' to play RACE");  // Добавляем новый пункт меню
    mvprintw(8, 5, "Press 'q' to EXIT");

    if (key == 't' || key == 'T') {
      clear();
      runTetrisGame();
    } else if (key == 's' || key == 'S') {
      clear();
      s21::GameLogic model{};
      s21::Controller controller{&model};
      s21::View view{&controller};
      view.runSnakeGame();
    } else if (key == 'r' || key == 'R') {  // Обработка нажатия 'r'
      clear();
      runRaceGame();  // Вызов функции для запуска игры гонки
    }

    key = getch();
  }
  end_ncurses();
  return 0;
}

void init_ncurses(void) {
  initscr();
  cbreak();
  noecho();
  keypad(stdscr, true);
  timeout(50);
  curs_set(0);
}

void end_ncurses(void) { endwin(); }

// Новая функция для запуска Python-скрипта
void runRaceGame(void) {
  end_ncurses();  // Завершение ncurses-сессии перед запуском скрипта
  system("python3 gui/cli/brick_game_cli.py");  // Вызов Python-скрипта
  init_ncurses();  // Повторная инициализация ncurses после завершения скрипта
}
