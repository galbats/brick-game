# BrickGame

### Описание

В данном проекте представлена реализация игр «Тетрис», «Змейка» и «Гонки» на языках программирования C, C++, Python.

### Архитектура проекта

```
src
├── Makefile
├── brick_game
│   ├── bricks.h
│   ├── race
│   │   └── game_logic.py
│   ├── server
│   │   ├── api
│   │   │   └── game_routes.py
│   │   └── app.py
│   ├── snake
│   │   ├── controller_snake.cpp
│   │   ├── controller_snake.h
│   │   ├── model_snake.cpp
│   │   └── model_snake.h
│   ├── tetris
│   │   ├── tetris.c
│   │   └── tetris.h
│   └── web_gui
│       ├── images/
│       ├── index.html
│       ├── main.js
│       ├── race.html
│       ├── race.js
│       ├── readme.md
│       ├── src/
│       └── styles.css
├── doc
│   ├── documentation.md
│   ├── raceFSM.png
│   ├── snakeFSM.jpg
│   └── tetrisFSM.jpg
├── gui
│   ├── cli
│   │   ├── brick_game_cli.py
│   │   ├── bricks_cli.cpp
│   │   ├── bricks_cli.h
│   │   ├── snake_cli.cpp
│   │   ├── snake_cli.h
│   │   ├── tetris_cli.c
│   │   └── tetris_cli.h
│   └── desktop/
└── tests
    └── test_game_logic.py
```

### Сборка и запуск

Сборка происходит с помощью Makefile, основные цели: 
- make: компиляция консольной и десктоп версии
- make cli: компиляция консольной версии (Tetris, Snake, Race)
- make desktop: компиляция десктоп версии (Tetris, Snake)
- make web-gui: запуск веб-сервера (для Race)
- make install: компиляция консольной и десктоп версии
- make uninstall: удаление скомпилированной консольной и дескотоп версии
- make dvi: открытие докуменации
- make dist: создает сжатый tar-архив текущей сборки проекта
- make clean: удаление всех файлов, создаваемых в процессе компиляции предыдущих целей
- make tests: запуск юнит-тестов


Для запуска игры, необходимо открыть файл:
- BrickGame_v2_cli для консольной версии
- BrickGame_v2_desktop для десктоп версии
- Открыть в браузере после запуска сервера (make web-gui): http://127.0.0.1:8080