#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>

#define SIZE 20
#define SLEEP_TIME 100000 // 100ms

typedef struct
{
    int grid[SIZE][SIZE];
    int previousGrid[SIZE][SIZE];
    pthread_mutex_t mutex;
    bool hasLife;
} GameState;

typedef struct
{
    int quadrant;
    int startRow;
    int endRow;
    int startCol;
    int endCol;
    GameState *state;
} ThreadData;

void printGrid(const GameState *state)
{
    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            printf("%c ", state->grid[i][j] ? '#' : '.');
        }
        printf("\n");
    }
    printf("\n");
}

int countNeighbors(const GameState *state, int row, int col)
{
    int count = 0;
    for (int i = -1; i <= 1; i++)
    {
        for (int j = -1; j <= 1; j++)
        {
            if (i == 0 && j == 0)
                continue;
            int newRow = row + i;
            int newCol = col + j;
            if (newRow >= 0 && newRow < SIZE && newCol >= 0 && newCol < SIZE)
            {
                count += state->grid[newRow][newCol];
            }
        }
    }
    return count;
}

void *simulateQuadrant(void *param)
{
    ThreadData *data = (ThreadData *)param;
    GameState *state = data->state;
    int newGrid[SIZE][SIZE];

    for (int i = data->startRow; i < data->endRow; i++)
    {
        for (int j = data->startCol; j < data->endCol; j++)
        {
            int neighbors = countNeighbors(state, i, j);
            if (state->grid[i][j] == 1)
            {
                newGrid[i][j] = (neighbors == 2 || neighbors == 3) ? 1 : 0;
            }
            else
            {
                newGrid[i][j] = (neighbors == 3) ? 1 : 0;
            }
        }
    }

    pthread_mutex_lock(&state->mutex);
    for (int i = data->startRow; i < data->endRow; i++)
    {
        for (int j = data->startCol; j < data->endCol; j++)
        {
            state->grid[i][j] = newGrid[i][j];
        }
    }
    pthread_mutex_unlock(&state->mutex);

    pthread_exit(0);
}

bool checkForLife(GameState *state)
{
    // Check if any life exists
    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            if (state->grid[i][j] == 1)
            {
                // Check if grid is different from previous grid
                for (int x = 0; x < SIZE; x++)
                {
                    for (int y = 0; y < SIZE; y++)
                    {
                        if (state->grid[x][y] != state->previousGrid[x][y])
                        {
                            return true; // Grid is different and has life
                        }
                    }
                }
                return false; // Grid is the same but has life
            }
        }
    }
    return false; // No life exists
}

int main()
{
    GameState state;
    pthread_mutex_init(&state.mutex, NULL);
    state.hasLife = true;

    pthread_t thread1;   /*thread identifier*/
    pthread_t thread2;   /*thread identifier*/
    pthread_t thread3;   /*thread identifier*/
    pthread_t thread4;   /*thread identifier*/
    pthread_attr_t attr; /*set of thread attributes */
    ThreadData threadData[4];

    // Initialize random grid
    srand(time(NULL));
    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            state.grid[i][j] = rand() % 2;
        }
    }

    pthread_attr_init(&attr); /* set the default attributes of the thread */
    int quadrantSize = SIZE / 2; // Divide grid into 4 quadrants

    for (int i = 0; i < 4; i++)
    {
        threadData[i].quadrant = i;
        threadData[i].startRow = (i / 2) * quadrantSize;
        threadData[i].endRow = threadData[i].startRow + quadrantSize;
        threadData[i].startCol = (i % 2) * quadrantSize;
        threadData[i].endCol = threadData[i].startCol + quadrantSize;
        threadData[i].state = &state;
    }

    int generation = 0;

    while (state.hasLife)
    {
        printf("Generation %d:\n", generation++);
        printGrid(&state);
        
        // Fixes issue of game never ending
        memcpy(state.previousGrid, state.grid, sizeof(state.grid));

        // Create individual threads
        pthread_create(&thread1, &attr, simulateQuadrant, &threadData[0]);
        pthread_create(&thread2, &attr, simulateQuadrant, &threadData[1]);
        pthread_create(&thread3, &attr, simulateQuadrant, &threadData[2]);
        pthread_create(&thread4, &attr, simulateQuadrant, &threadData[3]);

        // Wait for individual threads
        pthread_join(thread1, NULL);
        pthread_join(thread2, NULL);
        pthread_join(thread3, NULL);
        pthread_join(thread4, NULL);

        state.hasLife = checkForLife(&state);
        usleep(SLEEP_TIME);
    }

    printf("Simulation ended after %d generations\n", generation);
    printf("Final state:\n");
    printGrid(&state);
    pthread_mutex_destroy(&state.mutex);
    return 0;
}
