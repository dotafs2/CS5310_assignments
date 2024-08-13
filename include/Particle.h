//
// Created by DOTAFS on 2024/8/5.
//

#ifndef PARTICLE_H
#define PARTICLE_H

#endif //PARTICLE_H


#include "Image.h"
#include "Definition.h"
#ifdef __cplusplus
extern "C" {
#endif




 //   Bin grid[GRID_SIZE][GRID_SIZE][GRID_SIZE];


void bin_init(Bin *bin, int capacity);
void bin_add(Bin *bin, Particle *particle);
void bin_free(Bin *bin);
void bin_clear(Bin *bin);
void initialize_particle(Particle *particle, Point position, int maxNeighbours);
Particle* initialize_particles(int xCount, int yCount, int zCount, int maxNeighbours);
Bin*** initialize_grid(int initialBinCapacity);
void free_particle(Particle *particle);
void updateSpeed(Particle p, float dt);
void updatePosition(Particle p, float dt);
void apply_forces(Particle *particle, float deltaTime);
void find_neighbours(Particle *particle, float h, Bin ***grid);
void update_particle(Particle *particle, float deltaTime, Bin ***grid, int gridSize, float h);
void update_all_particles(Particle *particles, int numParticles, int solverIterations, float deltaTime, Bin ***grid, float h);
void add_particle_to_bin(Bin ***grid, Particle *particle, float h, int gridSizeX, int gridSizeY, int gridSizeZ);
void add_all_particles_to_bin(Bin ***grid, Particle *particle, int particleSize, float h, int gridSizeX, int gridSizeY, int gridSizeZ);
void initialize_fog(Fog *fog, Point position);


#ifdef __cplusplus
    }
#endif

