#include "mainwindow.h"
#include <QGraphicsPixmapItem>
#include <random>

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
        : QMainWindow(parent), src(image_create(1600, 800)), ssaaEnabled(false), mmsaEnabled(false) {
    // 创建中央小部件
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    // 创建左侧的图像显示区域
    graphicsView = new QGraphicsView(this);
    scene = new QGraphicsScene(this);
    graphicsView->setScene(scene);
    graphicsView->setFixedSize(1600, 800);

    // 创建右侧的控制面板
    QWidget *controlPanel = new QWidget(this);
    QVBoxLayout *controlLayout = new QVBoxLayout(controlPanel);
    controlLayout->setAlignment(Qt::AlignTop); // 设置顶部对齐
    controlLayout->addWidget(new QLabel("Anti-Aliasing Method:"));

    antiAliasingMethodComboBox = new QComboBox(this);
    antiAliasingMethodComboBox->addItem("None");
    antiAliasingMethodComboBox->addItem("SSAA");
    antiAliasingMethodComboBox->addItem("MMSA");
    controlLayout->addWidget(antiAliasingMethodComboBox);

    // 创建用于存放各个方法参数的QStackedWidget
    stackedWidget = new QStackedWidget(this);
    controlLayout->addWidget(stackedWidget);

    // 创建None参数页面
    QWidget *nonePage = new QWidget(this);
    QVBoxLayout *noneLayout = new QVBoxLayout(nonePage);
    noneLayout->addWidget(new QLabel("No parameters for this method."));
    stackedWidget->addWidget(nonePage);

    // 创建SSAA参数页面
    QWidget *ssaaPage = new QWidget(this);
    QVBoxLayout *ssaaLayout = new QVBoxLayout(ssaaPage);
    ssaaLayout->addWidget(new QLabel("Multiplier:"));
    antiAliasingMultiplierSpinBox = new QSpinBox(this);
    antiAliasingMultiplierSpinBox->setRange(1, 8);
    antiAliasingMultiplierSpinBox->setValue(2);
    ssaaLayout->addWidget(antiAliasingMultiplierSpinBox);
    stackedWidget->addWidget(ssaaPage);

    // 创建MMSA参数页面
    QWidget *mmsaPage = new QWidget(this);
    QVBoxLayout *mmsaLayout = new QVBoxLayout(mmsaPage);
    mmsaLayout->addWidget(new QLabel("MMSA parameters go here."));
    // Add MMSA specific controls here
    stackedWidget->addWidget(mmsaPage);

    QPushButton *applyButton = new QPushButton("Apply", this);
    controlLayout->addWidget(applyButton);

    connect(applyButton, &QPushButton::clicked, this, &MainWindow::toggleAntiAliasing);
    connect(antiAliasingMethodComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), stackedWidget, &QStackedWidget::setCurrentIndex);

    // 创建主布局，将图像区域和控制面板添加进去
    QHBoxLayout *mainLayout = new QHBoxLayout(centralWidget);
    mainLayout->addWidget(graphicsView);
    mainLayout->addWidget(controlPanel);

    // 启动定时器定期更新图像
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::draw);
    timer->start(50); // 10ms per draw call

    // 初始化multiplier的状态
    stackedWidget->setCurrentIndex(antiAliasingMethodComboBox->currentIndex());
}

MainWindow::~MainWindow() {
    image_free(src);
}

void MainWindow::updateImage(Image* src) {
    qImg = imageToQImage(src);
    scene->clear();
    scene->addPixmap(QPixmap::fromImage(qImg));
}

void MainWindow::draw() {
    srand(0x01234ABCD);
    static int frame = 0;
    test5c(frame);
// drawBall();
    // open it for use test5b
    //  test5b(frame);
    applyAntiAliasing();
    updateImage(src);
    frame++;

    if(frame >=500)
        frame = 0;
}

void MainWindow::applyAntiAliasing() {
    int multiplier = antiAliasingMultiplierSpinBox->value();
    QString method = antiAliasingMethodComboBox->currentText();

    if (method == "SSAA") {
        SSAA(src, multiplier);
    } else if (method == "MMSA") {
        MMSA(src, multiplier);
    }
}

void MainWindow::toggleAntiAliasing() {
    ssaaEnabled = !ssaaEnabled;
}

void MainWindow::onAntiAliasingMethodChanged(int index) {
    if (antiAliasingMethodComboBox->itemText(index) == "SSAA") {
        antiAliasingMultiplierSpinBox->setEnabled(true);
    } else {
        antiAliasingMultiplierSpinBox->setEnabled(false);
    }
}

void MainWindow::drawBall() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> distribution(0, 1);
    std::uniform_real_distribution<float> distribution2(0, 1);
    std::uniform_real_distribution<float> distribution3(0, 1);

    image_reset(src);
    Color White;
    Point p;
    Circle circ;
    static float time = 0;
    time += 0.01;

    color_set(&White, 1, 1, 1);

    float scale = 200.0f;
    Color red = { 1.0, 0.0, 0.0 };

    point_set2D(&p, 200, 200);
    Point center;
    center.val[0] = 400;
    center.val[1] = 400;
    center.val[2] = 0;
    center.val[3] = 1;
    bool fill = false;
    draw_ball(src, 20, 20, center, scale, White, fill, time);
}

void MainWindow::test5c(int frame) {
    const int rows = src->rows;
    const int cols = src->cols;
    const int numFrames = 500; // Number of frames for the animation

    static View3D view;
    static Matrix vtm;
    static Polygon side[6];
    static Polygon tpoly;
    static Point v[8];
    static Color color[6];
    static bool initialized = false;

    if (!initialized) {
        // set some colors
        color_set(&color[0], 0, 0, 1);
        color_set(&color[1], 0, 1, 0);
        color_set(&color[2], 1, 0, 0);
        color_set(&color[3], 1, 0, 1);
        color_set(&color[4], 0, 1, 1);
        color_set(&color[5], 1, 1, 0);

        // initialize polygons
        for (int i = 0; i < 6; i++) {
            polygon_init(&side[i]);
        }

        // corners of a cube, centered at (0, 0, 0)
        point_set(&v[0], -1, -1, -1, 1);
        point_set(&v[1], 1, -1, -1, 1);
        point_set(&v[2], 1, 1, -1, 1);
        point_set(&v[3], -1, 1, -1, 1);
        point_set(&v[4], -1, -1, 1, 1);
        point_set(&v[5], 1, -1, 1, 1);
        point_set(&v[6], 1, 1, 1, 1);
        point_set(&v[7], -1, 1, 1, 1);

        // front side
        polygon_set(&side[0], 4, &(v[0]));

        // back side
        polygon_set(&side[1], 4, &(v[4]));

        // top side
        Point tv[4];
        point_copy(&tv[0], &v[2]);
        point_copy(&tv[1], &v[3]);
        point_copy(&tv[2], &v[7]);
        point_copy(&tv[3], &v[6]);
        polygon_set(&side[2], 4, tv);

        // bottom side
        point_copy(&tv[0], &v[0]);
        point_copy(&tv[1], &v[1]);
        point_copy(&tv[2], &v[5]);
        point_copy(&tv[3], &v[4]);
        polygon_set(&side[3], 4, tv);

        // left side
        point_copy(&tv[0], &v[0]);
        point_copy(&tv[1], &v[3]);
        point_copy(&tv[2], &v[7]);
        point_copy(&tv[3], &v[4]);
        polygon_set(&side[4], 4, tv);

        // right side
        point_copy(&tv[0], &v[1]);
        point_copy(&tv[1], &v[2]);
        point_copy(&tv[2], &v[6]);
        point_copy(&tv[3], &v[5]);
        polygon_set(&side[5], 4, tv);

        initialized = true;
    }

    // Create image
    image_reset(src);

    // Set the viewpoint based on the current frame
    float alpha = (float)frame / (numFrames - 1);
    point_set(&(view.vrp), 3 * cos(alpha * 2 * M_PI), 2, 3 * sin(alpha * 2 * M_PI), 1.0);
    vector_set(&(view.vpn), -view.vrp.val[0], -view.vrp.val[1], -view.vrp.val[2]);
    vector_set(&(view.vup), 0, 1, 0);

    view.d = 1;  // focal length
    view.du = 2;
    view.dv = view.du * (float)rows / cols;
    view.f = 0; // front clip plane
    view.b = 4; // back clip plane
    view.screenx = cols;
    view.screeny = rows;

    matrix_setView3D(&vtm, &view);

    // use a temporary polygon to transform stuff
    polygon_init(&tpoly);
    for (int i = 0; i < 6; i++) {
        polygon_copy(&tpoly, &side[i]);
        matrix_xformPolygon(&vtm, &tpoly);

        // normalize by homogeneous coordinate before drawing
        polygon_normalize(&tpoly);
        polygon_draw(&tpoly, src, color[i]);
    }
}