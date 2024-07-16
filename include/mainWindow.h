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
#include "fsMath.h"
#include "Module.h"

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
    void test5c(int frame);
    void test6a(int frame);
    void test7b(int frame);
    void test7b_init();
    void test7b_end();
    void test7c(int frame);
    void test7c_init();
    void test7c_end();
    void test8b_init();
    void test8b(int frame);
    void test8b_end();
    void setWhite(Image *src);
    double drand();

private slots:
    void toggleAntiAliasing();
    void onAntiAliasingMethodChanged(int index);

private:
    const int nLines = 50;
    const int rows = 500; // Set these to appropriate values
    const int cols = 500; // Set these to appropriate values
    View2D view;
    DrawState ds;
    View3D view3;
    Matrix vtm;
    Matrix ltm;
    Line line[50];
    Line tline;
    float orient[50];
    float freq[50];
    Color color[50];
    int i;
    Matrix GTM;
    Matrix VTM;
    Module* curves;
    Module* curveA;
    Module* curveB;
    Module* curve;
    Module *cube;
    Module *cubes;
    Module *scenetest8;
    float angle;

};

#endif // MAINWINDOW_H