PROJECT_NAME=BrickGame_v2
CC=gcc
CPP=g++
CFLAGS=-Wall -Werror -Wextra
STANDART=-std=c++17

CLI=gui/cli
DESKTOP=gui/desktop
TETRIS=brick_game/tetris
SNAKE=brick_game/snake

COVFLAGS = -fprofile-arcs -ftest-coverage
GTEST_LIB := $(shell pkg-config --libs gtest)
INCLUDE := $(shell pkg-config --cflags gtest)

OPENOS = vi 
ifeq ($(shell uname -s), Linux) 
	OPENOS = xdg-open 
endif 
ifeq ($(shell uname -s), Darwin) 
	OPENOS = open 
endif 

all: clean cli desktop 

cli: libtetris.a libsnake.a
	mkdir build
	$(CC) $(CFLAGS) -c $(CLI)/*.c
	$(CPP) $(STANDART) -c $(CLI)/*.cpp
	cd build && $(CPP) $(STANDART) -o $(PROJECT_NAME)_cli ../*.o -lncurses -L./.. -lsnake -ltetris
	rm -rf *.o *.a

libsnake.a:
	$(CPP) $(STANDART) -c $(SNAKE)/*.cpp
	ar rc libsnake.a *.o
	rm -rf *.o

libtetris.a:
	$(CC) $(CFLAGS) -c $(TETRIS)/*.c
	ar rc libtetris.a *.o
	rm -rf *.o


desktop:
	rm -rf desk
	mkdir desk
	cd desk && qmake ../gui/desktop
	cd desk && make
	mkdir -p build
	mv desk/BrickGame_desktop.app/Contents/MacOS/BrickGame_desktop build/BrickGame_v2_desktop
	rm -rf desk

dvi: clean
	@$(OPENOS) doc/documentation.md

dist: install
	tar -cvzf $(PROJECT_NAME).tgz build
	rm -rf build

web-gui:
	python brick_game/server/app.py
	
.PHONY: tests

tests:
	coverage run --source=brick_game -m unittest discover -s tests
	coverage report -m --include="brick_game/race/game_logic.py"
	coverage html --include="brick_game/race/game_logic.py"

install: all

run_race_game:
	python3 gui/cli/brick_game_cli.py

uninstall:
	rm -rf build

clean: 
	@rm -rf build report $(PROJECT_NAME).tgz *.txt *.o *.a *.info snake_tests desk *.gcda *.gcno .coverage htmlcov