from flask import Flask, send_from_directory
import os
import sys

sys.path.append(os.path.abspath(os.path.join(os.path.dirname(__file__), '../..')))

from brick_game.server.api.game_routes import game_bp

app = Flask(__name__)
app.register_blueprint(game_bp, url_prefix='/api')

# Изменяем путь к папке web_gui
@app.route('/')
def serve_index():
    return send_from_directory(os.path.join(app.root_path, '..', 'web_gui'), 'index.html')

@app.route('/<path:path>')
def serve_static(path):
    return send_from_directory(os.path.join(app.root_path, '..', 'web_gui'), path)

# @app.route('/tetris.html')
# def tetris_page():
#     return send_from_directory('web_gui', 'tetris.html')

if __name__ == "__main__":
    app.run(host="0.0.0.0", port=8080, debug=True)
