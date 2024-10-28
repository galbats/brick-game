#include <QApplication>

#include "mainwindow.h"

int main(int argc, char *argv[]) {
  // Создание объекта QApplication, который управляет ресурсами приложения и
  // событиями
  QApplication app(argc, argv);

  // Создание главного окна приложения
  MainWindow win;

  // Отображение главного окна на экране
  win.show();

  // Запуск главного цикла обработки событий. Программа будет работать до тех
  // пор, пока не завершится цикл событий.
  return app.exec();
}
