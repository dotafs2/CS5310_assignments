#include "mainWindow.h"
#include <QPushButton>
#include <QVBoxLayout>
#include <QImage>
#include <QPixmap>
#include <Image.h>
#include <Line.h>
#include <ball.h>
#include "anti-alias.h"

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
        : QMainWindow(parent), src(image_create(800, 800)) {
    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(centralWidget);

    label = new QLabel(this);
    layout->addWidget(label);


    QPushButton *ssaaButton = new QPushButton("Apply SSAA", this);
    layout->addWidget(ssaaButton);

    QPushButton *mmsaButton = new QPushButton("Apply MMSA", this);
    layout->addWidget(mmsaButton);

    setCentralWidget(centralWidget);

    Color White;
    Point p;
    Circle circ;

    color_set( &White, 1.0, 1.0, 1.0 );

    src = image_create( 800, 800 );

    float scale = 200.0f;
    Color red = { 1.0, 0.0, 0.0 };

    point_set2D( &p, 200, 200);
    Point center;
    center.val[0] = 400;
    center.val[1] = 400;
    center.val[2] = 0;
    center.val[3] = 1;

    Color white = {1.0, 1.0, 1.0};
    color_set(&white, 1.0, 1.0, 1.0);
    circle_set( &circ, p, 80 );
    circle_draw( &circ, src, White );
    //SSAA(src,8);
    image_write( src, "mytest2.ppm" );


    connect(ssaaButton, &QPushButton::clicked, this, &MainWindow::applySSAA);
    connect(mmsaButton, &QPushButton::clicked, this, &MainWindow::applyMMSA);
}

MainWindow::~MainWindow() {
    image_free(src);
}

void MainWindow::updateImage(Image* src) {
    QImage qImg = imageToQImage(src);
    label->setPixmap(QPixmap::fromImage(qImg));
}

void MainWindow::applySSAA()
{
    Image* src_copy = src;
    SSAA(src_copy, 8);
    updateImage(src_copy);
}

void MainWindow::applyMMSA() {
    Image* src_copy = src;
    MMSA(src, 8);
    updateImage(src_copy);
}
