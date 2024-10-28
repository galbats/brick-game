import unittest
from brick_game.race.game_logic import BrickGame, Action

class TestBrickGame(unittest.TestCase):

    def setUp(self):
        """Создание нового экземпляра игры перед каждым тестом."""
        self.game = BrickGame()

    def test_initial_state(self):
        """Тест начального состояния игры."""
        state = self.game.update_current_state()
        self.assertFalse(self.game.is_running)
        self.assertEqual(state.score, 0)
        self.assertEqual(state.level, 1)
        self.assertEqual(state.speed, 1)
        self.assertEqual(state.player_position, [19, 4])

    def test_start_game(self):
        """Тест запуска игры."""
        self.game.user_input(Action.START)
        self.assertTrue(self.game.is_running)
        self.assertFalse(self.game.pause)
        state = self.game.update_current_state()
        self.assertEqual(state.score, 0)
        self.assertEqual(state.level, 1)
        self.assertEqual(state.speed, 1)

    def test_pause_game(self):
        """Тест паузы и возобновления игры."""
        self.game.user_input(Action.START)
        self.game.user_input(Action.PAUSE)
        self.assertTrue(self.game.pause)
        self.game.user_input(Action.PAUSE)
        self.assertFalse(self.game.pause)

    def test_terminate_game(self):
        """Тест завершения игры."""
        self.game.user_input(Action.START)
        self.game.user_input(Action.TERMINATE)
        self.assertFalse(self.game.is_running)
        state = self.game.update_current_state()
        self.assertEqual(state.score, 0)

    def test_move_left(self):
        """Тест перемещения игрока влево."""
        self.game.user_input(Action.START)
        initial_position = self.game.player_pos[1]
        self.game.user_input(Action.LEFT)
        self.assertEqual(self.game.player_pos[1], initial_position - 1)

    def test_move_right(self):
        """Тест перемещения игрока вправо."""
        self.game.user_input(Action.START)
        initial_position = self.game.player_pos[1]
        self.game.user_input(Action.RIGHT)
        self.assertEqual(self.game.player_pos[1], initial_position + 1)


    def test_level_up(self):
        """Тест повышения уровня и увеличения скорости."""
        self.game.user_input(Action.START)
        self.game.score = 19  # Устанавливаем очки на границе повышения уровня
        self.game.user_input(Action.UP)
        self.assertEqual(self.game.level, 2)
        self.assertEqual(self.game.speed, 2)  # Скорость увеличивается на втором уровне

if __name__ == '__main__':
    unittest.main()
