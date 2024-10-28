import { applyRootStyles } from './src/utils.js';
import { GameBoard } from './src/game-board.js';
import { rootStyles, keyCodes } from './src/config.js';

applyRootStyles(rootStyles);
const gameBoard = new GameBoard(document.querySelector('#game-board'));

let gameLoopInterval;
const baseInterval = 300; // Уменьшенная базовая частота обновления игрового цикла

document.getElementById('start-btn').addEventListener('click', () => {
    console.log("Start button clicked");

    if (gameLoopInterval) {
        clearInterval(gameLoopInterval);
    }

    fetch('/api/games/1/start', { method: 'POST' })
        .then(response => {
            if (!response.ok) {
                throw new Error("Failed to start the game");
            }
            return response.json();
        })
        .then(data => {
            console.log("Response from server:", data);
            startGameLoop();
        })
        .catch(error => console.error("Error in start game request:", error));
});

document.getElementById('pause-btn').addEventListener('click', () => {
    console.log("Pause button clicked");
    fetch('/api/games/1/pause', { method: 'POST' })
        .then(response => response.json())
        .then(data => {
            console.log("Response from server:", data);

            if (data.pause) {
                stopGameLoop();  // Останавливаем игровой цикл при паузе
            } else {
                startGameLoop();  // Возобновляем игровой цикл, если игра продолжается
            }
        })
        .catch(error => console.error("Error in pause game request:", error));
});

document.getElementById('exit-btn').addEventListener('click', () => {
    console.log("Exit button clicked");

    fetch('/api/games/1/terminate', { method: 'POST' })
        .then(response => response.json())
        .then(data => {
            console.log("Response from server:", data);

            // После успешного завершения игры, перенаправляем на главную страницу
            window.location.href = '/index.html';
        })
        .catch(error => console.error("Error in exit game request:", error));
});

document.addEventListener('keydown', function (event) {
    let action = null;
    if (keyCodes.up.includes(event.code)) action = 'Up';
    if (keyCodes.right.includes(event.code)) action = 'Right';
    if (keyCodes.left.includes(event.code)) action = 'Left';
    if (event.code === 'Space') action = 'Action';

    if (action) {
        fetch('/api/games/1/action', {
            method: 'POST',
            headers: { 'Content-Type': 'application/json' },
            body: JSON.stringify({ action: action })
        }).then(updateGameField);
        console.log(action.toLowerCase());
    }
});

function startGameLoop() {
    console.log("Game loop started");

    if (gameLoopInterval) {
        clearInterval(gameLoopInterval);
    }

    gameLoopInterval = setInterval(() => {
        fetch('/api/games/1/action', {
            method: 'POST',
            headers: { 'Content-Type': 'application/json' },
            body: JSON.stringify({ action: 'Up' })
        }).then(updateGameField);
    }, baseInterval); // Установили частоту на 100 мс
}

function stopGameLoop() {
    clearInterval(gameLoopInterval);
    gameLoopInterval = null;
}

function updateGameField() {
    fetch('/api/games/1/state')
        .then(response => response.json())
        .then(data => {
            console.log("Received game state:", data);

            gameBoard.clear();

            data.field.forEach((row, rowIndex) => {
                row.forEach((cell, colIndex) => {
                    if (cell) {
                        console.log(`Activating tile at [${rowIndex}, ${colIndex}]`);
                        gameBoard.enableTile(rowIndex, colIndex, false);
                    }
                });
            });

            const playerPos = data.player_position;
            if (playerPos) {
                console.log(`Player position: [${playerPos[0]}, ${playerPos[1]}]`);
                gameBoard.enableTile(playerPos[0], playerPos[1], true);
            } else {
                console.error("Player position is missing in the game state");
            }

            document.getElementById('score').textContent = `Score: ${data.score}`;
            document.getElementById('high-score').textContent = `High Score: ${data.high_score}`;
            document.getElementById('level').textContent = `Level: ${data.level}`;

            if (gameLoopInterval) {
                clearInterval(gameLoopInterval);
            }
            gameLoopInterval = setInterval(() => {
                fetch('/api/games/1/action', {
                    method: 'POST',
                    headers: { 'Content-Type': 'application/json' },
                    body: JSON.stringify({ action: 'Up' })
                }).then(updateGameField);
            }, baseInterval / data.speed); // Увеличиваем частоту на основе скорости
        })
        .catch(error => console.error("Error fetching game state:", error));
}

updateGameField();
