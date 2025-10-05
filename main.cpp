// cc main.cpp -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

#include "raylib.h"
#include <cstdlib>
#include <vector>
#include <iostream>

#define MAP_SIZE 60

//function declaration
void drawMap(int [0][MAP_SIZE], std::vector<Vector2>);
void updateMap(int [0][MAP_SIZE], std::vector<Vector2> * path);
void initMapBorders(int map[MAP_SIZE][MAP_SIZE]);
bool check_touch(int map[MAP_SIZE][MAP_SIZE], int x, int y, int dir);
void logPath(std::vector<Vector2>);
std::vector<int> rand_vec(int);

int main(void)
{
    //setup window
    const int screenWidth = 1920;
    const int screenHeight = 1080;
    InitWindow(screenWidth, screenHeight, "raylib game");
    SetTargetFPS(60);

    //setup game resources
    int map[MAP_SIZE][MAP_SIZE] = {0};
    initMapBorders(map);
    std::vector<Vector2> path = {(Vector2){1, 1}};

    //game loop
    while (!WindowShouldClose())
    {
        updateMap(map, &path);
        //logPath(path);

        //draw
        BeginDrawing();
        ClearBackground(DARKGRAY);

        drawMap(map, path);

        EndMode2D();

        EndDrawing();
    }

    CloseWindow();        // Close window and OpenGL context

    return 0;
}

//draw

void drawMap(int array[][MAP_SIZE], std::vector<Vector2> path){
    float cell_size = GetScreenHeight() / MAP_SIZE;
    int map_width = (MAP_SIZE * cell_size) / 2;

    for (int y = 0; y <+ MAP_SIZE; y++){
        for (int x = 0; x <+ MAP_SIZE; x++){
            Rectangle rect = {(float)x*cell_size+GetScreenWidth()/2-map_width, (float)y*cell_size, cell_size, cell_size};

            DrawRectangleRec(rect, BLACK);
            if (array[y][x] == 1) DrawRectangleRec(rect, GRAY);
        }
    }
    if (path.size() == 0) return;

    //draw all not backtracked cells
    for (Vector2 i: path){
        Rectangle rect = {(float)i.x*cell_size + GetScreenWidth()/2-map_width, (float)i.y*cell_size, cell_size, cell_size};
        DrawRectangleRec(rect, BLUE);
        if (i.x == path.back().x && i.y == path.back().y){
            DrawRectangleRec(rect, GREEN);
        }
    }
}

//update

void updateMap(int map[][MAP_SIZE], std::vector<Vector2> *path){
    // 0 left 1 up 2 right 3 down
    Vector2 last = path -> back();

    std::vector<int> dirs = rand_vec(4);
    
    for (int dir: dirs){
        if (dir == 0){
            //right
            if (last.x + 1 < MAP_SIZE && map[(int)last.y][(int)last.x + 1] == 0){
                if (!check_touch(map, last.x + 1, last.y, dir)){
                    map[(int)last.y][(int)last.x + 1] = 1;
                    path -> push_back((Vector2{last.x + 1, last.y}));
                    return;
                }
            }
        }
        if (dir == 1){
            //left
            if (last.x - 1 >= 0 && map[(int)last.y][(int)last.x -1] == 0){
                if (!check_touch(map, last.x - 1, last.y, dir)){
                    map[(int)last.y][(int)last.x - 1] = 1;
                    path -> push_back((Vector2{last.x - 1, last.y}));
                    return;
                }
            }
        }
        if (dir == 2){
            //top
            if (last.y - 1 >= 0 && map[(int)last.y - 1][(int)last.x] == 0){
                if (!check_touch(map, last.x, last.y - 1, dir)){
                    map[(int)last.y - 1][(int)last.x] = 1;
                    path -> push_back((Vector2{last.x, last.y - 1}));
                    return;
                }
            }
        }
        if (dir == 3){
            //down
            if (last.y + 1 < MAP_SIZE && map[(int)last.y + 1][(int)last.x] == 0){
                if (!check_touch(map, last.x, last.y + 1, dir)){
                    map[(int)last.y + 1][(int)last.x] = 1;
                    path -> push_back((Vector2{last.x, last.y + 1}));
                    return;
                }
            }
        }
    }
    //if we backtracked till the end this means we we're in all places
    if (path -> size() == 0) return;

    //backtrack
    else { path -> pop_back(); }
}

bool check_touch(int map[MAP_SIZE][MAP_SIZE], int x, int y, int dir){
    if (dir == 0){
        if (map[y][x + 1] == 1) return true;       // right
        else if (map[y - 1][x + 1] == 1) return true;   // top right
        else if (map[y + 1][x + 1] == 1) return true;   // down right
        else if (map[y - 1][x] == 1) return true;   // top
        else if (map[y + 1][x] == 1) return true;   // down
    }
    else if (dir == 1){
        if (map[y + 1][x - 1] == 1) return true;   // down left
        else if (map[y][x - 1] == 1) return true;       // left
        else if (map[y - 1][x - 1] == 1) return true;   // top left
        else if (map[y - 1][x] == 1) return true;   // top
        else if (map[y + 1][x] == 1) return true;   // down
    }
    else if (dir == 2){
        if (map[y - 1][x] == 1) return true;       // top
        else if (map[y - 1][x + 1] == 1) return true;   // top right
        else if (map[y - 1][x - 1] == 1) return true;   // top left
        else if (map[y][x - 1] == 1) return true;   // left
        else if (map[y][x + 1] == 1) return true;   // right
    }
    else if (dir == 3){
        if (map[y + 1][x + 1] == 1) return true;   // down right
        else if (map[y + 1][x] == 1) return true;       // down
        else if (map[y + 1][x - 1] == 1) return true;   // down left
        else if (map[y][x - 1] == 1) return true;   // left
        else if (map[y][x + 1] == 1) return true;   // right
    }
    return false;
}

void initMapBorders(int map[MAP_SIZE][MAP_SIZE]){
    for (int i = 0; i < MAP_SIZE; i++){
        map[i][0] = 2;
        map[0][i] = 2;
        map[i][MAP_SIZE - 1] = 2;
        map[MAP_SIZE - 1][i] = 2;
    }
}

std::vector<int> rand_vec(int size){
    //create vector with specified size
    std::vector<int> normal_vec = {};
    for (int i = 0; i < size; i++){
        normal_vec.push_back(i); 
    }
    //shuffle
    for (int i = 0; i < size; i++){
        int j = rand() % (0, size - 1);
        std::swap(normal_vec[i], normal_vec[j]);
    }
    return normal_vec;
}

void logPath(std::vector<Vector2> path){
    for (Vector2 x: path){
        std::cout << x.y;
        std::cout << x.x << std::endl;
        
    }
}

