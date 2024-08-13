//
// Created by DOTAFS on 2024/7/31.
//

#ifndef MAINWINDOWOFFLINE_H
#define MAINWINDOWOFFLINE_H

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
#include "WaterSimulation.h"
#include <QKeyEvent>
#include <memory>

class MainWindowOffline : public QMainWindow {
    Q_OBJECT

    public:
    MainWindowOffline(QWidget *parent = nullptr);
    ~MainWindowOffline();

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
    void setWhite(Image *src);
    double drand();
    QMap<int, QTimer*> keyTimers;

    WaterSimulation water;
    const int nLines = 50;
    const int rows = 720; // Set these to appropriate values
    const int cols = 1280; // Set these to appropriate values
    View2D view;
    DrawState ds;
    View3D view3;
    Matrix vtm;
    Matrix ltm;

    Matrix GTM;
    Matrix VTM;
    Module* curves;
    Module* curveA;
    Module* curveB;
    Module* curve;
    Module *cube;
    float angle;
    float ZPlus;
    float XSub;
    float ZSub;
    float XPlus;


};
#endif //MAINWINDOWOFFLINE_H
