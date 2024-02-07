// NimotsuKun1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <conio.h>

const char initializeMap[] = "\
########\n\
# .. p #\n\
# oo   #\n\
#      #\n\
########";

int mapWidth = 8; // col
int mapHeight = 5; // row

enum Object {
    OBJ_SPACE,      // 
    OBJ_WALL,       // #
    OBJ_PLAYER,     // p
    OBJ_BOX,        // o
    OBJ_GOAL,       // .
    OBJ_BOX_GOAL,   // O
    OBJ_PLAYER_GOAL // P
};

enum Direction {
    DIR_UP,         // up = 0
    DIR_RIGHT,      // right = 1
    DIR_DOWN,       // down = 2
    DIR_LEFT        // left = 3
};

int directions[4][2] = 
{
    {-1, 0},         // up
    {0, 1},         // right
    {1, 0},        // down
    {0, -1}         // left
};

char getSymbol(Object symbol);
void initializeGame(Object**& myMap, const char* initializeMap, int mapWidth, int mapHeight, int& playerX, int& playerY);
void deleteTheMap(Object**& myMap, int mapHeight);
bool checkGameOver(Object** myMap, int mapWidth, int mapHeight);
bool movePlayer(Object** myMap, int mapWidth, int mapHeight, int curr_x, int curr_y, int des_x, int des_y);
bool moveBox(Object** myMap, int mapWidth, int mapHeight, int curr_x, int curr_y, int des_x, int des_y);
void getInput(Direction& direction);
void updateGame(Object** myMap, int mapWidth, int mapHeight, Direction direction, int& playerX, int& PlayerY);
void draw(Object** myMap, int mapWidth, int mapHeight);

int main()
{
    Object** myMap;
    int playerX(0);
    int playerY(0);
    Direction direction;

    initializeGame(myMap, initializeMap, mapWidth, mapHeight, playerX, playerY);
    // draw(myMap, mapWidth, mapHeight);

    while (true) {
        system("cls");
        draw(myMap, mapWidth, mapHeight);
        if (checkGameOver(myMap, mapWidth, mapHeight)) {
            std::cout << "Congratulations! You win the game." << std::endl;
            break;
        }
        getInput(direction);

        updateGame(myMap, mapWidth, mapHeight, direction, playerX, playerY);
    }
    
    deleteTheMap(myMap, mapHeight);


    system("pause");
}

Object getObject(char object)
{
    switch (object)
    {
    case ' ':
        return OBJ_SPACE;
    case '#':
        return OBJ_WALL;
    case 'p':
        return OBJ_PLAYER;
    case 'o':
        return OBJ_BOX;
    case '.':
        return OBJ_GOAL;
    case 'O':
        return OBJ_BOX_GOAL;
    case 'P':
        return OBJ_PLAYER_GOAL;
    default:
        break;
    }

    return OBJ_WALL;
}

char getSymbol(Object symbol)
{
    switch (symbol)
    {
    case OBJ_SPACE:
        return ' ';
    case OBJ_WALL:
        return '#';
    case OBJ_PLAYER:
        return 'p';
    case OBJ_BOX:
        return 'o';
    case OBJ_GOAL:
        return '.';
    case OBJ_BOX_GOAL:
        return 'O';
    case OBJ_PLAYER_GOAL:
        return 'P';
    default:
        break;
    }

    return 0;
}

//void initializeGameWithTheFile(Object**& myMap)

void initializeGame(Object**& myMap, const char* initializeMap, int mapWidth, int mapHeight, int& playerX, int& playerY)
{
    // allocate the map 
    myMap = new Object * [mapHeight];

    for (int i = 0; i < mapHeight; i++) {
        myMap[i] = new Object[mapWidth];
    }

    int flag = 0;
    // initialize the map
    for (int i = 0; i < mapHeight; i++) {
        for (int j = 0; j < mapWidth; j++) {
            myMap[i][j] = getObject(initializeMap[flag++]);
            if (myMap[i][j] == OBJ_PLAYER) {
                playerX = i;
                playerY = j;
            }
        }
        flag++;
        // TODO
        // error: flag is out of range .
    }
}

void deleteTheMap(Object**& myMap, int mapHeight)
{
    for (int i = 0; i < mapHeight; i++) {
        delete[] myMap[i];
    }
    delete[] myMap;

    myMap = nullptr;
}

bool checkGameOver(Object** myMap, int mapWidth, int mapHeight)
{
    for (int i = 0; i < mapHeight; i++) {
        for (int j = 0; j < mapWidth; j++) {
            if (myMap[i][j] == OBJ_BOX || myMap[i][j] == OBJ_GOAL) {
                return false;
            }
        }
    }

    return true;
}

bool movePlayer(Object** myMap, int mapWidth, int mapHeight, int curr_x, int curr_y, int des_x, int des_y)
{
    // the box has moved if it exists
    if (des_x < 0 || des_y < 0 || des_x >= mapHeight || des_y >= mapWidth) {
        // TODO: show some error messages
        return false;
    }
    if ((myMap[curr_x][curr_y] != OBJ_PLAYER && myMap[curr_x][curr_y] != OBJ_PLAYER_GOAL) || 
        ((myMap[des_x][des_y] != OBJ_SPACE && myMap[des_x][des_y] != OBJ_GOAL))) {
        return false; // current object is not player and cannot move player
    }
    
    if (myMap[des_x][des_y] == OBJ_SPACE) {
        myMap[des_x][des_y] = OBJ_PLAYER;
    }
    else if (myMap[des_x][des_y] == OBJ_GOAL) {
        myMap[des_x][des_y] = OBJ_PLAYER_GOAL;
    }

    if (myMap[curr_x][curr_y] == OBJ_PLAYER) {
        myMap[curr_x][curr_y] = OBJ_SPACE;
    }
    else if (myMap[curr_x][curr_y] == OBJ_PLAYER_GOAL) {
        myMap[curr_x][curr_y] = OBJ_GOAL;
    }
    
    return true;
}

// makesure myMap[curr_x][curr_y] is the box
bool moveBox(Object** myMap, int mapWidth, int mapHeight, int curr_x, int curr_y, int des_x, int des_y)
{
    if (des_x < 0 || des_y < 0 || des_x >= mapHeight || des_y >= mapWidth) {
        return false;
    }

    if ((myMap[curr_x][curr_y] != OBJ_BOX && myMap[curr_x][curr_y] != OBJ_BOX_GOAL) || 
        (myMap[des_x][des_y] != OBJ_SPACE && myMap[des_x][des_y] != OBJ_GOAL)) {
        return false; // cannot move the box
    }

    if (myMap[des_x][des_y] == OBJ_SPACE) {
        myMap[des_x][des_y] = OBJ_BOX;
    }
    else if (myMap[des_x][des_y] == OBJ_GOAL) {
        myMap[des_x][des_y] = OBJ_BOX_GOAL;
    }

    if (myMap[curr_x][curr_y] == OBJ_BOX) {
        myMap[curr_x][curr_y] = OBJ_SPACE;
    }
    else if (myMap[curr_x][curr_y] == OBJ_BOX_GOAL) {
        myMap[curr_x][curr_y] = OBJ_GOAL;
    }

    //printf("move box: \n");
    return true;
}

void getInput(Direction& direction)
{
    char flag;
    
    while (true) {
        flag = _getch();
        if (flag == 'A' || flag == 'a') {
            direction = DIR_LEFT;
            break;
        }
        else if (flag == 'D' || flag == 'd') {
            direction = DIR_RIGHT;
            break;
        }
        else if (flag == 'W' || flag == 'w') {
            direction = DIR_UP;
            break;
        }
        else if (flag == 'S' || flag == 's') {
            direction = DIR_DOWN;
            break;
        }
    }

    //std::cout << direction << std::endl;
}

void updateGame(Object** myMap, int mapWidth, int mapHeight, Direction direction, int& playerX, int& PlayerY)
{
    int currX(playerX), currY(PlayerY);
    int nextX = playerX + directions[direction][0];
    int nextY = PlayerY + directions[direction][1];

    if (myMap[nextX][nextY] == OBJ_BOX || myMap[nextX][nextY] == OBJ_BOX_GOAL) {
        int boxNextX = nextX + directions[direction][0];
        int boxNextY = nextY + directions[direction][1];
        if (!moveBox(myMap, mapWidth, mapHeight, nextX, nextY, boxNextX, boxNextY)) {
            return;
        }
    }
    if (movePlayer(myMap, mapWidth, mapHeight, currX, currY, nextX, nextY)) {
        playerX = nextX;
        PlayerY = nextY;
    }
}


void draw(Object** myMap, int mapWidth, int mapHeight)
{
    for (int i = 0; i < mapHeight; i++) {
        for (int j = 0; j < mapWidth; j++) {
            std::cout << getSymbol(myMap[i][j]);
        }
        std::cout << std::endl;
    }
}
