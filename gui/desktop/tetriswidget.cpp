#include "tetriswidget.h"

TetrisWidget::TetrisWidget(QWidget *parent) : QWidget{parent} {
  setFixedSize(505, 605);
  setFocusPolicy(Qt::StrongFocus);
  setWindowTitle("TETRIS BRICK GAME");
  timer = new QTimer;
  initCurrentGameInfo();
  connect(timer, &QTimer::timeout, this, &TetrisWidget::update_paint);
  timer->start(50);
}

TetrisWidget::~TetrisWidget() { delete timer; }

void TetrisWidget::paintEvent(QPaintEvent *e) {
  Q_UNUSED(e)
  QPainter game_field(this);
  game_field.drawRect(0, 0, 300, 600);
  GameInfo_t inf = updateCurrentState();
  for (int i{}; i < 20; ++i) {
    for (int j{}; j < 10; ++j) {
      if (inf.field[i][j]) {
        game_field.setBrush(QColor(0, 100, 0));
      } else {
        game_field.setBrush(Qt::black);
      }
      game_field.fillRect(30 * j, 30 * i, 30, 30, game_field.brush());
    }
  }

  // Рисование серого поля справа
  game_field.setBrush(Qt::gray);
  game_field.drawRect(300, 0, 305, 600);
  game_field.fillRect(300, 0, 305, 600, game_field.brush());

  if (inf.next) {
    int len = initCurrInfo()->next_piece->size;
    for (int i{}; i < len; ++i) {
      for (int j{}; j < len; ++j) {
        if (inf.next[i][j]) {
          game_field.setBrush(QColor(0, 100, 0));
        } else {
          game_field.setBrush(Qt::gray);
        }
        game_field.fillRect(30 * j + 310, 30 * i + 30, 30, 30,
                            game_field.brush());
      }
    }
  }
  QFont font;
  font.setPointSize(20);
  game_field.setFont(font);
  game_field.setPen(Qt::black);
  game_field.drawText(305, 20, "Next:");
  std::string s = "Score: " + std::to_string(inf.score);
  game_field.drawText(305, 200, s.data());
  std::string l = "Level: " + std::to_string(inf.level);
  game_field.drawText(305, 240, l.data());
  std::string hs = "High score: " + std::to_string(inf.high_score);
  game_field.drawText(305, 280, hs.data());
  if (inf.pause) {
    game_field.drawText(305, 420, "Pause");
  }
}

void TetrisWidget::keyPressEvent(QKeyEvent *event) {
  UserAction_t action = Start;
  if (event->key() == Qt::Key_Down) {
    action = Down;
  } else if (event->key() == Qt::Key_Up) {
    action = Up;
  } else if (event->key() == Qt::Key_Left) {
    action = Left;
  } else if (event->key() == Qt::Key_Right) {
    action = Right;
  } else if (event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return) {
    action = Start;
  } else if (event->key() == 'q' || event->key() == 'Q') {
    action = Terminate;
  } else if (event->key() == 'p' || event->key() == 'P') {
    action = Pause;
  } else if (event->key() == 'r' || event->key() == 'R') {
    action = Action;
  }
  bool hold = (action == Down) ? true : false;
  userInput(action, hold);
  update_paint();
  if (action == Terminate) {
    QWidget::close();
  }
}

void TetrisWidget::update_paint(void) { repaint(); }
