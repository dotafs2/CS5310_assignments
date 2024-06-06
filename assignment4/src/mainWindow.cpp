#include "mainWindow.h"
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
    controlLayout->setAlignment(Qt::AlignTop);
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
    timer->start(10);  // 10ms per draw call

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
    drawBall();
    applyAntiAliasing();
    updateImage(src);
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
