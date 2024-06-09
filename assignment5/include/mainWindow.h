#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QComboBox>
#include <QSpinBox>
#include <QVBoxLayout>
#include <QLabel>
#include <QImage>
#include <QPixmap>
#include <QTimer>
#include <QPushButton>
#include <QStackedWidget>
#include "ball.h"
#include "anti-alias.h"
#include "Image.h"

class MainWindow : public QMainWindow {
Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    QGraphicsView *graphicsView;
    QGraphicsScene *scene;
    QLabel *imageLabel;
    QImage qImg;
    Image *src;
    QTimer *timer;
    bool ssaaEnabled;
    bool mmsaEnabled;
    QComboBox *antiAliasingMethodComboBox;
    QSpinBox *antiAliasingMultiplierSpinBox;
    QStackedWidget *stackedWidget;

    void updateImage(Image* src);
    void draw();
    void applyAntiAliasing();
    void drawBall();

private slots:
    void toggleAntiAliasing();
    void onAntiAliasingMethodChanged(int index);
};

#endif // MAINWINDOW_H
