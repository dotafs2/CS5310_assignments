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
    srand(0x01234ABCD);
    static int frame = 0;
    // Generate random lines in the [0,1] square
    for (i = 0; i < nLines; i++) {
        line_set2D(&(line[i]), drand(), drand(), drand(), drand());
        orient[i] = drand() * M_PI * 2;
        freq[i] = 0.25 * drand();
        color_set(&(color[i]), drand(), drand(), drand());
        printf("Initial Line %d: (%.2f, %.2f) to (%.2f, %.2f)\n",
               i, line[i].a.val[0], line[i].a.val[1], line[i].b.val[0], line[i].b.val[1]);
    }

    // Set up a view centered on (1.5, 1.5) with x pointing right
    point_set2D(&(view.vrp), 1.8, 1.8);
    view.dx = 1.0;
    vector_set(&(view.x), 1.0, 0.0, 0.0);
    view.screenx = cols;
    view.screeny = rows;

    matrix_setView2D(&vtm, &view);
    printf("vtm:\n");
    matrix_print(&vtm, stdout);
   // drawBall();
    test5b(frame);
    applyAntiAliasing();
    updateImage(src);
    frame++;
    if(frame >=50)
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

void MainWindow::setWhite( Image *src ) {
    int i, j;
    Color White;

    color_set(&White, 1.0, 1.0, 1.0);

    for(i=0;i<src->rows;i++) {
        for(j=0;j<src->cols;j++) {
            image_setColor( src, i, j, White );
        }
    }
}
double MainWindow::drand() {
    return (double)rand() / RAND_MAX;
}

void MainWindow::test5b(int frame) {

    setWhite(src);

    // Rotate the lines about one end
    for (i = 0; i < nLines; i++) {
        float angle = orient[i] + freq[i] * 2 * M_PI * frame / 50;
        tline = line[i];
        printf("Original Line %d: (%.2f, %.2f) to (%.2f, %.2f)\n",
               i, line[i].a.val[0], line[i].a.val[1], line[i].b.val[0], line[i].b.val[1]);

        matrix_identity(&ltm);
        matrix_translate2D(&ltm, -tline.a.val[0], -tline.a.val[1]);
        matrix_rotateZ(&ltm, cos(angle), sin(angle));
        matrix_translate2D(&ltm, tline.a.val[0], tline.a.val[1]);
        printf("ltm:\n");
        matrix_print(&ltm, stdout);

        matrix_xformLine(&ltm, &tline);
        printf("Transformed Line %d: (%.2f, %.2f) to (%.2f, %.2f)\n",
               i, tline.a.val[0], tline.a.val[1], tline.b.val[0], tline.b.val[1]);
        matrix_xformLine(&vtm, &tline);
        printf("View Transformed Line %d: (%.2f, %.2f) to (%.2f, %.2f)\n",
               i, tline.a.val[0], tline.a.val[1], tline.b.val[0], tline.b.val[1]);

        line_draw(&tline, src, color[i]);
    }

    printf("Writing file\n");
    // sprintf(filename, "frame-%04d.ppm", frame);
    // image_write(src, filename);

    // Translate the view across the scene
    point_set2D(&(view.vrp), 1.8 - 2.4 * (frame + 1) / 50, 1.8 - 2.4 * (frame + 1) / 50);
    matrix_setView2D(&vtm, &view);
    matrix_print(&vtm, stdout);
}


