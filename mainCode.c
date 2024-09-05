#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include "raylib.h"

#define FRAME 60
#define SIZE_WINDOW 400
#define SQUARE_H 30
#define SQUARE_W 30
#define SPEED 6
#define ENEMY_SIZE 6
#define COLOR GREEN
#define BULLET_SIZE 6
#define MAX 10

signed int SQUARE_X = SIZE_WINDOW/2;
signed int SQUARE_Y = SIZE_WINDOW - SQUARE_H;
bool colledPlayerWithEnemy = false;
Sound bulletShootSound, collisionSound;
struct gameScore
{
    unsigned int score;
}score = {0};

typedef struct enemy
{
    Vector2 position;
    Vector2 size;
    signed int speed;
    bool active;
}ENEMY;

typedef struct bullet
{
    Vector2 position;
    Vector2 size;
    bool active;
    signed int speedY;
}BULLET;

void playerUpdate(void);
void exitWindow(void);
void enemyUpdate(ENEMY* listOfEnemies);
void bulletUpdate(BULLET* listOfBullet);
void collision(BULLET* listOfBullet, ENEMY* listOfEnemy);
char* convertIntToString(void);
void clearEnemyBullet(BULLET* listOfBullet, ENEMY* listofEnemy);


int main(void)
{
    srand(time(NULL));
    Vector2 buttonPosition_1 = {162.5, 175};
    Vector2 buttonSize_1 = {75, 50};
    Vector2 buttonPosition_2 = {162.5, 235};
    Vector2 buttonSize_2 = {75, 50};
    Rectangle Button_1 = {buttonPosition_1.x, buttonPosition_1.y, buttonSize_1.x, buttonSize_1.y};
    Rectangle Button_2 = {buttonPosition_2.x, buttonPosition_2.y, buttonSize_2.x, buttonSize_2.y};
    Color defaultColor_1 = GRAY, defaultColor_2 = GRAY;
    Color pressedColor = BLACK;
    Color mouseOnButtonColor = DARKBROWN;
    bool buttonPressed_1 = false, buttonPressed_2 = false;
    ENEMY listOfEnemies[ENEMY_SIZE];
    BULLET listOfBullet[BULLET_SIZE];
    InitWindow(SIZE_WINDOW, SIZE_WINDOW, "BASIC GAME");
    SetTargetFPS(FRAME);
    InitAudioDevice();
    bulletShootSound = LoadSound("gun-gunshot-02.wav");
    collisionSound = LoadSound("you-lose-game-sound-230514.mp3");
    while(!WindowShouldClose())
    {
        if(!buttonPressed_1)
        {
            Vector2 mousePosition = GetMousePosition();
            bool mouseCollideButton_1 = CheckCollisionPointRec(mousePosition, Button_1);
            bool mouseCollideButton_2 = CheckCollisionPointRec(mousePosition, Button_2);
            if(mouseCollideButton_1)
            {
                defaultColor_1 = (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) ? pressedColor : mouseOnButtonColor;
                if(IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
                    buttonPressed_1 = true;
            }
            else if(mouseCollideButton_2)
            {
                defaultColor_2 = (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) ? pressedColor : mouseOnButtonColor;
                if(IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
                    buttonPressed_2 = true; 
            }
            else
            {
                defaultColor_1 = GRAY;
                defaultColor_2 = GRAY;
            }
            BeginDrawing();
                ClearBackground(WHITE);
                DrawText("SKY DEFENDER GAME", 120, 140, 15, BLACK);
                DrawRectangle(Button_1.x, Button_1.y, Button_1.width, Button_1.height, defaultColor_1);
                DrawRectangle(Button_2.x, Button_2.y, Button_2.width, Button_2.height, defaultColor_2);
                DrawText("EXIT", Button_2.x+13, Button_2.y+10, 15, WHITE);
                DrawText("START", Button_1.x+10, Button_1.y+10, 15, WHITE);
                DrawText("CREATED BY : DALI FETHI ABDELLATIF", 50, 350, 15, BLACK);
                if(buttonPressed_2)
                    exitWindow();
            EndDrawing();
        }
        else
        {
            if(!colledPlayerWithEnemy)
            {
                playerUpdate();
                bulletUpdate(listOfBullet);
                enemyUpdate(listOfEnemies);
                collision(listOfBullet, listOfEnemies);
                BeginDrawing();
                    ClearBackground(WHITE);
                    DrawRectangle(SQUARE_X, SQUARE_Y, SQUARE_W, SQUARE_W, BLACK);
                    for(unsigned int i = 0; i < ENEMY_SIZE; ++i)
                    {
                        if(listOfEnemies[i].active)
                            DrawRectangle(listOfEnemies[i].position.x, listOfEnemies[i].position.y, listOfEnemies[i].size.x, listOfEnemies[i].size.y, COLOR);                }
                    for(unsigned int i = 0; i < BULLET_SIZE; ++i)
                    {
                        if(listOfBullet[i].active)
                            DrawRectangle(listOfBullet[i].position.x, listOfBullet[i].position.y, listOfBullet[i].size.x, listOfBullet[i].size.y, BLUE);
                    }
                EndDrawing();
            }
            else
            {
                char* scoreStr = convertIntToString();
                if(IsKeyPressed(KEY_ENTER))
                {
                    colledPlayerWithEnemy = false;
                    buttonPressed_1 = false;
                    buttonPressed_2 = false;
                    score.score = 0;
                    clearEnemyBullet(listOfBullet, listOfEnemies);
                    free(scoreStr);
                }
                BeginDrawing();
                    ClearBackground(WHITE);
                    DrawText("END GAME", 100, 100, 15, BLACK);
                    DrawText("YOUR SCORE : ", 100, 150, 15, BLACK);
                    DrawText(scoreStr, 230, 150, 18, BLACK);
                    DrawText("PRESS ENTER", 100, 200, 15, BLACK);
                EndDrawing();
            }
        }
    }
    CloseWindow();
    return 0;
}

void clearEnemyBullet(BULLET* listOfBullet, ENEMY* listofEnemy)
{
    for(unsigned int i = 0; i < BULLET_SIZE; ++i)
        listOfBullet[i].active = false;
    for(unsigned int i = 0; i < BULLET_SIZE; ++i)
        listofEnemy[i].active = false;
}

char* convertIntToString(void)
{
    signed int TOP = -1;
    unsigned int i = 0, number = score.score;
    unsigned char stack[MAX];
    char* str = calloc(MAX, sizeof(char));
    while(i < MAX)
        str[i++] = '\0';
    if(number == 0)
        str[0] = 0x30;
    else
    {
        i = 0;
        while(number != 0)
        {
            stack[++TOP] = (number % 10) + 0x30;
            number /= 10;
        }
        while(TOP != -1)
            str[i++] = stack[TOP--];
    }
    return str;
}
void collision(BULLET* listOfBullet, ENEMY* listOfEnemy)            // COLLISION FUNCTION BETWEEN BULLET AND ENEMY
{
    for(unsigned int i = 0; i < BULLET_SIZE; ++i)
    {
        if(listOfBullet[i].active)
        {
            for(unsigned int j = 0; j < ENEMY_SIZE; ++j)
            {
                if(listOfEnemy[i].active)
                {
                    if (listOfBullet[i].position.x + listOfBullet[i].size.x < listOfEnemy[j].position.x || listOfBullet[i].position.x > listOfEnemy[j].position.x + listOfEnemy[j].size.x ||
                        listOfBullet[i].position.y + listOfBullet[i].size.y < listOfEnemy[j].position.y || listOfBullet[i].position.y > listOfEnemy[j].position.y + listOfEnemy[j].size.y);
                    else
                    {
                        listOfBullet[i].active = false;
                        listOfEnemy[j].active = false;
                        score.score += 10;
                        break;
                    }
                }
            }
        }
    }
    for(unsigned int i = 0; i < ENEMY_SIZE; ++i)
    {
        if(listOfEnemy[i].active)
        {
            if (listOfEnemy[i].position.x + listOfEnemy[i].size.x < SQUARE_X || listOfEnemy[i].position.x > SQUARE_X + SQUARE_W ||
                listOfEnemy[i].position.y + listOfEnemy[i].size.y < SQUARE_Y || listOfEnemy[i].position.y > SQUARE_Y + SQUARE_H);
            else
            {
                listOfEnemy[i].active = false;
                colledPlayerWithEnemy = true;
                PlaySound(collisionSound);
                break;
            }
        }
    }
}


void bulletUpdate(BULLET* listOfBullet)                 // FUNCTION TO UPDATE BULLET
{
    if(IsKeyPressed(KEY_SPACE))
    {
        PlaySound(bulletShootSound);
        for(unsigned int i = 0; i < BULLET_SIZE; ++i)
        {
            if(!(listOfBullet[i].active))
            {
                listOfBullet[i].active = true;
                listOfBullet[i].position.x = SQUARE_X;
                listOfBullet[i].position.y = SQUARE_Y;
                listOfBullet[i].size.x = 3;
                listOfBullet[i].size.y = 8;
                listOfBullet[i].speedY = -4;
                break;
            }
        }
    }
    for(unsigned int i = 0; i < BULLET_SIZE; ++i)
    {
        if(listOfBullet[i].active)
        {
            listOfBullet[i].position.y += listOfBullet[i].speedY;
            if(listOfBullet[i].position.y < 0)
                listOfBullet[i].active = false;
        }
    }
}
void enemyUpdate(ENEMY* listOfEnemies)
{
    for(unsigned int i = 0; i < ENEMY_SIZE; ++i)
    {
        if(!(listOfEnemies[i].active))
        {
            listOfEnemies[i].active = true;
            listOfEnemies[i].size.x = SQUARE_H;
            listOfEnemies[i].size.y = SQUARE_W;
            listOfEnemies[i].position.x = rand() % (SIZE_WINDOW - SQUARE_W);
            listOfEnemies[i].position.y = rand()%50;
            listOfEnemies[i].speed = SPEED;
            break;
        }
    }
    for(unsigned int i = 0; i < ENEMY_SIZE; ++i)
    {
        if(listOfEnemies[i].active)
        {
            listOfEnemies[i].position.y += listOfEnemies[i].speed;
            if(listOfEnemies[i].position.y >= SIZE_WINDOW)
                listOfEnemies[i].active = false;
        }
    }
}

void playerUpdate(void)
{
    if(IsKeyDown(KEY_LEFT))
    {
        SQUARE_X -= SPEED;
        if(SQUARE_X <= 0)
            SQUARE_X = SIZE_WINDOW;
    }
    else if(IsKeyDown(KEY_RIGHT))
    {
        SQUARE_X += SPEED;
        if(SQUARE_X >= SIZE_WINDOW)
            SQUARE_X = 0;
    }
}


void exitWindow(void)
{   
    CloseWindow();
}