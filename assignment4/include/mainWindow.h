#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QListWidget>
#include "Image.h"

class MainWindow : public QMainWindow {
Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Image *src;
    QLabel *label;
    QListWidget *listWidget;
    void updateImage(Image* src);

private slots:
    void applySSAA();
    void applyMMSA();
};

#endif // MAINWINDOW_H
