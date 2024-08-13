//
// Created by DOTAFS on 2024/7/25.
//

#include "WaterSimulation.h"


unsigned int seed = 1314;
perlinNoise perlin(seed);

float random_float(float min, float max) {
    return min + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (max - min)));
}


WaterSimulation::WaterSimulation() {
    points = (Point **)malloc(HEIGHT * sizeof(Point *));
    for (int i = 0; i < HEIGHT; ++i) {
        points[i] = (Point *)malloc(WIDTH * sizeof(Point));
    }


    checkboardpoints = (Point **)malloc(HEIGHT * sizeof(Point *));
    for (int i = 0; i < HEIGHT; ++i) {
        checkboardpoints[i] = (Point *)malloc(WIDTH * sizeof(Point));
    }

}


void init_fogSystem(std::vector<Fog>& fogSystem, const perlinNoise& perlin) {
    for (int i = 0; i < FOGAMOUNT; i++) {
        Fog* fog = (Fog*)malloc(sizeof(Fog));  // 使用 malloc 分配 Fog 对象的内存
        if (fog == NULL) {
            fprintf(stderr, "Failed to allocate memory for fog\n");
            exit(EXIT_FAILURE);
        }

        Point position;

        // 使用Perlin噪声生成粒子在水面上方的随机位置
        float x = random_float(2, 8);
        float y = random_float(2.8, 2.9);  // 雾的高度范围
        float z = random_float(2, 5);
        position.val[0] = x;
        position.val[1] = y;
        position.val[2] = z;
        position.val[3] = 1;

        // 初始化粒子
        initialize_fog(fog, position);

        fog->c.c[0] = 1;
        fog->c.c[1] = 1;
        fog->c.c[2] = 1;

        // 使用Perlin噪声生成随机透明度
        float noiseValue = perlin.noise(x, y, z);
        fog->alpha = random_float(0.1f, 0.8f) * noiseValue;

        // 将粒子添加到系统中
        fogSystem.push_back(*fog);

        // 释放malloc的内存，因为我们已经将fog对象拷贝到了vector中
        free(fog);
    }
}

void update_fogSystem(std::vector<Fog>& fogSystem, float delta_time, const perlinNoise& perlin) {
    for (int i = 0; i < FOGAMOUNT; i++) {
        Fog& fog = fogSystem[i];

        // 使用Perlin噪声控制粒子缓慢移动，模拟雾气的漂浮
        float noiseX = perlin.noise(fog.position.val[0], fog.position.val[1], fog.position.val[2]);
        float noiseY = perlin.noise(fog.position.val[0] + 10, fog.position.val[1], fog.position.val[2]);
        float noiseZ = perlin.noise(fog.position.val[0], fog.position.val[1], fog.position.val[2] + 10);

        fog.position.val[0] += noiseX * random_float(-0.1f, 0.1f) ;
        fog.position.val[1] += noiseY * random_float(-0.1f, 0.1f) ;
        fog.position.val[2] += noiseZ * random_float(-0.1f, 0.1f);

        // 轻微调整粒子的透明度，模拟雾气的动态变化
        float noiseAlpha = perlin.noise(fog.position.val[0], fog.position.val[1], fog.position.val[2]);
        fog.alpha += random_float(-0.0f, 0.1f) * noiseAlpha;
        if (fog.alpha < 0.2f) fog.alpha = 0.2f;
        if (fog.alpha > 0.8f) fog.alpha = 0.8f;
    }
}


double WaterSimulation::calculate_wave_height_SW(double x, double z, double t, double y0, int Nw,
                             double *A, double *kx, double *kz, double *omega) {
    double height = -y0;
    //  double distance = sqrt( x * x  + z * z);  // normal of vector which used in the center of water
    double centerX = WIDTH * 0.5 / 2;
    double centerZ = HEIGHT * 0.5 / 2;
    double distance = sqrt((x - centerX) * (x - centerX) + (z - centerZ) * (z - centerZ));
    double distance2 = sqrt((x) * (x) + (z) * (z));
    double distance3 = sqrt((x - 2 *centerX)* (x - 2 * centerX) + (z - 2*centerZ) * (z - 2*centerZ));

    double wave1 =  A[0] * cos(kx[0] * distance2 - omega[0] * t); // center water simulation
    double wave2 = A[1] * cos(kx[1] * x + kz[1] * z - omega[1] * t); // normal simulation
    double wave3 =  A[2] * cos(kx[2] * distance3 - omega[2] * t);
    double wave4 =  A[3] * cos(kx[3] * distance - omega[3] * t);
    for (int i = 0; i < Nw; i++) {
        if(realtime) {
            height += A[i] * cos(kx[i] * x + kz[i] * z - omega[i] * t); // normal simulation
        }
        else {
            height = wave1 + wave2 + wave3 + wave4;
        }
    }
    return height;
}


void WaterSimulation::SinusoidsWaveInit(){
    // initialize the image
    color_set( &White, 1, 1, 1 );
    color_set( &Grey, 0.6, 0.62, 0.64 );
    color_set( &DkGrey, 0.1, 0.1, 0.1 );
    color_set(&Blue, 0.1,0.25,0.35);
    src = image_create(rows, cols);

    // initialize matrices
    matrix_identity(&GTM);
    matrix_identity(&VTM);

    // set the View parameters
    point_set3D(&(view.vrp), 2, 2.5, -6.0);
    vector_set(&(view.vpn), -3, -2, 5);
    vector_set(&(view.vup), 0.0, 1.0, 0.0);
    view.d = 2.0;
    view.du = 1.6;
    view.dv = .9;
    view.f = 0.0;
    view.b = 15;
    view.screenx = cols;
    view.screeny = rows;
    matrix_setView3D(&VTM, &view);
        for (int i = 0; i < HEIGHT; ++i) {
            for (int j = 0; j < WIDTH; ++j) {
                if(realtime) {
                    points[i][j].val[0] = i * 0.1;
                    points[i][j].val[1] = 0; // height
                    points[i][j].val[2] = j;
                    points[i][j].val[3] = 1.0;
                }
                else {
                    points[i][j].val[0] = i * 0.01;
                    points[i][j].val[1] = 0; // height
                    points[i][j].val[2] = j *0.01;
                    points[i][j].val[3] = 1.0;
                }
            }
        }

    // create drawstate and module
    ds = drawstate_create();
    cube = module_create();
}

Image* WaterSimulation::SinusoidsWave(int t) {
    // reset image pixels and module
    image_reset(src);
    module_clear(cube);
    // update height of all waves
    for (int i = 0; i < HEIGHT; ++i) {
        for (int j = 0; j < WIDTH; ++j) {
            double x = points[i][j].val[0];
            double z = points[i][j].val[2];
            double y = 0; // prevent bug
            point_print(&points[i][j],stdout);
            if(realtime)
                y = calculate_wave_height_SW(x * 5, z * 5, t, y0, Nw, A, kx, kz, omega);
            else
                y = calculate_wave_height_SW(x * 50, z * 50, t, y0, Nw, A, kx, kz, omega);

            points[i][j].val[1] = y/25; // Adjust this divisor as need
        }
    }
    // 计算水面中心
    int midX = WIDTH / 2;
    int midZ = HEIGHT / 2;

    // 获取中心点的 X 和 Z 值
    double centerX = points[midZ][midX].val[0];
    double centerZ = points[midZ][midX].val[2];

    point_copy(&(ds->viewer), &(view.vrp));
  //  module_addLight(cube,light);

    module_scale(cube,1.2,1.2,1.2);
    module_color( cube, &White );
    module_translate(cube,-5,-1.5,0);
    module_bodyColor( cube, &White );
    module_surfaceColor( cube, &White );

    light = lighting_create();
    Point light_pos = {centerX - 4,4,centerZ + 4,1};
    lighting_add( light, LightPoint, &White, NULL, &(light_pos), 0, 0 );
    module_addLight(cube,light);
    module_plane( cube, points, HEIGHT, WIDTH);
    // module_checkboard(cube);
    // module_rotateZ(cube,0.0,1.0);

    ds->shade = ShadePhong;
    //ds->shade = ShadeConstant;
    matrix_identity(&GTM);
   // set_skybox(src,&VTM,view);
   // set_checkboard(src,&VTM,view);
    ds->textureMaps[0] = *texture_read("water.ppm");
    ds->textureMaps[1] = *texture_read("checkboard.ppm");
    // module_parseLighting(cube,&GTM,light);
  module_draw(cube, &VTM, &GTM, ds, NULL, src);


    return src;
}

void WaterSimulation::update_viewPosition(float x, float z) {
    view.vrp.val[0] += x;
    view.vrp.val[2] += z;
    matrix_setView3D(&VTM, &view);
}


void WaterSimulation::PBFInit() {

    // initialize the image
    color_set( &White, 1, 1, 1);
    color_set( &Grey, 0.6, 0.62, 0.64 );
    color_set( &DkGrey, 0.1, 0.1, 0.1 );
    color_set(&Blue, 0.1,0.25,0.35);
    color_set(&Red, 0.35,0.25,0.1);
    src = image_create(rows, cols);
    // initialize matrices
    matrix_identity(&GTM);
    matrix_identity(&VTM);
    // set the View parameters
    point_set3D(&(view.vrp), 2, 2.5, -6.0);
    vector_set(&(view.vpn), -3, -2, 5);
    vector_set(&(view.vup), 0.0, 1.0, 0.0);
    view.d = 2.0;
    view.du = 1.6;
    view.dv = .9;
    view.f = 0.0;
    view.b = 15;
    view.screenx = cols;
    view.screeny = rows;
    matrix_setView3D(&VTM, &view);
    // create drawstate and module
    ds = drawstate_create();
    ds->textureMaps[0] = *texture_read("water.ppm");
    ds->textureMaps[0].alphaFlag = 1;
    ds->textureMaps[1] = *texture_read("checkboard.ppm");
    ds->textureMaps[0].alphaFlag = 0;
    cube = module_create();
    particleCount = PARTICLECOUNT;

    particles = initialize_particles(XCOUNT,YCOUNT,ZCOUNT,MAX_NEIGHBOURS);

    grid = initialize_grid(10);
    add_all_particles_to_bin(grid,particles,particleCount,RANGE_H,simSizeX,simSizeY,simSizeZ);
}

Image* WaterSimulation::PBF(float t) {
    image_reset(src);
    module_clear(cube);
    // clear all bin in grid
    for (int i = 0; i < simSizeX; i++) {
        for (int j = 0; j < simSizeY; j++) {
            for (int k = 0; k < simSizeZ; k++) {
                grid[i][j][k].size = 0;
                // no need reset element, because size decide wheather it exist or not
                printf("%d\n",simSizeX);
            }
        }
    }

    // reset the bin
    add_all_particles_to_bin(grid,particles,particleCount,RANGE_H,simSizeX,simSizeY,simSizeZ);

    update_all_particles(particles,PARTICLECOUNT,20,TIME_STEP,grid,RANGE_H);

    module_scale(cube,1.2,1.2,1.2);
    module_color( cube, &White );
    module_translate(cube,-5,-1.5,0);
    module_bodyColor( cube, &White );
    module_surfaceColor( cube, &White );
    //  module_plane( cube, points, HEIGHT, WIDTH);
     module_checkboard(cube);

     module_bodyColor( cube, &Blue );
     module_surfaceColor( cube, &Blue );

    // 更新速度和位置
    for (int i = 0; i < particleCount; i++) {
       module_sphere(cube, particles[i].position,0.03,1, 8, 8 );
    }

    light = lighting_create();
    lighting_add( light, LightPoint, &White, nullptr, &(view.vrp), 0, 0 );
    point_copy(&(ds->viewer), &(view.vrp));
    ds->shade = ShadePhong;
    matrix_identity(&GTM);
    ds->color = White;
    // module_parseLighting(cube,&GTM,light);
    module_draw(cube, &VTM, &GTM, ds, light, src);


    return src;

}


void WaterSimulation::TexturePlayGroundInit(){


    srand(static_cast<unsigned int>(time(0)));


    // initialize the image
    color_set( &White, 1, 1, 1);
    color_set( &Grey, 0.6, 0.62, 0.64 );
    color_set( &DkGrey, 0.1, 0.1, 0.1 );
    color_set(&Blue, 0.1,0.25,0.35);
    color_set(&Red, 0.35,0.25,0.1);
    src = image_create(rows, cols);
    // char *filename = "test9d.ppm";
    // src = image_read(filename);

    // initialize matrices
    matrix_identity(&GTM);
    matrix_identity(&VTM);
    // set the View parameters
    point_set3D(&(view.vrp), 2, 2.5, -6.0);
    vector_set(&(view.vpn), -3, -2, 5);
    vector_set(&(view.vup), 0.0, 1.0, 0.0);
    view.d = 2.0;
    view.du = 1.6;
    view.dv = 0.9;
    view.f = 0;
    view.b = 100;
    view.screenx = cols;
    view.screeny = rows;
    matrix_setView3D(&VTM, &view);
    for (int i = 0; i < HEIGHT; ++i) {
        for (int j = 0; j < WIDTH; ++j) {
            if(realtime) {
                points[i][j].val[0] = i * 0.1;
                points[i][j].val[1] = 0; // height
                points[i][j].val[2] = j;
                points[i][j].val[3] = 1.0;
            }
            else {
                points[i][j].val[0] = i * 0.02;
                points[i][j].val[1] = 0; // height
                points[i][j].val[2] = j * 0.02;
                points[i][j].val[3] = 1.0;
            }
        }
    }

    for (int i = 0; i < HEIGHT; ++i) {
        for (int j = 0; j < WIDTH; ++j) {
            if(realtime) {
                checkboardpoints[i][j].val[0] = i * 0.1;
                checkboardpoints[i][j].val[1] = 0; // height
                checkboardpoints[i][j].val[2] = j;
                checkboardpoints[i][j].val[3] = 1.0;
            }
            else {
                checkboardpoints[i][j].val[0] = i * 0.02;
                checkboardpoints[i][j].val[1] = 0; // height
                checkboardpoints[i][j].val[2] = j * 0.02;
                checkboardpoints[i][j].val[3] = 1.0;
            }
        }
    }

    // create drawstate and module
    ds = drawstate_create();
    ds->color = DkGrey;
    ds->textureMaps[0] = *texture_read("water.ppm");
    ds->textureMaps[0].alphaFlag = 1;
    ds->textureMaps[1] = *texture_read("checkboard.ppm");
    ds->textureMaps[1].alphaFlag = 0;
    cube = module_create();

    float minX = 0.1, maxX = 8;
    float minZ = 0.1, maxZ = 5;
    float minY = 0.1, maxY = 2.8;


    for (int i = 0; i < 200; ++i) {
        Point random_position;
        random_position.val[0] = random_float(minX, maxX); // 随机X
        random_position.val[1] = random_float(minY,maxY);       // 固定Y
        random_position.val[2] = random_float(minZ, maxZ); // 随机Z
        random_position.val[3] = 1.0f;

        rainPoints.push_back(random_position); // 将点添加到动态数组中
        Particle particle;
        // 调用 initialize_particle 函数


     //   init_fogSystem(fogs,perlin);
        // initialize_particle(&particle, random_position, MAX_NEIGHBOURS);
    }




}

Image* WaterSimulation::TexturePlayGround(float t) {
 image_reset(src);
    module_clear(cube);
    module_scale(cube,1.2,1.2,1.2);
    module_color( cube, &White );
    module_translate(cube,-8.5,-1.5,-3.5);
    module_bodyColor( cube, &White );
    module_surfaceColor( cube, &White );
    light = lighting_create();

    Point light_pos = {-100,45,50,1};
    lighting_add( light, LightPoint, &White, nullptr, &(light_pos), 0, 0 );
    module_addLight(cube,light);
    // update height of all waves
    for (int i = 0; i < HEIGHT; ++i) {
        for (int j = 0; j < WIDTH; ++j) {
            double x = points[i][j].val[0];
            double z = points[i][j].val[2];
            double y = 0; // prevent bug
            if(realtime)
                y = calculate_wave_height_SW(x * 5, z * 5, t, y0, Nw, A, kx, kz, omega);
            else
                 y = calculate_wave_height_SW(x * 50, z * 50, t, y0, Nw, A, kx, kz, omega);

            points[i][j].val[1] = y/18 + 0.3; // Adjust this divisor as need
        }
    }


 for (auto & rainPoint : rainPoints) {
     rainPoint.val[1] -= 2 * 0.1;
     if(rainPoint.val[1] <= 0) {
         rainPoint.val[0] = random_float(0.5, 8);
         rainPoint.val[2] = random_float(0.5, 5);
         rainPoint.val[1] = RANGE_Y_MAX;
     }
     module_sphere(cube, rainPoint,0.03,0.3, 8, 8 );
 }

   // update_fogSystem(fogs, 0.5, perlin);



    module_plane_checkboard(cube,checkboardpoints,HEIGHT,WIDTH);
     module_plane( cube, points, HEIGHT, WIDTH);
    // for (auto & fog : fogs) {
    //     module_fog(cube,&fog);
    // }
     //module_checkboard(cube);
     module_bodyColor( cube, &Blue );
     module_surfaceColor( cube, &Blue );


    point_copy(&(ds->viewer), &(view.vrp));
    ds->shade = ShadePhong;
    matrix_identity(&GTM);
    module_draw(cube, &VTM, &GTM, ds, NULL, src);
    return src;
}