import sys
import time
import os
import curses
# Добавляем src в PYTHONPATH
sys.path.append(os.path.abspath(os.path.join(os.path.dirname(__file__), '../../')))

from brick_game.race.game_logic import BrickGame, Action

def print_instructions(stdscr):
    """Вывод инструкций по управлению игрой."""
    stdscr.addstr(0, 0, "Команды управления:")
    stdscr.addstr(1, 0, "  w - Вверх")
    stdscr.addstr(2, 0, "  a - Влево")
    stdscr.addstr(3, 0, "  s - Вниз")
    stdscr.addstr(4, 0, "  d - Вправо")
    stdscr.addstr(5, 0, "  p - Пауза")
    stdscr.addstr(6, 0, "  q - Выйти")
    stdscr.addstr(8, 0, "  [Space] - Начать игру")
    # stdscr.addstr(9, 0, "  [x] - Закончить игру")
    stdscr.refresh()

def display_game_state(stdscr, game):
    """Вывод состояния игры."""
    state = game.update_current_state()

    # Получаем размеры окна
    max_y, max_x = stdscr.getmaxyx()
    
    # Очищаем экран
    stdscr.clear()
    
    # Выводим основные данные
    stdscr.addstr(0, 0, f"Очки: {state.score}")
    stdscr.addstr(1, 0, f"Максимальные очки: {state.high_score}")
    stdscr.addstr(2, 0, f"Уровень: {state.level}")

    stdscr.addstr(4, 0, "Игровое поле:")

    # Проверка границ и вывод игрового поля
    field_height = len(state.field)
    field_width = len(state.field[0]) if field_height > 0 else 0

    for i, row in enumerate(state.field):
        if i + 5 < max_y:  # Проверка по высоте
            row_str = " ".join(["1" if cell else "." for cell in row])
            if len(row_str) < max_x:  # Проверка по ширине
                stdscr.addstr(5 + i, 0, row_str, curses.color_pair(1))
            else:
                stdscr.addstr(5 + i, 0, row_str[:max_x], curses.color_pair(1))
        else:
            break

    # Отображаем машинку игрока
    if state.player_position:
        player_y, player_x = state.player_position
        if 5 + player_y < max_y and 2 * player_x < max_x:  # Проверка по границам
            stdscr.addstr(5 + player_y, 2 * player_x, '0')  # Печатаем машинку как 'P'

    stdscr.refresh()

def main(stdscr):
    curses.curs_set(0)  # Скрыть курсор

    # Инициализация цветов
    curses.start_color()
    curses.init_pair(1, curses.COLOR_GREEN, curses.COLOR_BLACK) 

    game = BrickGame()
    game_running = False
    last_update_time = time.time()
    
    stdscr.nodelay(True)  # Устанавливаем неблокирующий режим для ввода
    stdscr.timeout(100)   # Устанавливаем таймаут для обновления экрана (в миллисекундах)
    
    print_instructions(stdscr)

    while True:
        key = stdscr.getch()

        if key == ord('w'):
            game.user_input(Action.UP)
        elif key == ord('a'):
            game.user_input(Action.LEFT)
        elif key == ord('s'):
            game.user_input(Action.DOWN)
        elif key == ord('d'):
            game.user_input(Action.RIGHT)
        elif key == ord('p'):
            game.user_input(Action.PAUSE)
        elif key == ord('q'):
            # stdscr.addstr(30, 0, "Выход из игры...")
            stdscr.refresh()
            time.sleep(1)
            break
        elif key == ord(' '):  # space to start the game
            if not game_running:
                game.user_input(Action.START)
                game_running = True
                last_update_time = time.time()  # Сброс таймера
        elif key == ord('x'):  # x to stop the game
            if game_running:
                game.user_input(Action.TERMINATE)
                game_running = False
        
        # Автоматическое обновление состояния игры
        if game_running and (time.time() - last_update_time) > (1 / game.speed):
            game.user_input(Action.UP)
            last_update_time = time.time()

        if game_running:
            display_game_state(stdscr, game)
            time.sleep(0.05)  # Управление частотой обновления экрана

if __name__ == "__main__":
    curses.wrapper(main)
