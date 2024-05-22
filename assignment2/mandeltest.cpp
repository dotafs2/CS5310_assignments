#include <iostream>
#include <cstdlib>
#include <sys/stat.h>
#include <cstring>
#include <cerrno>

extern "C" {
#include "Image.h"
}
#include "fractals.h"

static const int numOfFrames = 40;

bool folder_exists(const char* folder) {
    struct stat info;
    if (stat(folder, &info) != 0) {
        std::cerr << "Cannot access " << folder << ": " << strerror(errno) << std::endl;
        return false;
    } else if (!(info.st_mode & S_IFDIR)) {
        std::cerr << folder << " is not a directory" << std::endl;
        return false;
    }
    return true;
}

bool execute_command(const char* command) {
    int result = system(command);
    if (result != 0) {
        std::cerr << "Command failed: " << command << std::endl;
        return false;
    }
    return true;
}

int main(int argc, char *argv[]) {
//    const char* folder = "../master";
//
//    // Check if the folder exists
//    if (!folder_exists(folder)) {
//        return 1;
//    }
//
//    for (int i = 0; i < numOfFrames; ++i) {
//        Image *srd = image_create(750, 1000);
//        float x0 = -1.8;
//        float y0 = -1.8 * 0.75;
//        float dx = 3.6;
//        float offset = -0.001;
//       // julia(srd, x0, y0, dx);
//        myJulia(srd,x0,y0,dx,i * offset);
//        char filename[256];
//        snprintf(filename, sizeof(filename), "%s/julia_%02d.ppm", folder, i);
//        image_write(srd, filename);
//        image_free(srd);
//    }
//    // Use ImageMagick to combine the PPM files into a GIF
//    char command[512];
//    snprintf(command, sizeof(command), "convert -delay 20 -loop 0 %s/julia_*.ppm julia_animation.gif", folder);
//    if (!execute_command(command)) {
//        return 1;
//    }
//
//    std::cout << "GIF created successfully: julia_animation.gif" << std::endl;


    Image *src;

    src = image_create(750, 1000);

    mandelbrot( src, -1.5, -1.5, 4);

    image_write( src, "mandelbrot-full.ppm");

    mandelbrot( src, 1.755, -0.02, 0.02);

    image_write( src, "mandelbrot.ppm");

    julia( src, -1.8, -1.8*0.75, 3.6);

    image_write( src, "julia.ppm");

    image_free( src );


    return 0;
}
