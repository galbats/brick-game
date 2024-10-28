from enum import Enum
from collections import namedtuple
from typing import List
import random
import json
import os

class Action(Enum):
    START = "Start"
    PAUSE = "Pause"
    TERMINATE = "Terminate"
    LEFT = "Left"
    RIGHT = "Right"
    UP = "Up"
    DOWN = "Down"
    ACTION = "Action"

State = namedtuple('State', ['field', 'next', 'score', 'high_score', 'level', 'speed', 'pause', 'player_position'])

class BrickGame:
    def __init__(self):
        self.field = [[False] * 10 for _ in range(20)]
        self.next = [[False] * 4 for _ in range(4)]
        self.score = 0
        self.high_score = self.load_high_score()
        self.level = 1
        self.speed = 1
        self.pause = False
        self.is_running = False
        self.player_pos = [19, 4]  # Начальная позиция машинки
        self.spawn_enemy()

    def load_high_score(self):
        """Загружаем максимальное количество очков из файла."""
        if os.path.exists('high_score.json'):
            with open('high_score.json', 'r') as f:
                return json.load(f).get('high_score', 0)
        return 0

    def save_high_score(self):
        """Сохраняем максимальное количество очков в файл."""
        with open('high_score.json', 'w') as f:
            json.dump({'high_score': self.high_score}, f)

    def spawn_enemy(self):
        """Генерация нового противника в верхней части поля."""
        col = random.randint(0, 9)
        self.field[0][col] = True

    def user_input(self, action: Action, hold: bool = False):
        if action == Action.START:
            if not self.is_running:  # Проверяем, если игра уже идет
                self.start_game()
            else:
                print("Game is already running")
        elif action == Action.PAUSE:
            if self.is_running:  
                self.pause_game()
        elif action == Action.TERMINATE:
            self.terminate_game()
        elif action == Action.LEFT:
            if not self.pause:  
                self.move_left(hold)
        elif action == Action.RIGHT:
            if not self.pause:  
                self.move_right(hold)
        elif action == Action.UP:
            if not self.pause:  
                self.move_down(hold)


    def start_game(self):
        print("Starting the game...")
        self.is_running = True
        self.pause = False
        self.score = 0
        self.level = 1
        self.speed = 1  # Сбрасываем скорость до начального значения
        self.field = [[False] * 10 for _ in range(20)]
        self.player_pos = [19, 4]
        self.spawn_enemy()

        # Логирование для проверки
        print(f"Game started: Level = {self.level}, Speed = {self.speed}")


    def pause_game(self):
        if self.is_running:
            self.pause = not self.pause
            print(f"Game {'paused' if self.pause else 'resumed'}.")

    
    def terminate_game(self):
        self.is_running = False
        if self.score > self.high_score:
            self.high_score = self.score
            self.save_high_score()
        print("Game terminated.")
        self.reset_game()

    def reset_game(self):
        """Сбрасываем состояние игры для новой игры."""
        self.field = [[False] * 10 for _ in range(20)]
        self.score = 0
        self.level = 1
        self.speed = 1
        self.player_pos = [19, 4]

    def move_left(self, hold: bool):
        if self.is_running and not self.pause and self.player_pos[1] > 0:
            self.player_pos[1] -= 1

    def move_right(self, hold: bool):
        if self.is_running and not self.pause and self.player_pos[1] < 9:
            self.player_pos[1] += 1

    def move_down(self, hold: bool):
        if self.is_running and not self.pause:
            self.update_game_field()

    def update_game_field(self):
        """Обновление игрового поля: перемещение объектов и проверка на столкновения."""
        new_field = [[False] * 10 for _ in range(20)]
        collision = False
        for i in range(1, 20):
            for j in range(10):
                if self.field[i - 1][j]:
                    if i == 19 and self.player_pos[0] == i and self.player_pos[1] == j:
                        collision = True
                    new_field[i][j] = True
        if collision:
            self.terminate_game()
        else:
            self.field = new_field
            self.spawn_enemy()
            self.score += 1
            if self.score % 20 == 0 and self.level < 10:
                self.level += 1
                self.speed = 1 + self.level // 2  # Увеличиваем скорость с каждым уровнем
                print(f"Level up! New level: {self.level}, Speed: {self.speed}")


    def update_current_state(self) -> State:
        # Логируем текущую позицию игрока
        print(f"Player position (server): {self.player_pos}")
        
        return State(
            field=self.field,
            next=self.next,
            score=self.score,
            high_score=self.high_score,
            level=self.level,
            speed=self.speed,
            pause=self.pause,
            player_position=self.player_pos  # Возвращаем позицию игрока
        )


