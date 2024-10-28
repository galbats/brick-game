#ifndef S21_TETRIS_H
#define S21_TETRIS_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "../bricks.h"

// СТРУКТУРЫ

// typedef enum { I, O, L, J, T, Z, S } TetrominoType;

typedef enum GameState_t {
  GameStart,
  Spawn,
  Moving,
  Rotate,
  Fixing,
  GameOver
} GameState_t;

typedef struct Tetromino {
  int **position;
  int size;
  int x, y;
} Tetromino;

typedef struct ConditionGame {
  int score;
  int high_score;
  int level;
  int speed;
  bool pause;
  bool start;
} ConditionGame;

typedef struct CurrentGameInfo {
  int **field;
  Tetromino *current_piece;
  Tetromino *next_piece;
  ConditionGame current_game;
  struct timespec drop_interval;
} CurrentGameInfo;

// ДЕЙСТВИЯ НАД ФИГУРАМИ\ПОЛЕМ

Tetromino *generateNextPiece(void);
void freePiece(Tetromino *piece);
int getSize(int id);
int **getPiece(int id, int size);

bool movePieceLeft(CurrentGameInfo *info);
bool movePieceRight(CurrentGameInfo *info);
bool movePieceDown(CurrentGameInfo *info);
void rotatePiece(CurrentGameInfo *info);

void fixPiece(CurrentGameInfo *info);
void spawnNewPiece(CurrentGameInfo *info);

int **initField(int rows, int cols);
void freeField(int **matrix, int size);

// ЛОГИКА ИГРЫ
GameState_t *initGameState(void);
CurrentGameInfo *initCurrInfo(void);
GameInfo_t *initGameInfo(void);

void initCurrentGameInfo(void);
void freeGameResources(void);

GameInfo_t createGameInfo(CurrentGameInfo *info);
void updateCurrentGameInfo(CurrentGameInfo *info);

bool isGameOver(CurrentGameInfo *info);
bool canMove(CurrentGameInfo info);

void addHighScore(int high_score);
int takeHighScore(void);
int countScores(int **field);
void removeFullRow(int **field, int row);

// СОСТОЯНИЕ ИГРЫ
GameInfo_t updateCurrentState(void);
void userInput(UserAction_t action, bool hold);

#endif