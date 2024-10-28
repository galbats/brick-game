import { Tile } from './tile.js';

export class GameBoard {
    constructor($root) {
        this.$root = $root;
        this.tiles = [];
        this.createBoard();
    }

    createBoard() {
        const totalTiles = 10 * 20; // 10x20 board

        for (let i = 0; i < totalTiles; i++) {
            const tile = new Tile();
            this.tiles.push(tile);
            this.$root.appendChild(tile.$el);
        }
    }

    enableTile(row, col, isPlayer = false) {
        const index = row * 10 + col;

        if (isPlayer) {
            console.log(`Setting player tile at [${row}, ${col}] with index ${index}`); // Лог для проверки
            this.tiles[index].setActive(true, true); // Присваиваем класс player
        } else {
            this.tiles[index].setActive(true, false);
        }
    }



    disableTile(row, col) {
        const index = row * 10 + col;
        this.tiles[index].setActive(false);
    }

    clear() {
        this.tiles.forEach(tile => tile.setActive(false));
    }
}
