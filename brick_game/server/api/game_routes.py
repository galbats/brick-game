from flask import Blueprint, jsonify, request
from ctypes import CDLL, POINTER, Structure, c_int, c_bool, c_void_p, byref
import os
import sys

# Добавляем корневую директорию проекта в sys.path
sys.path.append(os.path.abspath(os.path.join(os.path.dirname(__file__), '../../..')))

from brick_game.race.game_logic import BrickGame as RaceGame, Action as RaceAction
from ctypes import CDLL, POINTER, Structure, c_int, c_bool, c_void_p, byref

game_bp = Blueprint('game_bp', __name__)

# Список доступных игр
games = {
    1: {"id": 1, "name": "Race"},
    2: {"id": 2, "name": "Tetris"},
}

current_game = None

# Определение структуры для GameInfo_t (используется в Тетрисе)
class GameInfo_t(Structure):
    _fields_ = [
        ("field", POINTER(POINTER(c_int))),
        ("next", POINTER(POINTER(c_int))),
        ("score", c_int),
        ("high_score", c_int),
        ("level", c_int),
        ("speed", c_int),
        ("pause", c_bool)
    ]

@game_bp.route('/games', methods=['GET'])
def get_games():
    return jsonify({"games": list(games.values())}), 200

@game_bp.route('/games/<int:game_id>', methods=['POST'])
def choose_game(game_id):
    global current_game
    if game_id == 1:
        current_game = RaceGame()
    elif game_id == 2:
        lib_path = os.path.abspath(os.path.join(os.path.dirname(__file__), '../../../libtetris.so'))
        current_game = CDLL(lib_path)
        current_game.updateCurrentState.restype = GameInfo_t
    else:
        return jsonify({"message": "Game not found"}), 404

    return jsonify({"message": f"Game {games[game_id]['name']} started"}), 200

@game_bp.route('/games/<int:game_id>/action', methods=['POST'])
def perform_action(game_id):
    if current_game is None:
        return jsonify({"message": "No game selected"}), 400
    
    action = request.json.get('action')
    
    if game_id == 1:
        current_game.user_input(RaceAction[action.upper()])
    elif game_id == 2:
        # Соответствие действий для тетриса
        tetris_actions = {
            'Left': 0,      # Move left
            'Right': 1,     # Move right
            'Down': 2,      # Drop down
            'Up': 3,        # Rotate (typically Up is mapped to rotate)
            'Action': 3     # Action key for rotation as well
        }
        if action in tetris_actions:
            action_id = tetris_actions[action]
            current_game.userInput(c_int(action_id), c_bool(False))
        else:
            return jsonify({"message": "Invalid action"}), 400
    
    return jsonify({"message": f"{action} performed"}), 200

@game_bp.route('/games/<int:game_id>/start', methods=['POST'])
def start_game(game_id):
    global current_game
    if game_id == 1:
        current_game = RaceGame()
        current_game.user_input(RaceAction.START)
    elif game_id == 2:
        lib_path = os.path.abspath(os.path.join(os.path.dirname(__file__), '../../../libtetris.so'))
        current_game = CDLL(lib_path)
        current_game.updateCurrentState.restype = GameInfo_t
        current_game.userInput(c_int(1), c_bool(False))
        state = current_game.updateCurrentState()
        print(f"Game started. Initial state: Score={state.score}, Level={state.level}, High Score={state.high_score}")
    else:
        return jsonify({"message": "Game not found"}), 404
    
    return jsonify({"message": f"Game {games[game_id]['name']} started"}), 200

@game_bp.route('/games/<int:game_id>/state', methods=['GET'])
def get_game_state(game_id):
    if current_game is None:
        return jsonify({"message": "No game selected"}), 400
    
    try:
        if game_id == 1:
            state = current_game.update_current_state()
            return jsonify({
                "field": state.field,
                "next": state.next,
                "score": state.score,
                "high_score": state.high_score,
                "level": state.level,
                "speed": state.speed,
                "pause": state.pause,
                "player_position": state.player_position
            }), 200
        elif game_id == 2:
            state = current_game.updateCurrentState()

            # Проверяем корректность возвращенного состояния
            if not isinstance(state, GameInfo_t):
                print("Error: updateCurrentState did not return GameInfo_t")
                return jsonify({"message": "Failed to get game state"}), 500

            field = [[state.field[i][j] for j in range(10)] for i in range(20)]
            next_piece = [[state.next[i][j] for j in range(4)] for i in range(4)] if state.next else []
            return jsonify({
                "field": field,
                "next": next_piece,
                "score": state.score,
                "high_score": state.high_score,
                "level": state.level,
                "speed": state.speed,
                "pause": state.pause
            }), 200
    except Exception as e:
        print(f"Error in get_game_state: {e}")
        return jsonify({"message": "Internal server error"}), 500

@game_bp.route('/games/<int:game_id>/pause', methods=['POST'])
def pause_game(game_id):
    if current_game is None:
        return jsonify({"message": "No game selected"}), 400
    
    if game_id == 1:
        current_game.user_input(RaceAction.PAUSE)
    elif game_id == 2:
        current_game.userInput(c_int(2), c_bool(False))  # 2 - действие паузы
    
    return jsonify({"message": "Game paused"}), 200

@game_bp.route('/games/<int:game_id>/terminate', methods=['POST'])
def terminate_game(game_id):
    global current_game
    if isinstance(current_game, RaceGame):
        current_game.user_input(RaceAction.TERMINATE)
        current_game = None
        return jsonify({"message": "Game terminated"}), 200
    elif isinstance(current_game, CDLL):
        current_game.userInput(c_int(3), c_bool(False))  # 3 - действие завершения игры
        current_game.freeGameResources()  # Освобождаем ресурсы
        current_game = None
        return jsonify({"message": "Game terminated"}), 200
    else:
        return jsonify({"message": "No game selected"}), 400

