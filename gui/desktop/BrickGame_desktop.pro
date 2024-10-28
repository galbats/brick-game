QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    ../../brick_game/snake/model_snake.cpp \
    ../../brick_game/snake/controller_snake.cpp \
    ../../brick_game/tetris/tetris.c \
    snakewidget.cpp \
    tetriswidget.cpp

HEADERS += \
    mainwindow.h \
    ../../brick_game/snake/model_snake.h \
    ../../brick_game/snake/controller_snake.h \
    ../../brick_game/bricks.h \
    ../../brick_game/tetris/tetris.h \
    snakewidget.h \
    tetriswidget.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
