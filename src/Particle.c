//
// Created by DOTAFS on 2024/8/5.
//
#include "Particle.h"

#include "fsMath.h"



void bin_init(Bin *bin, int capacity) {
    bin->elements = (Particle**)malloc(capacity * sizeof(Particle*));
    bin->size = 0;
    bin->capacity = capacity;
}

void bin_add(Bin *bin, Particle *particle) {
    if (bin->size < bin->capacity) {
        bin->elements[bin->size++] = particle;
    } else {
        // 尝试增加内存
        Particle **new_elements = (Particle **)realloc(bin->elements, 2 * bin->capacity * sizeof(Particle *));
        if (new_elements == NULL) {
            fprintf(stderr, "Failed to reallocate memory for bin elements\n");
            exit(EXIT_FAILURE);
        }
        bin->elements = new_elements;
        bin->capacity *= 2;
        bin->elements[bin->size++] = particle;
    }
}


void initialize_fog(Fog *fog, Point position) {
    fog->position.val[0]  = position.val[0];
    fog->position.val[1]  = position.val[1];
    fog->position.val[2]  = position.val[2];
    fog->position.val[3]  = position.val[3];
    fog->alpha = 0.1f;
}



void bin_free(Bin *bin) {
    free(bin->elements);
}


void bin_clear(Bin *bin) {
    bin->size = 0;
}

void initialize_particle(Particle *particle, Point position, int maxNeighbours) {
    particle->position.val[0]  = position.val[0];
    particle->position.val[1]  = position.val[1];
    particle->position.val[2]  = position.val[2];
    particle->position.val[3]  = position.val[3];

    particle->pred_position.val[0]  = position.val[0];
    particle->pred_position.val[1]  = position.val[1];
    particle->pred_position.val[2]  = position.val[2];
    particle->pred_position.val[3]  = position.val[3];

    particle->buffer_position.val[0]  = 0;
    particle->buffer_position.val[1]  = 0;
    particle->buffer_position.val[2]  = 0;
    particle->buffer_position.val[3]  = 0;


    particle->mass = 0.03f;
    particle->oneTimeForceAlive = 0;
    particle->neighboursSize = 0;
    particle->neighboursCapacity = maxNeighbours;
    // 分配初始内存给 neighbours 数组
    particle->neighbours = (Particle**)malloc(maxNeighbours * sizeof(Particle*));
    if (particle->neighbours == NULL) {
        fprintf(stderr, "Failed to allocate memory for neighbours\n");
        exit(EXIT_FAILURE);
    }
}


Particle* initialize_particles(int xCount, int yCount, int zCount, int maxNeighbours) {
    int particleCount = xCount * yCount * zCount;

    // 动态分配 particles 数组的内存
    Particle *particles = (Particle*)malloc(particleCount * sizeof(Particle));
    if (particles == NULL) {
        fprintf(stderr, "Failed to allocate memory for particles\n");
        exit(EXIT_FAILURE);
    }

    // 计算粒子在每个轴上的间距
    double RANGE_X_MAX_bias = RANGE_X_MAX - 0.5f;
    double RANGE_X_MIN_bias = RANGE_X_MIN + 0.5f;
    double RANGE_Y_MAX_bias = RANGE_Y_MAX - 0.5f;
    double RANGE_Y_MIN_bias = RANGE_Y_MIN + 0.5f;
    double RANGE_Z_MAX_bias = RANGE_Z_MAX - 0.5f;
    double RANGE_Z_MIN_bias = RANGE_Z_MIN + 0.5f;

    double stepX = (RANGE_X_MAX_bias - RANGE_X_MIN_bias) / (double)(xCount - 1);
    double stepY = (RANGE_Y_MAX_bias - RANGE_Y_MIN_bias) / (double)(yCount - 1);
    double stepZ = (RANGE_Z_MAX_bias - RANGE_Z_MIN_bias) / (double)(zCount - 1);

    // 初始化粒子
    int index = 0;
    for (int i = 0; i < xCount; ++i) {
        for (int j = 0; j < yCount; ++j) {
            for (int k = 0; k < zCount; ++k) {
                Point position;
                position.val[0] = RANGE_X_MIN_bias + i * stepX;
                position.val[1] = RANGE_Y_MIN_bias + j * stepY;
                position.val[2] = RANGE_Z_MIN_bias + k * stepZ;
                position.val[3] = 1.0f;

                initialize_particle(&particles[index], position, maxNeighbours);
                particles[index].id = index;

             //   printf("Particle %d: x=%f, y=%f, z=%f\n", index, position.val[0], position.val[1], position.val[2]);
                if(position.val[2] < RANGE_Z_MIN_bias || position.val[2] > RANGE_Z_MAX_bias)
                    continue;
                index++;
            }
        }
    }

    return particles;
}



Bin*** initialize_grid(int initialBinCapacity) {

    Bin ***grid = (Bin***)malloc((simSizeX) * sizeof(Bin**));
    if (grid == NULL) {
        fprintf(stderr, "Failed to allocate memory for grid\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < (simSizeX); i++) {
        grid[i] = (Bin**)malloc((simSizeY) * sizeof(Bin*));
        if (grid[i] == NULL) {
            fprintf(stderr, "Failed to allocate memory for grid row\n");
            // 释放已经分配的内存
            for (int x = 0; x < i; x++) {
                free(grid[x]);
            }
            free(grid);
            exit(EXIT_FAILURE);
        }

        for (int j = 0; j < (simSizeY); j++) {
            grid[i][j] = (Bin*)malloc((simSizeZ) * sizeof(Bin));
            if (grid[i][j] == NULL) {
                fprintf(stderr, "Failed to allocate memory for grid cell\n");
                // 释放已经分配的内存
                for (int y = 0; y < j; y++) {
                    free(grid[i][y]);
                }
                for (int x = 0; x <= i; x++) {
                    free(grid[x]);
                }
                free(grid);
                exit(EXIT_FAILURE);
            }

            for (int k = 0; k < (simSizeZ); k++) {
                bin_init(&grid[i][j][k], initialBinCapacity);
            }
        }
    }

    return grid;
}




void reset_grid(Bin ***grid) {
    for (int i = 0; i < simSizeX; i++) {
        for (int j = 0; j < simSizeY; j++) {
            for (int k = 0; k < simSizeZ; k++) {
                grid[i][j][k].size = 0;
                // 如果需要，可以选择清空元素数组中的指针，这取决于你如何管理内存
                for (int p = 0; p < grid[i][j][k].capacity; p++) {
                    grid[i][j][k].elements[p] = NULL;
                }
            }
        }
    }
}



void add_particle_to_bin(Bin ***grid, Particle *particle) {
    int gridX = (int)floor((particle->pred_position.val[0] - RANGE_X_MIN) / RANGE_H);
    int gridY = (int)floor((particle->pred_position.val[1] - RANGE_Y_MIN) / RANGE_H);
    int gridZ = (int)floor((particle->pred_position.val[2] - RANGE_Z_MIN) / RANGE_H);
    if (gridX >= 0 && gridX <= simSizeX && gridY >= 0 && gridY <= simSizeY && gridZ >= 0 && gridZ <= simSizeZ) {
        Bin *bin = &grid[gridX][gridY][gridZ];

        if (bin->size >= bin->capacity) {
            Particle **new_elements = (Particle**)realloc(bin->elements, 2 * bin->capacity * sizeof(Particle*));
            if (new_elements == NULL) {
                fprintf(stderr, "Failed to reallocate memory for bin elements\n");
                exit(EXIT_FAILURE);
            }
            bin->elements = new_elements;
            bin->capacity *= 2;
        }
        bin->elements[bin->size++] = particle;
    }
    else
        printf("\n adding particle to bin error.");
}

void add_all_particles_to_bin(Bin ***grid, Particle *particle, int particleSize) {
    for (int i = 0; i < particleSize; ++i) {
        add_particle_to_bin(grid,&particle[i]);
    }
}

void free_particle(Particle *particle) {
    free(&particle->neighbours);
}

Vector reflect(Vector v, Vector n, double damping) {
    double dot = vector_dot(&v, &n);
    Vector scaled_normal;
    Vector result;
    vector_scale(&n,2*dot,&scaled_normal);
    Vector reflection;
    vector_subtract(&v, &scaled_normal,&reflection);
    vector_scale(&reflection,damping,&result);
    return result;
}

// 如果碰到边界，重新给出新的pred_position, 更新根据normal算出新的反弹方向
void apply_boundary_constraint(Particle *particle, double damping, double dt) {
    if (particle->pred_position.val[0] < RANGE_X_MIN) {
        Vector normal = {1.0, 0.0, 0.0, 0.0};
        // 这里假设dt足够小，所以不用计算pre_position到碰撞点的距离，直接把pre_position移动到反弹点。
        particle->v = reflect(particle->v, normal, damping);  // 反弹并衰减
    } else if (particle->pred_position.val[0] > RANGE_X_MAX) {
        Vector normal = {-1.0, 0.0, 0.0, 0.0};
        particle->v = reflect(particle->v, normal, damping);  // 反弹并衰减
    }

    if (particle->pred_position.val[1] < RANGE_Y_MIN) {
        Vector normal = {0.0, 1.0, 0.0, 0.0};
        particle->v = reflect(particle->v, normal, damping);  // 反弹并衰减
    } else if (particle->pred_position.val[1] > RANGE_Y_MAX) {
        Vector normal = {0.0, -1.0, 0.0, 0.0};
        particle->v = reflect(particle->v, normal, damping);  // 反弹并衰减
    }

    if (particle->pred_position.val[2] < RANGE_Z_MIN) {
        Vector normal = {0.0, 0.0, 1.0, 0.0};
        particle->v = reflect(particle->v, normal, damping);  // 反弹并衰减
    } else if (particle->pred_position.val[2] > RANGE_Z_MAX) {
        particle->pred_position.val[2] = RANGE_Z_MAX;
        Vector normal = {0.0, 0.0, -1.0, 0.0};
        particle->v = reflect(particle->v, normal, damping);  // 反弹并衰减
    }

    // 为了方便计算假设从现在的点重新发出反弹预测坐标
    particle->pred_position.val[0] = particle->position.val[0] + dt * particle->v.val[0];
    particle->pred_position.val[1] = particle->position.val[1] + dt * particle->v.val[1];
    particle->pred_position.val[2] = particle->position.val[2] + dt * particle->v.val[2];
}




void calculate_gradient(Particle *particle1, Particle *particle2, Vector *gradient) {

    // normal between two particles
    gradient->val[0] = particle1->pred_position.val[0] - particle2->pred_position.val[0];
    gradient->val[1] = particle1->pred_position.val[1] - particle2->pred_position.val[1];
    gradient->val[2] = particle1->pred_position.val[2] - particle2->pred_position.val[2];
    gradient->val[3] = 0;
}

// 对于每一个粒子，计算他的neighours和拉格朗日系数lambda, 计算的是违背了我们目标位置的程度
void calculate_lambda(Particle *particle, double restDensity) {
    double C = 0.0f;
    double sumGradC2 = 0.0f;

    if(particle->neighboursSize == 0) {
        particle->lambda = 0;
        return;
    }

    // 计算密度约束 C(p)
    for (int i = 0; i < particle->neighboursSize; i++) {
        Particle *neighbour = particle->neighbours[i];
        Vector gradient = {0, 0, 0, 0};

        // 计算梯度
        calculate_gradient(particle, neighbour, &gradient);

        // 计算当前密度
        double dist = vector_length(&gradient);
        if (dist == 0)
            continue;

        // 累加约束
        double constraint = dist - restDensity;
        C += constraint;

        // 计算约束的梯度 (归一化的方向)
        vector_normalize(&gradient);
        double gradC2 = vector_dot(&gradient, &gradient);

        sumGradC2 +=  (2.0f / (neighbour->mass + 0.0001f)) * gradC2;
    }

    // 计算拉格朗日乘子 lambda
     particle->lambda = -C / sumGradC2;
}


// 位置预计修成量
void calculate_position_buffer(Particle *particle) {
    double stiffness = STIFFNESS;
    double lambda_i = particle->lambda;
    for (int i = 0; i < particle->neighboursSize ; i++) {
        // 只关注id小的作为particle，大的作为neighbour，别的全部跳过
        if(particle->id >= particle->neighbours[i]->id) continue;
        double lambda_j = particle->neighbours[i]->lambda;
        Particle *neighbour = particle->neighbours[i];
        Vector gradient = {0,0,0,0}; // 两个点之间的距离差

        // 计算两个粒子之间的当前距离向量和长度
        calculate_gradient(particle, neighbour, &gradient);

        // 计算约束的梯度 (归一化的方向) Cij
        vector_normalize(&gradient);

        if(vector_length(&gradient) <= 0)
            continue;

        Vector Cij = gradient;

        particle->buffer_position.val[0] -= -lambda_i * Cij.val[0] * stiffness;
        particle->buffer_position.val[1] -= -lambda_i * Cij.val[1] * stiffness;
        particle->buffer_position.val[2] -= -lambda_i * Cij.val[2] * stiffness;

        particle->neighbours[i]->buffer_position.val[0] -= lambda_j * Cij.val[0] * stiffness;
        particle->neighbours[i]->buffer_position.val[1] -= lambda_j * Cij.val[1] * stiffness;
        particle->neighbours[i]->buffer_position.val[2] -= lambda_j * Cij.val[2] * stiffness;
    }
}



void compute_external_forces(Particle *particle) {
    particle->externalForce.val[0] = 0.0;
    particle->externalForce.val[1] =0.0; // GRAVITY * particle->mass;
    particle->externalForce.val[2] = 0.0;
    if(particle->oneTimeForceAlive){
        particle->oneTimeForceAlive = 0;
        vector_add(&particle->externalForce,&particle->oneTimeForce,&particle->externalForce);
    }
}

void compute_internal_forces(Particle *particle) {

    for (int i = 0; i < particle->neighboursSize; i++) {
        Particle *neighbour = &particle->neighbours[i];
        // 假设这里有一个函数来计算相互作用力
    }
}



// before apply forces
void update_acceleration(Particle *particle) {
    particle->acceleration.val[0] = particle->externalForce.val[0] / particle->mass;
    particle->acceleration.val[1] = particle->externalForce.val[1] / particle->mass;
    particle->acceleration.val[2] = particle->externalForce.val[2] / particle->mass;
}


// 应用力并预测位置
void apply_forces_and_predict_position(Particle *particle, double deltaTime) {
    particle->v.val[0] += deltaTime * particle->acceleration.val[0];
    particle->v.val[1] += deltaTime * particle->acceleration.val[1];
    particle->v.val[2] += deltaTime * particle->acceleration.val[2];

    particle->pred_position.val[0] = particle->position.val[0] + deltaTime * particle->v.val[0];
    particle->pred_position.val[1] = particle->position.val[1] + deltaTime * particle->v.val[1];
    particle->pred_position.val[2] = particle->position.val[2] + deltaTime * particle->v.val[2];
}


// 更新速度和位置
void update_velocity_and_position(Particle *particle, double deltaTime) {
    particle->v.val[0] = (particle->pred_position.val[0] - particle->position.val[0]) / deltaTime;
    particle->v.val[1] = (particle->pred_position.val[1] - particle->position.val[1]) / deltaTime;
    particle->v.val[2] = (particle->pred_position.val[2] - particle->position.val[2]) / deltaTime;

    // 应用阻尼
    particle->v.val[0] *= DAMPING;
    particle->v.val[1] *= DAMPING;
    particle->v.val[2] *= DAMPING;

    particle->position = particle->pred_position;
}


void find_neighbours(Particle *particle, double h, Bin ***grid) {
    int gridX = (int)floor((particle->pred_position.val[0] - RANGE_X_MIN) / RANGE_H);
    int gridY = (int)floor((particle->pred_position.val[1] - RANGE_Y_MIN) / RANGE_H);
    int gridZ = (int)floor((particle->pred_position.val[2] - RANGE_Z_MIN) / RANGE_H);

    particle->neighboursSize = 0;

    // search all the around bins
    for (int i = gridX - 1; i <= gridX + 1; i++) {
        for (int j = gridY - 1; j <= gridY + 1; j++) {
            for (int k = gridZ - 1; k <= gridZ + 1; k++) {
                if (i >= 0 && i < simSizeX && j >= 0 && j < simSizeY && k >= 0 && k < simSizeZ) {
                    Bin *bin = &grid[i][j][k];
                    for (int p = 0; p < bin->size; p++) {
                        Particle *neighbour = bin->elements[p];
                        if(neighbour->id == particle->id) continue; // 剔除自己
                        double dx = particle->pred_position.val[0] - neighbour->pred_position.val[0];
                        double dy = particle->pred_position.val[1]- neighbour->pred_position.val[1];
                        double dz = particle->pred_position.val[2] - neighbour->pred_position.val[2];
                        double distance = sqrt(dx * dx + dy * dy + dz * dz);
                        if (distance < RESTDISTANCE) {
                            // 检查是否需要扩容
                            if (particle->neighboursSize >= particle->neighboursCapacity) {
                                particle->neighboursCapacity *= 2;
                                Particle **new_neighbours = (Particle**)realloc(particle->neighbours, particle->neighboursCapacity * sizeof(Particle*));
                                if (new_neighbours == NULL) {
                                    fprintf(stderr, "Failed to reallocate memory for neighbours\n");
                                    exit(EXIT_FAILURE);
                                }
                                particle->neighbours = new_neighbours;
                            }
                            // 添加邻居
                            particle->neighbours[particle->neighboursSize++] = neighbour;
                        }
                    }
                }
            }
        }
    }
}


void solve_constraints(Particle *particles, int numParticles, double restDistance, double damping, int solverIterations, double deltaTime) {

        for (int i = 0; i < numParticles; i++) {
            calculate_lambda(&particles[i], restDistance);
        }
        for (int i = 0; i < numParticles; i++) {
            // 计算距离约束
            calculate_position_buffer(&particles[i]);
        }

        for (int i = 0; i < numParticles; i++) {
            // 预估位置如果不对，施加碰撞盒子的回弹，如果回弹应该改变predict position，
            if  (particles[i].pred_position.val[0] < RANGE_X_MIN || particles[i].pred_position.val[0] > RANGE_X_MAX ||
             particles[i].pred_position.val[1] < RANGE_Y_MIN ||  particles[i].pred_position.val[1] > RANGE_Y_MAX ||
             particles[i].pred_position.val[2] < RANGE_Z_MIN ||  particles[i].pred_position.val[2] > RANGE_Z_MAX) {
                apply_boundary_constraint(&particles[i], damping, deltaTime);
             }
        }
}


void update_all_particles(Particle *particles, int numParticles, int solverIterations, double deltaTime, Bin ***grid, double h) {
    double damping = 0.8f;
    double restDistance = RESTDISTANCE;
    for (int i = 0; i < numParticles; ++i) {
        // 应用力
        compute_external_forces(&particles[i]);
      //  compute_internal_forces(&particles[i]);
        // 通过外力计算加速度
        update_acceleration(&particles[i]);
        // 通过加速度计算predict位置，在这里应该计算
        apply_forces_and_predict_position(&particles[i], TIME_STEP);

        // 预估位置的地方寻找所有的neighbours
        find_neighbours(&particles[i], h, grid);
    }

    for (int iter = 0; iter < solverIterations; iter++) {
        // 判断约束
        solve_constraints(particles, numParticles, restDistance,damping,solverIterations,deltaTime);

        // 把buffer position移动到pred position里
        for (int i = 0; i < numParticles; ++i) {
            particles[i].pred_position.val[0] += particles[i].buffer_position.val[0];
            particles[i].pred_position.val[1] += particles[i].buffer_position.val[1];
            particles[i].pred_position.val[2] += particles[i].buffer_position.val[2];
            // 清空所有的buffer
            particles[i].buffer_position.val[0] = 0;
            particles[i].buffer_position.val[1] = 0;
            particles[i].buffer_position.val[2] = 0;
        }
    }
    for (int i = 0; i < numParticles; ++i) {
        // 更新当前速度，把预估位置变成现在的位置 --- 简而言之，移动
        if(particles[i].id == 0)
            continue;
        update_velocity_and_position(&particles[i], deltaTime);
    }
}
