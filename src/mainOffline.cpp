//
// Created by DOTAFS on 2024/8/1.
//
#include <QApplication>
#include "mainWindowOffline.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    MainWindowOffline window;
    window.resize(500, 500);
    window.show();

    return app.exec();
}