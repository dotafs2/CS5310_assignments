#include "mainwindow.h"
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

MainWindow::MainWindow(QWidget *parent)
        : QMainWindow(parent), src(image_create(500, 500)), ssaaEnabled(false), mmsaEnabled(false) {
    // 创建中央小部件
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    // 创建左侧的图像显示区域
    graphicsView = new QGraphicsView(this);
    scene = new QGraphicsScene(this);
    graphicsView->setScene(scene);
    graphicsView->setFixedSize(1000, 500);

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
    test7b_init();
	test8b_init();
    // 启动定时器定期更新图像
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::draw);
    timer->start(50); // 10ms per draw call

    // 初始化multiplier的状态
    stackedWidget->setCurrentIndex(antiAliasingMethodComboBox->currentIndex());

}

MainWindow::~MainWindow() {
    test7b_end();
	test7c_end();
	test8b_end();
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
   // test5c(frame);
    if (frame > 100)
        frame = 0 ;
    // test5c(frame);
   // test7b(frame);
	test8b(frame);
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

void MainWindow::test7b_init() {
    int i;
    Color blue, green, purple, red, white;
    Point p[4];
    BezierCurve bc;
    View3D view;
    int divisions = 4;
    int rows = src->rows , cols = src->cols;

    // grab the command line argument, if one exists

    int tmp = 0;
    if( tmp >= 0 && tmp < 10 )
        divisions = tmp;
    printf("Creating Bezier curves with %d subdivisions\n", divisions);

    color_set(&white, 1.0, 1.0, 1.0 );
    color_set(&blue, .1, .2, .8);
    color_set(&green, .2, 0.7, 0.3 );
    color_set(&purple, 0.6, 0.1, 0.7 );
    color_set(&red, 0.75, 0.3, 0.3 );

    // set one curve
    point_set3D(&p[0], 0.0, 0.0, 0.0);
    point_set3D(&p[1], 1.0, 0.2, 0.0);
    point_set3D(&p[2], 0.7, 0.5, 0.2);
    point_set3D(&p[3], 1.0, 1.0, 1.0);
    bezierCurve_set(&bc, p);

    // put the curve into a module
    curveA = module_create();
    module_color(curveA, &blue);
    module_bezierCurve(curveA, &bc, divisions);

    // set the second curve
    point_set3D(&p[0], 0.0, 0.0, 0.0);
    point_set3D(&p[1], 0.0, 0.2, 1.0);
    point_set3D(&p[2], 0.2, 0.5, 0.7);
    point_set3D(&p[3], 1.0, 1.0, 1.0);
    bezierCurve_set(&bc, p);

    // put the curve into a module
    curveB = module_create();
    module_color(curveB, &green);
    module_bezierCurve(curveB, &bc, divisions);

    // create a module with six curves
    curves = module_create();
    for(i=0;i<3;i++) {
        module_module( curves, curveA );
        module_module( curves, curveB );
        module_rotateY( curves, cos(2.0*M_PI/3.0), sin(2.0*M_PI/3.0) );
    }

    // set up the drawstate
    drawstate_setColor(&ds, white);

    // set up the view
    point_set3D(&(view.vrp), 0.0, 0.5, -3.0 );
    vector_set( &(view.vpn), 0.0, 0.0, 1.0 );
    vector_set( &(view.vup), 0.0, 1.0, 0.0 );
    view.d = 1.0;
    view.du = 1.0;
    view.dv = 1.0*rows/cols;
    view.screeny = rows;
    view.screenx = cols;
    view.f = 0.0;
    view.b = 3.0;

    matrix_setView3D( &VTM, &view );
    matrix_identity( &GTM );

    matrix_print( &VTM, stdout );
}

void MainWindow::test7b(int frame){
		char buffer[256];
    image_reset(src);
		matrix_rotateY(&GTM, cos(M_PI/30.0), sin(M_PI/30.0) );
		module_draw( curves, &VTM, &GTM, &ds, NULL, src );
	//	sprintf(buffer, "bez3d-frame%03d.ppm", frame);
	//	image_write(src, buffer);
		// image_reset(src);
	}

void MainWindow::test7b_end() {
    module_delete( curveA );
    module_delete( curveB );
    module_delete( curves );
}

void MainWindow::test7c_init() {
	Color blue, green, purple, red, white;
	Point p[16];
	BezierSurface bc;
	View3D view;
	const int divisions = 4;

	color_set(&white, 1.0, 1.0, 1.0 );
	color_set(&blue, .1, .2, .8);
	color_set(&green, .2, 0.7, 0.3 );
	color_set(&purple, 0.6, 0.1, 0.7 );
	color_set(&red, 0.75, 0.3, 0.3 );

	curve = module_create();

	// create a flat plane
	point_set3D(&p[0], 0.0, -0.2, 0.0); // first row, constant x, even spacing in z
	point_set3D(&p[1], 0.0, -0.2, 0.33);
	point_set3D(&p[2], 0.0, -0.2, 0.66);
	point_set3D(&p[3], 0.0, -0.2, 1.0);
	point_set3D(&p[4], 0.33, -0.2, 0.0); // second row
	point_set3D(&p[5], 0.33, -0.2, 0.33);
	point_set3D(&p[6], 0.33, -0.2, 0.66);
	point_set3D(&p[7], 0.33, -0.2, 1.0);
	point_set3D(&p[8], 0.66, -0.2, 0.0); // third row
	point_set3D(&p[9], 0.66, -0.2, 0.33);
	point_set3D(&p[10], 0.66, -0.2, 0.66);
	point_set3D(&p[11], 0.66, -0.2, 1.0);
	point_set3D(&p[12], 1.0, -0.2, 0.0); // fourth row
	point_set3D(&p[13], 1.0, -0.2, 0.33);
	point_set3D(&p[14], 1.0, -0.2, 0.66);
	point_set3D(&p[15], 1.0, -0.2, 1.0);
	bezierSurface_set(&bc, p);

	// put the curve into a module
	module_color(curve, &red);
	module_bezierSurface(curve, &bc, divisions, 0);

	// create a curved surface sitting above the plane
	point_set3D(&p[0], 0.0, 0.0, 0.0); // first row, constant x, even spacing in z
	point_set3D(&p[1], 0.0, 0.2, 0.33);
	point_set3D(&p[2], 0.0, 0.5, 0.66);
	point_set3D(&p[3], 0.0, 0.1, 1.0);
	point_set3D(&p[4], 0.33, 0.8, 0.0); // second row
	point_set3D(&p[5], 0.33, -0.1, 0.33);
	point_set3D(&p[6], 0.33, 0.0, 0.66);
	point_set3D(&p[7], 0.33, 0.3, 1.0);
	point_set3D(&p[8], 0.66, 0.3, 0.0); // third row
	point_set3D(&p[9], 0.66, 0.8, 0.33);
	point_set3D(&p[10], 0.66, 0.9, 0.66);
	point_set3D(&p[11], 0.66, 0.5, 1.0);
	point_set3D(&p[12], 1.0, 0.4, 0.0); // fourth row
	point_set3D(&p[13], 1.0, 0.2, 0.33);
	point_set3D(&p[14], 1.0, 0.5, 0.66);
	point_set3D(&p[15], 1.0, 1.0, 1.0);
	bezierSurface_set(&bc, p);

	// put the curve into a module
	module_color(curve, &green);
	module_bezierSurface(curve, &bc, divisions, 0);

	// set up the drawstate
	drawstate_setColor(&ds, white);

	// set up the view
	point_set3D(&(view.vrp), 0.0, 1.2, -3.0 );
	vector_set( &(view.vpn), 0.0, -0.8, 2.5 );
	vector_set( &(view.vup), 0.0, 1.0, 0.0 );
	view.d = 1.5;
	view.du = 1.0;
	view.dv = 1.0*rows/cols;
	view.screeny = rows;
	view.screenx = cols;
	view.f = 0.0;
	view.b = 3.0;

	matrix_setView3D( &VTM, &view );
	matrix_identity( &GTM );
}


void MainWindow::test7c(int frame) {
	// Create the animation by adjusting the GTM
		char buffer[256];
		image_reset(src);
		matrix_rotateY(&GTM, cos(M_PI/30.0), sin(M_PI/30.0) );
		module_draw( curve, &VTM, &GTM, &ds, NULL, src );

	//	sprintf(buffer, "bezSurf-frame%03d.ppm", frame);
		//image_write(src, buffer);
}

void MainWindow::test7c_end() {
	module_delete( curve );
}


void MainWindow::test8b_init() {
  int i;

  Color Grey;
  Color Yellow;
  Color Blue;


	color_set( &Grey, 175/255.0, 178/255.0, 181/255.0 );
	color_set( &Yellow, 240/255.0, 220/255.0, 80/255.0 );
	color_set( &Blue, 50/255.0, 60/255.0, 200/255.0 );



  // initialize matrices
  matrix_identity(&GTM);
  matrix_identity(&VTM);

  // set the View parameters
  point_set3D(&(view3.vrp), 0.0, 0.0, -40.0);
  vector_set(&(view3.vpn), 0.0, 0.0, 1.0);
  vector_set(&(view3.vup), 0.0, 1.0, 0.0);
  view3.d = 2.0;
  view3.du = 1.0;
  view3.dv = 1.0;
  view3.f = 0.0;
  view3.b = 50;
  view3.screenx = cols;
  view3.screeny = rows;
  matrix_setView3D(&VTM, &view3);

  // print out VTM
  printf("Final VTM: \n");
  matrix_print(&VTM, stdout);

  // make a simple cube module
  cube = module_create();
  module_cube(cube, 1);

  // make a set of 3 cubes
  cubes = module_create();

  module_identity(cubes);
  module_color(cubes, &Grey);
  module_scale(cubes, 1.5, 2, 1);
  module_translate(cubes, 1, 1, 1);
  module_module(cubes, cube);

  module_identity(cubes);
  module_color(cubes, &Yellow);
  module_scale(cubes, 2, 1, 3);
  module_translate(cubes, -1, -1, -1);
  module_module(cubes, cube);

  module_identity(cubes);
  module_color(cubes, &Blue);
  module_scale(cubes, 2, 2, 2);
  module_module(cubes, cube);

  // make a scene with lots of cube sets
  scenetest8 = module_create();
	for(i=0;i<30;i++) {

		// initialize LTM
		module_identity(scenetest8);

		// rotate by some random angles
		angle = drand48() * 2*M_PI;
		module_rotateX(scenetest8, cos(angle), sin(angle));
		angle = drand48() * 2*M_PI;
		module_rotateY(scenetest8, cos(angle), sin(angle));
		angle = drand48() * 2*M_PI;
		module_rotateZ(scenetest8, cos(angle), sin(angle));

		// translate to a location
		module_translate(scenetest8,
				 (drand48()-0.5)*15.0,
				 (drand48()-0.5)*15.0,
				 (drand48()-0.5)*15.0);

		// add a tri-cube
		module_module(scenetest8, cubes);
	}

	ds = *drawstate_create();
	ds.shade = ShadeDepth;


}


void MainWindow::test8b(int i) {

		char buffer[256];

		image_reset( src );

		matrix_identity(&GTM);
		matrix_rotateY(&GTM, cos(i*2*M_PI/36.0), sin(i*2*M_PI/36.0));
		module_draw(scenetest8, &VTM, &GTM, &ds, NULL, src);

		image_write(src, buffer);

}

void MainWindow::test8b_end() {

	// free stuff here
	module_delete( cube );
	module_delete( cubes );
	module_delete( scenetest8 );
	image_free( src );
}