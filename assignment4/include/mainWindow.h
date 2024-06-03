#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QTimer>
#include "Image.h"

class MainWindow : public QMainWindow {
Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void draw();
    void toggleSSAA();
    void toggleMMSA();

private:
    void updateImage(Image* src);

    QLabel *label;
    QTimer *timer;
    Image *src;
    bool ssaaEnabled;
    bool mmsaEnabled;
};

#endif // MAINWINDOW_H
