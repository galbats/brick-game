export class Tile {
    constructor() {
        this.$el = document.createElement('div');
        this.$el.classList.add('tile');
    }

    setActive(isActive, isPlayer = false) {
        if (isActive) {
            if (isPlayer) {
                console.log("Applying player class"); // Лог для проверки
                this.$el.classList.add('player');  // Добавляем класс player
            } else {
                this.$el.classList.add('active');
            }
        } else {
            this.$el.classList.remove('active');
            this.$el.classList.remove('player');  // Убираем класс player
        }
    }
}
