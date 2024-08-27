#include <raylib.h>

int main(int argc, char *argv[]) {
    const int screenWidth = 800;
    const int screenHeight = 800;
    Color green = {20, 160, 133, 255}; // Custom-defined color

    InitWindow(screenWidth, screenHeight, "My Raylib Game");
    SetTargetFPS(60);

    int ballX = 400;
    int ballY = 400;

    // Game Loop
    while (WindowShouldClose() == false) {
        // 1. Event Handling + Updating Position
        if (IsKeyDown(KEY_RIGHT)) {
            ballX += 3;
        } else if (IsKeyDown(KEY_LEFT)) {
            ballX -= 3;
        } else if (IsKeyDown(KEY_UP)) {
            ballY -= 3;
        } else if (IsKeyDown(KEY_DOWN)) {
            ballY += 3;
        }

        ClearBackground(green);

        // 3. Drawing
        BeginDrawing();
        DrawCircle(ballX, ballY, 40, WHITE);
        EndDrawing();
    }

    CloseWindow();

    return 0;
}
