#include <raylib.h>
#include <stdio.h>
#include <stdlib.h> // for rand

const int screen_width = 1000;
const int screen_height = 800;
const int intial_speed = 2;
const int speed_choices[] = {-1, 1};

int player_wins = 0;
int bot_wins = 0;

void reset_ball(int *x, int *y, int *speed_x, int *speed_y) {
    *x = screen_width / 2;
    *y = screen_height / 2;

    // Reset the speed of ball as well
    (*speed_x) = intial_speed * speed_choices[rand() % 2];
    (*speed_y) = intial_speed * speed_choices[rand() % 2];
}

void increase_speed(int *speed) {
    if (*speed < 0) {
        *speed -= 1;
    } else {
        *speed += 1;
    }
}

int main() {
    printf("Starting the Game\n");

    Color cBLACK = {34, 40, 49, 255};
    Color cDARK_GREY = {57, 62, 70, 255};
    Color cTEAL = {0, 173, 181, 255};

    InitWindow(screen_width, screen_height, "Evil-Pong");
    SetTargetFPS(120);

    int ball_x = screen_width / 2;
    int ball_y = screen_height / 2;
    int ball_radius = 20;
    int speed_x = intial_speed;
    int speed_y = intial_speed;

    // Paddles
    const int rect_width = 30;
    const int rect_height = 140;
    const int padding = 10;
    int rect_speed = 3;

    const int rect_y = screen_height / 2 - (rect_height / 2);

    int player_rect_x = screen_width - rect_width - padding;
    int player_rect_y = rect_y;

    int bot_rect_x = 0 + padding;
    int bot_rect_y = rect_y;

    const int font_size = 30;

    while (!WindowShouldClose()) {
        ClearBackground(cBLACK);

        // 2: Drawing
        BeginDrawing();

        DrawRectangle(screen_width / 2, 0, screen_width / 2, screen_height,
                      cDARK_GREY);

        DrawLine(screen_width / 2, 0, screen_width / 2, screen_height, GRAY);

        DrawCircle(ball_x, ball_y, ball_radius, MAGENTA);

        // Bot paddle
        if (ball_y < bot_rect_y + rect_height / 2)
            bot_rect_y -= rect_speed;
        if (ball_y > bot_rect_y + rect_height / 2)
            bot_rect_y += rect_speed;

        DrawRectangleRounded(
            (Rectangle){bot_rect_x, bot_rect_y, rect_width, rect_height}, 0.5,
            0, cTEAL);

        // Player paddle
        DrawRectangleRounded(
            (Rectangle){player_rect_x, player_rect_y, rect_width, rect_height},
            0.5, 0, cTEAL);

        // Score
        DrawText(TextFormat("%i", bot_wins), screen_width / 4 - font_size,
                 font_size, font_size, WHITE);
        DrawText(TextFormat("%i", player_wins),
                 screen_width - screen_width / 4 - font_size, font_size,
                 font_size, WHITE);
        EndDrawing();

        // 1: Event Handling
        if (IsKeyDown(KEY_UP)) {
            if (player_rect_y >= 0)
                player_rect_y -= rect_speed;
        }
        if (IsKeyDown(KEY_DOWN)) {
            if (player_rect_y <= screen_height - rect_height)
                player_rect_y += rect_speed;
        }

        // Winning condition
        if (ball_x >= screen_width - ball_radius) {
            bot_wins++;
            reset_ball(&ball_x, &ball_y, &speed_x, &speed_y);
        }
        if (ball_x <= 0 + ball_radius) {
            player_wins++;
            reset_ball(&ball_x, &ball_y, &speed_x, &speed_y);
        }

        // Handle Ball moves out of the window
        if (ball_y >= screen_height - ball_radius || ball_y <= 0 + ball_radius)
            speed_y *= -1;

        ball_x += speed_x;
        ball_y += speed_y;

        if (CheckCollisionCircleRec((Vector2){ball_x, ball_y}, ball_radius,
                                    (Rectangle){player_rect_x, player_rect_y,
                                                rect_width, rect_height})) {
            speed_x *= -1;
            // Increase speed on each collision
            increase_speed(&speed_x);
        }
        if (CheckCollisionCircleRec(
                (Vector2){ball_x, ball_y}, ball_radius,
                (Rectangle){bot_rect_x, bot_rect_y, rect_width, rect_height})) {
            speed_x *= -1;
            increase_speed(&speed_x);
        }
    }

    CloseWindow();

    return 0;
}
