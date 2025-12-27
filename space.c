#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include <time.h>

#define W 35
#define H 15

typedef struct { int x, y, active; } Entity;

// Global game variables
Entity p = {17, 14, 1}; // Player
Entity e[5];           // 5 Enemies
Entity b = {0, 0, 0};   // 1 Bullet
int score = 0, game = 1;

void setup() {
    srand(time(NULL));
    for (int i = 0; i < 5; i++) {
        e[i].x = i * 6 + 2;
        e[i].y = 0;
        e[i].active = 1;
    }
}

void handleInput() {
    if (_kbhit()) {
        char k = _getch();
        if (k == 'a' && p.x > 0) p.x--;
        if (k == 'd' && p.x < W - 1) p.x++;
        if (k == ' ' && !b.active) { 
            b.x = p.x; b.y = p.y - 1; b.active = 1; 
        }
        if (k == 'x') game = 0;
    }
}

void updateLogic() {
    static int frame = 0;
    
    // Bullet movement and collision
    if (b.active) {
        b.y--;
        if (b.y < 0) b.active = 0;
        for (int i = 0; i < 5; i++) {
            // Collision check: exact match or "passing through" gap
            if (b.x == e[i].x && (b.y == e[i].y || b.y == e[i].y + 1)) {
                score += 10;
                b.active = 0;
                e[i].y = 0;
                e[i].x = rand() % (W - 1);
            }
        }
    }

    // Enemy movement (slower than bullets)
    if (++frame % 12 == 0) {
        for (int i = 0; i < 5; i++) {
            e[i].y++;
            if (e[i].y >= H) game = 0; // Game Over if enemy reaches bottom
        }
    }
}

void drawScreen() {
    COORD cursorPosition = {0, 0};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cursorPosition);
    for (int y = 0; y < H; y++) {
        for (int x = 0; x < W; x++) {
            int ch = ' ';
            if (x == p.x && y == p.y) ch = 'A';
            if (b.active && x == b.x && y == b.y) ch = '|';
            for (int i = 0; i < 5; i++) if (x == e[i].x && y == e[i].y) ch = 'W';
            putchar(ch);
        }
        putchar('\n');
    }
    printf("Score: %d | A-D: Move | SPACE: Shoot | X: Exit", score);
}

int main() {
    setup();
    while (game) {
        drawScreen();
        handleInput();
        updateLogic();
        Sleep(80); // Control game speed in milliseconds
    }
    system("cls");
    printf("\n\n   GAME OVER! Final Score: %d\n", score); 
    return 0;
}