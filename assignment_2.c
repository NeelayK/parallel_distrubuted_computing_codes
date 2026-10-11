#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

#define VEHICLES 1000
#define LANES 10
#define TIMESTEPS 1000

#define MAX_VEL 30
#define MAX_STARTING_POSITION 30
#define SAFE_DISTANCE 20
#define BRAKE_ACC 5

#define GPU_CHUNKS 10
#define CHUNK_SIZE (VEHICLES / GPU_CHUNKS)
#define GPU_DELAY 0.00002

#define PRINT 5

struct Vehicle {
    int id;
    unsigned int position;
    unsigned int velocity;
    short int lane;
};

void init_car(struct Vehicle *v, int id, int position,int velocity, int lane) {
    if (v == NULL)
        return;

    v->id = id;
    v->position = position;
    v->velocity = velocity;
    v->lane = lane;
}

void update_velocity(struct Vehicle *v, int acceleration) {
    if (v == NULL)
        return;

    unsigned int new_velocity = v->velocity + acceleration;

    if (new_velocity < 0)
        new_velocity = 0;

    if (new_velocity > MAX_VEL)
        new_velocity = MAX_VEL;

    v->velocity = new_velocity;
}

void update_position(struct Vehicle *v) {
    if (v == NULL)
        return;

    v->position += v->velocity;
}

void serial() {
    struct Vehicle vehicles[VEHICLES];

    int lane_pos[LANES] = {0};

    srand((unsigned int)time(NULL));
    int collision = 0;
    for (int i = 0; i < VEHICLES; i++) {
        int lane = rand() % LANES;

        lane_pos[lane] += rand() % (MAX_STARTING_POSITION + 1);

        int position = lane_pos[lane];
        int velocity = rand() % (MAX_VEL + 1);

        init_car(&vehicles[i], i, position, velocity, lane);
    }

    double start = omp_get_wtime();

    for (int t = 0; t < TIMESTEPS; t++) {

        for (int i = 0; i < VEHICLES; i++) {

            int acceleration = rand() % 5 - BRAKE_ACC;

            for (int j = 0; j < VEHICLES; j++) {
                if (i == j)
                    continue;

                if (vehicles[i].lane == vehicles[j].lane &&
                    vehicles[j].position > vehicles[i].position &&
                    vehicles[j].position - vehicles[i].position
                        <= SAFE_DISTANCE) {

                    acceleration = -BRAKE_ACC;
                    break;
                }
                if (i!=j && vehicles[i].position==vehicles[j].position){
                    collision++;
                }
            }

            update_velocity(&vehicles[i], acceleration);

            update_position(&vehicles[i]);
        }
    }

    double end = omp_get_wtime();

    double elapsed = end - start;

    printf("Serial Traffic Simulation\n");
    printf("-------------------------\n");
    printf("Vehicles : %d\n", VEHICLES);
    printf("Lanes : %d\n", LANES);
    printf("Timestamp : %d\n", TIMESTEPS);
    printf("Execution Time : %.6f seconds\n", elapsed);
    printf("Collisions : %d\n", collision);
 
    printf("\nVehicle states (first n):\n");
    printf("ID\tLane\tPosition\tVelocity\n");

    for (int i = 0; i < PRINT; i++) {
        printf("%d\t%d\t%u\t\t%u\n",
               vehicles[i].id,
               vehicles[i].lane,
               vehicles[i].position,
               vehicles[i].velocity);
    }

    return;
}

void parallel() {
    struct Vehicle vehicles[VEHICLES];

    int lane_pos[LANES] = {0};

    srand((unsigned int)time(NULL));
    int collisions = 0;
    for (int i = 0; i < VEHICLES; i++) {
        int lane = rand() % LANES;

        lane_pos[lane] += rand() % (MAX_STARTING_POSITION + 1);

        int position = lane_pos[lane];
        int velocity = rand() % (MAX_VEL + 1);

        init_car(&vehicles[i], i, position, velocity, lane);
    }

    double start = omp_get_wtime();

    for (int t = 0; t < TIMESTEPS; t++) {
        #pragma omp parallel for
        for (int i = 0; i < VEHICLES; i++) {

            int acceleration = rand() % 5 - BRAKE_ACC;

            for (int j = 0; j < VEHICLES; j++) {
                if (i == j)
                    continue;

                if (vehicles[i].lane == vehicles[j].lane &&
                    vehicles[j].position > vehicles[i].position &&
                    vehicles[j].position - vehicles[i].position
                        <= SAFE_DISTANCE) {

                    acceleration = -BRAKE_ACC;
                    break;
                }

                if (i!=j && vehicles[i].position==vehicles[j].position){
                    collisions++;
                }

            }

            update_velocity(&vehicles[i], acceleration);

            update_position(&vehicles[i]);
        }
    }

    double end = omp_get_wtime();

    double elapsed = end - start;

    printf("Parallel Traffic Simulation\n");
    printf("---------------------------\n");
    printf("Vehicles : %d\n", VEHICLES);
    printf("Lanes : %d\n", LANES);
    printf("Timestamp : %d\n", TIMESTEPS);
    printf("Execution Time : %.6f seconds\n", elapsed);
    printf("Collisions : %d\n", collisions);
 
    printf("\nVehicle states (first n):\n");
    printf("ID\tLane\tPosition\tVelocity\n");

    for (int i = 0; i < PRINT; i++) {
        printf("%d\t%d\t%u\t\t%u\n",
               vehicles[i].id,
               vehicles[i].lane,
               vehicles[i].position,
               vehicles[i].velocity);
    }

    return;
}

void fake_gpu_delay() {
    double wait_start = omp_get_wtime();
    while (omp_get_wtime() - wait_start < GPU_DELAY) {

    }
}

void gpu() {
    struct Vehicle vehicles[VEHICLES];

    int lane_pos[LANES] = {0};

    srand((unsigned int)time(NULL));
    int collisions = 0;
    for (int i = 0; i < VEHICLES; i++) {
        int lane = rand() % LANES;

        lane_pos[lane] += rand() % (MAX_STARTING_POSITION + 1);

        int position = lane_pos[lane];
        int velocity = rand() % (MAX_VEL + 1);

        init_car(&vehicles[i], i, position, velocity, lane);
    }

    double start = omp_get_wtime();

    for (int t = 0; t < TIMESTEPS; t++) {

        for (int c = 0; c < GPU_CHUNKS; c++) {

            int first = c * CHUNK_SIZE;
            int last = first + CHUNK_SIZE;

            fake_gpu_delay();

            #pragma omp parallel for reduction(+:collisions)
            for (int i = first; i < last; i++) {

                int acceleration = rand() % 5 - BRAKE_ACC;

                for (int j = 0; j < VEHICLES; j++) {
                    if (i == j)
                        continue;

                    if (vehicles[i].lane == vehicles[j].lane &&
                        vehicles[j].position > vehicles[i].position &&
                        vehicles[j].position - vehicles[i].position
                            <= SAFE_DISTANCE) {

                        acceleration = -BRAKE_ACC;
                        break;
                    }

                    if (i!=j && vehicles[i].position==vehicles[j].position){
                        collisions++;
                    }
                }

                update_velocity(&vehicles[i], acceleration);

                update_position(&vehicles[i]);
            }
        }
    }

    double end = omp_get_wtime();

    double elapsed = end - start;

    printf("Fake GPU Traffic Simulation\n");
    printf("---------------------------\n");
    printf("Vehicles : %d\n", VEHICLES);
    printf("Lanes : %d\n", LANES);
    printf("Timestamp : %d\n", TIMESTEPS);
    printf("Chunks : %d (size %d)\n", GPU_CHUNKS, CHUNK_SIZE);
    printf("Execution Time : %.6f seconds\n", elapsed);
    printf("Collisions : %d\n", collisions);
 
    printf("\nVehicle states (first n):\n");
    printf("ID\tLane\tPosition\tVelocity\n");

    for (int i = 0; i < PRINT; i++) {
        printf("%d\t%d\t%u\t\t%u\n",
               vehicles[i].id,
               vehicles[i].lane,
               vehicles[i].position,
               vehicles[i].velocity);
    }

    return;
}

void main(){
    for(int i=1;i<=16;i=i*2){
    printf("--------------Executing with %d Threads-----------------\n",i);
    omp_set_num_threads(i);
    serial();
    parallel();
    gpu();
    printf("--------------------------------------------------------\n\n");
}
    return;
}