#include "mainWindow.h"
#include <QPushButton>
#include <QVBoxLayout>
#include <QImage>
#include <QPixmap>
#include <QTimer>
#include <Image.h>
#include <Line.h>
#include <ball.h>
#include "anti-alias.h"
#include "Polygon.h"

QImage imageToQImage(Image *img) {
    QImage qImg(img->cols, img->rows, QImage::Format_RGB32);
    for (int y = 0; y < img->rows; ++y) {
        for (int x = 0; x < img->cols; ++x) {
            FPixel pixel = img->data[y * img->cols + x];
            QColor color(pixel.rgb[0] * 255, pixel.rgb[1] * 255, pixel.rgb[2] * 255);
            qImg.setPixelColor(x, y, color);
        }
    }
    return qImg;
}

MainWindow::MainWindow(QWidget *parent)
        : QMainWindow(parent), src(image_create(800, 800)), ssaaEnabled(false), mmsaEnabled(false) {
    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(centralWidget);

    label = new QLabel(this);
    layout->addWidget(label);

    QPushButton *ssaaButton = new QPushButton("Toggle SSAA", this);
    layout->addWidget(ssaaButton);

    QPushButton *mmsaButton = new QPushButton("Toggle MMSA", this);
    layout->addWidget(mmsaButton);

    setCentralWidget(centralWidget);

    connect(ssaaButton, &QPushButton::clicked, this, &MainWindow::toggleSSAA);
    connect(mmsaButton, &QPushButton::clicked, this, &MainWindow::toggleMMSA);

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::draw);
    timer->start(10);  // 10ms per draw call
}

MainWindow::~MainWindow() {
    image_free(src);
}

void MainWindow::updateImage(Image* src) {
    QImage qImg = imageToQImage(src);
    label->setPixmap(QPixmap::fromImage(qImg));
}

void MainWindow::draw() {
    drawBall();

    if (ssaaEnabled) {
        SSAA(src, 8);
    }

    if (mmsaEnabled) {
        MMSA(src, 8);
    }

    updateImage(src);
}

void MainWindow::drawCircle(){
    Color White;
    Point p;
    Circle circ;

    color_set(&White, 1.0, 1.0, 1.0);

    image_reset(src);

    float scale = 200.0f;
    Color red = { 1.0, 0.0, 0.0 };

    point_set2D(&p, 200, 200);
    Point center;
    center.val[0] = 400;
    center.val[1] = 400;
    center.val[2] = 0;
    center.val[3] = 1;

    Color white = {1.0, 1.0, 1.0};
    color_set(&white, 1.0, 1.0, 1.0);
    circle_set(&circ, p, 80);
    circle_draw(&circ, src, White);
}


void MainWindow::drawPolygon(){
    const int rows = 300;
    const int cols = 400;
    Polygon *p = polygon_create();  // Ensure p is properly allocated
    Color Red, Orange, White, Blue;
    Point pt[100];
    int i;

    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<float> distribution(-10,10);
    uniform_real_distribution<float> distribution2(-5,5);
    uniform_real_distribution<float> distribution3(-1,1);
    color_set(&Red, 0.9, 0.2, 0.1);
    color_set(&Orange, 0.95, 0.7, 0.3);
    color_set(&White, 1.0, 1.0, 1.0);
    color_set(&Blue, 0.2, 0.1, 0.95);

    image_reset(src);

    // something more interesting
    for (i = 0; i < 50; i++) {
        float dr = distribution(gen);
        point_set2D(&(pt[i]),
                    200 + cos((float)i * M_PI * 2.0 / 50.0) * (70 + dr),
                    150 + sin((float)i * M_PI * 2.0 / 50.0) * (70 + dr));
    }
    polygon_set(p, 50, pt);
    polygon_drawFill(p, src, Red);

    for (i = 0; i < 50; i++) {
        float dr = distribution2(gen);
        point_set2D(&(pt[i]),
                    200 + cos((float)i * M_PI * 2.0 / 50.0) * (50 + dr),
                    150 + sin((float)i * M_PI * 2.0 / 50.0) * (50 + dr));
    }
    polygon_set(p, 50, pt);
    polygon_drawFill(p, src, Orange);

    for (i = 0; i < 50; i++) {
        float dr = distribution3(gen);
        point_set2D(&(pt[i]),
                    200 + cos((float)i * M_PI * 2.0 / 50.0) * (30 + dr),
                    150 + sin((float)i * M_PI * 2.0 / 50.0) * (30 + dr));
    }
    polygon_set(p, 50, pt);
    polygon_drawFill(p, src, White);

    image_write(src, "test4a.ppm");

    polygon_free(p);  // Ensure p is properly freed
}

void MainWindow::drawBall(){

    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<float> distribution(0,1);
    uniform_real_distribution<float> distribution2(0,1);
    uniform_real_distribution<float> distribution3(0,1);

    image_reset(src);
    Color White;
    Point p;
    Circle circ;
    static float time = 0;
    time += 0.01;

    color_set( &White, 1,1,1);

    float scale = 200.0f;
    Color red = { 1.0, 0.0, 0.0 };

    point_set2D( &p, 200, 200);
    Point center;
    center.val[0] = 400;
    center.val[1] = 400;
    center.val[2] = 0;
    center.val[3] = 1;
    bool fill = false;
    draw_ball(src,20,20,center,scale,White,fill,time);
}



void MainWindow::toggleSSAA() {
    ssaaEnabled = !ssaaEnabled;
}

void MainWindow::toggleMMSA() {
    mmsaEnabled = !mmsaEnabled;
}
