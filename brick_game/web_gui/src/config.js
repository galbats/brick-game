export const GAME_BOARD_WIDTH = 10;
export const GAME_BOARD_HEIGHT = 20;

export const rootStyles = {
    '--tile-size': '20px',  // Размер плитки
    '--tile-color': '#eee',  // Цвет плитки
    '--tile-active-color': '#222',  // Цвет активной плитки
    '--game-board-width': GAME_BOARD_WIDTH,  // Ширина игрового поля
    '--game-board-height': GAME_BOARD_HEIGHT,  // Высота игрового поля
    '--game-board-gap': '2px',  // Промежуток между плитками
    '--game-board-background': '#333',  // Цвет фона игрового поля
};

export const keyCodes = {
    up: ['ArrowUp', 'KeyW', 'KeyI'],
    right: ['ArrowRight', 'KeyD', 'KeyL'],
    down: ['ArrowDown', 'KeyS', 'KeyK'],
    left: ['ArrowLeft', 'KeyA', 'KeyJ'],
};
