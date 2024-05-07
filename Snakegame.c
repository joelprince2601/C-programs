#include <stdio.h>
#include <conio.h>
#include <windows.h>

int width = 20;
int height = 20;

int snakeX, snakeY;
int fruitX, fruitY;
int score;
int gameover;
int tailX[100], tailY[100];
int tailLength;

enum eDirection { STOP = 0, LEFT, RIGHT, UP, DOWN };
enum eDirection dir;

void Setup() {
    gameover = 0;
    dir = STOP;
    snakeX = width / 2;
    snakeY = height / 2;
    fruitX = rand() % width;
    fruitY = rand() % height;
    score = 0;
}

void Draw() {
    system("cls");
    for (int i = 0; i < width + 2; i++)
        printf("#");
    printf("\n");

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (j == 0)
                printf("#");

            if (i == snakeY && j == snakeX)
                printf("O");
            else if (i == fruitY && j == fruitX)
                printf("F");
            else {
                int printTail = 0;
                for (int k = 0; k < tailLength; k++) {
                    if (tailX[k] == j && tailY[k] == i) {
                        printf("o");
                        printTail = 1;
                    }
                }
                if (!printTail)
                    printf(" ");
            }

            if (j == width - 1)
                printf("#");
        }
        printf("\n");
    }

    for (int i = 0; i < width + 2; i++)
        printf("#");
    printf("\n");
    printf("Score: %d\n", score);
}

void Input() {
    if (_kbhit()) {
        switch (_getch()) {
            case 'a':
                dir = LEFT;
                break;
            case 'd':
                dir = RIGHT;
                break;
            case 'w':
                dir = UP;
                break;
            case 's':
                dir = DOWN;
                break;
            case 'x':
                gameover = 1;
                break;
        }
    }
}

void Logic() {
    int prevX = tailX[0];
    int prevY = tailY[0];
    int prev2X, prev2Y;
    tailX[0] = snakeX;
    tailY[0] = snakeY;
    for (int i = 1; i < tailLength; i++) {
        prev2X = tailX[i];
        prev2Y = tailY[i];
        tailX[i] = prevX;
        tailY[i] = prevY;
        prevX = prev2X;
        prevY = prev2Y;
    }

    switch (dir) {
        case LEFT:
            snakeX--;
            break;
        case RIGHT:
            snakeX++;
            break;
        case UP:
            snakeY--;
            break;
        case DOWN:
            snakeY++;
            break;
    }

    if (snakeX >= width)
        snakeX = 0;
    else if (snakeX < 0)
        snakeX = width - 1;

    if (snakeY >= height)
        snakeY = 0;
    else if (snakeY < 0)
        snakeY = height - 1;

    for (int i = 0; i < tailLength; i++) {
        if (tailX[i] == snakeX && tailY[i] == snakeY) {
            gameover = 1;
            break;
        }
    }

    if (snakeX == fruitX && snakeY == fruitY) {
        score += 10;
        fruitX = rand() % width;
        fruitY = rand() % height;
        tailLength++;
    }
}

int main() {
    Setup();
    while (!gameover) {
        Draw();
        Input();
        Logic();
        Sleep(10);  // Delay in milliseconds
    }
    return 0;
}
