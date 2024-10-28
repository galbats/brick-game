#include "mainwindow.h"

#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow),
      snake(new SnakeWidget),
      tetris(new TetrisWidget) {
  ui->setupUi(this);
  setFixedSize(435, 320);
}

MainWindow::~MainWindow() {
  delete snake;
  delete tetris;
  delete ui;
}

void MainWindow::on_tetrisButton_clicked() { tetris->show(); }

void MainWindow::on_snakeButton_clicked() { snake->show(); }
