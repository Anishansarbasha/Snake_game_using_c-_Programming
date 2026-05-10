#include <iostream>
#include <conio.h>
#include <windows.h>
using namespace std;

bool gameOver;
const int width = 40;
const int height = 20;
int x, y, fruitX, fruitY, score;
int tailX[100], tailY[100];
int nTail;
enum eDirection { STOP = 0, LEFT, RIGHT, UP, DOWN };
eDirection dir;

// Function to set cursor position
void gotoxy(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

// Function to hide cursor
void hideCursor() {
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
    cursorInfo.bVisible = false;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
}

void Setup() {
    gameOver = false;
    dir = STOP;
    x = width / 2;
    y = height / 2;
    fruitX = rand() % width;
    fruitY = rand() % height;
    score = 0;
    nTail = 0;
}

void DrawInitial() {
    system("cls");
    hideCursor();
    
    // Top wall
    for (int i = 0; i < width + 2; i++)
        cout << "#";
    cout << endl;

    // Game area
    for (int i = 0; i < height; i++) {
        cout << "#";
        for (int j = 0; j < width; j++) {
            cout << " ";
        }
        cout << "#" << endl;
    }

    // Bottom wall
    for (int i = 0; i < width + 2; i++)
        cout << "#";
    cout << endl;
    
    cout << "Score: 0" << endl;
    cout << "Controls: W=Up, A=Left, S=Down, D=Right, X=Exit" << endl;
    cout << "Speed: Press 1(Slow), 2(Medium), 3(Fast)" << endl;
}

void Draw() {
    // Draw snake head
    gotoxy(x + 1, y + 1);
    cout << "O";
    
    // Draw snake tail
    for (int i = 0; i < nTail; i++) {
        gotoxy(tailX[i] + 1, tailY[i] + 1);
        cout << "o";
    }
    
    // Draw fruit
    gotoxy(fruitX + 1, fruitY + 1);
    cout << "F";
    
    // Update score
    gotoxy(0, height + 2);
    cout << "Score: " << score << "  ";
}

void ClearPosition(int px, int py) {
    gotoxy(px + 1, py + 1);
    cout << " ";
}

void Input(int &gameSpeed) {
    if (_kbhit()) {
        switch (_getch()) {
            case 'a':
            case 'A':
                if (dir != RIGHT)
                    dir = LEFT;
                break;
            case 'd':
            case 'D':
                if (dir != LEFT)
                    dir = RIGHT;
                break;
            case 'w':
            case 'W':
                if (dir != DOWN)
                    dir = UP;
                break;
            case 's':
            case 'S':
                if (dir != UP)
                    dir = DOWN;
                break;
            case 'x':
            case 'X':
                gameOver = true;
                break;
            case '1':
                gameSpeed = 200; // Slow
                break;
            case '2':
                gameSpeed = 100; // Medium
                break;
            case '3':
                gameSpeed = 50; // Fast
                break;
        }
    }
}

void Logic() {
    int prevX = tailX[0];
    int prevY = tailY[0];
    int prev2X, prev2Y;
    tailX[0] = x;
    tailY[0] = y;
    
    for (int i = 1; i < nTail; i++) {
        prev2X = tailX[i];
        prev2Y = tailY[i];
        tailX[i] = prevX;
        tailY[i] = prevY;
        prevX = prev2X;
        prevY = prev2Y;
    }
    
    // Clear last tail position
    if (nTail > 0) {
        ClearPosition(prevX, prevY);
    }

    // Clear old head position
    ClearPosition(x, y);

    switch (dir) {
        case LEFT:
            x--;
            break;
        case RIGHT:
            x++;
            break;
        case UP:
            y--;
            break;
        case DOWN:
            y++;
            break;
        default:
            break;
    }

    // Wall collision (wrap around)
    if (x >= width) x = 0; 
    else if (x < 0) x = width - 1;
    if (y >= height) y = 0; 
    else if (y < 0) y = height - 1;

    // Self collision
    for (int i = 0; i < nTail; i++)
        if (tailX[i] == x && tailY[i] == y)
            gameOver = true;

    // Fruit collision
    if (x == fruitX && y == fruitY) {
        score += 10;
        fruitX = rand() % width;
        fruitY = rand() % height;
        nTail++;
    }
}

int main() {
    int gameSpeed = 150; // Default speed (slower)
    
    Setup();
    DrawInitial();
    
    while (!gameOver) {
        Input(gameSpeed);
        Logic();
        Draw();
        Sleep(gameSpeed);
    }
    
    gotoxy(0, height + 4);
    cout << "\n\nGame Over! Final Score: " << score << endl;
    cout << "Press any key to exit...";
    _getch();
    return 0;
}
