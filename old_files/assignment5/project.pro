# project.pro

# 项目名称
TEMPLATE = app
TARGET = assignment5

# 编译器设置
CONFIG += c++14
QMAKE_CXXFLAGS += -g -Wall

# 包含目录
INCLUDEPATH += include

# Qt 模块
QT += core widgets

# 源文件和头文件
SOURCES += \
    src/Image.c \
    src/Line.c \
    src/anti-alias.cpp \
    src/ball.cpp \
    src/Polygon.c \
    src/fsMath.c \
    src/mainwindow.cpp \
    src/main.cpp

HEADERS += \
    include/Line.h \
    include/Polygon.h \
    include/fsMath.h \
    include/mainwindow.h

# 自动生成 MOC、UIC 和 RCC 文件
CONFIG += qmake_autogen