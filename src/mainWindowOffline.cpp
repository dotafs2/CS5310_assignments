#include "mainWindowOffline.h"
#include <QGraphicsPixmapItem>
#include <random>
#include "Module.h"


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

MainWindowOffline::MainWindowOffline(QWidget *parent)
        : QMainWindow(parent), src(image_create(720, 1280)){
    // 创建中央小部件
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    // 创建左侧的图像显示区域
    graphicsView = new QGraphicsView(this);
    scene = new QGraphicsScene(this);
    graphicsView->setScene(scene);
    graphicsView->setFixedSize(1480, 940);

    QHBoxLayout *mainLayout = new QHBoxLayout(centralWidget);
    mainLayout->addWidget(graphicsView);

    water = WaterSimulation();
   //  water.SinusoidsWaveInit();
    water.PBFInit();
  //  water.TexturePlayGroundInit();


    // 启动定时器定期更新图像
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindowOffline::draw);
    if(realtime) // realtime rendering
        timer->start(50); // 10 ms per draw
    else // offline rendering
        timer->start(50); // 1000 ms per draw
}

MainWindowOffline::~MainWindowOffline() {
	for (auto timer : keyTimers) {
		if (timer) {
			timer->stop();
			delete timer;
		}
	}
    image_free(src);
}

void MainWindowOffline::updateImage(Image* src) {
    if (qImg.isNull() || qImg.width() != src->cols || qImg.height() != src->rows) {
        qImg = QImage(src->cols, src->rows, QImage::Format_RGB32);
    }

    for (int y = 0; y < src->rows; ++y) {
        for (int x = 0; x < src->cols; ++x) {
            FPixel pixel = src->data[y * src->cols + x];
            QColor color(pixel.rgb[0] * 255, pixel.rgb[1] * 255, pixel.rgb[2] * 255);
            qImg.setPixelColor(x, y, color);
        }
    }

    scene->clear();
    scene->addPixmap(QPixmap::fromImage(qImg));
}


void MainWindowOffline::draw() {
    static int frame = 0;
	//src = water.SinusoidsWave(frame);
   // src = water.TexturePlayGround(frame * 2)

    src = water.PBF(frame/10);
    frame ++;
    char filename[256];
    int total_frames = 100;
    snprintf(filename, sizeof(filename), "frame_%03d.ppm", frame);
    image_write(src, filename);
    updateImage(src);
}