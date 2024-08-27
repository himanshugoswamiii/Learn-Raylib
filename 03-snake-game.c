#include <raylib.h>
#include <raymath.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define CELL_SIZE 30
#define CELL_COUNT 25
#define OFFSET 60

#define GAME_WIDTH (CELL_SIZE * CELL_COUNT)
#define GAME_HEIGHT (CELL_SIZE * CELL_COUNT)

#define SCREEN_WIDTH (OFFSET + GAME_WIDTH + OFFSET)
#define SCREEN_HEIGHT (OFFSET + GAME_HEIGHT + OFFSET)

#define MAX_SNAKE_LENGTH (CELL_COUNT * CELL_COUNT)

enum GameScreen { START_SCREEN = 0, GAMEPLAY, END_SCREEN };  // Used for multiple game scenes / screens

// Declaration + Partial Initialization of Array
Vector2 snake[MAX_SNAKE_LENGTH] = {
    [MAX_SNAKE_LENGTH - 3] = {6, 9}, [MAX_SNAKE_LENGTH - 2] = {5, 9}, [MAX_SNAKE_LENGTH - 1] = {4, 9}};

const int borderThickness = 5;

int snakeHead = MAX_SNAKE_LENGTH - 3;
int snakeEnd = MAX_SNAKE_LENGTH - 1;
bool addSegmentToSnake = false;

bool gameRunning = true;
bool exitGame = false;
int score = 0;

Vector2 randomFoodPosition() {
    srand(time(NULL));
    int x = rand() % CELL_COUNT;
    int y = rand() % CELL_COUNT;

    return (Vector2){x, y};
}

bool checkFoodOnSnake(Vector2 foodPosition) {
    int i = snakeHead;
    while (i != snakeEnd) {
        if (Vector2Equals(snake[i], foodPosition)) {
            return true;
        }
        i = (i + 1) % MAX_SNAKE_LENGTH;
    }
    return false;
}

Vector2 generateRandomFoodPosition() {
    Vector2 foodPosition = randomFoodPosition();

    // Check if the food position is on snake
    while (checkFoodOnSnake(foodPosition)) {
        foodPosition = randomFoodPosition();
    }
    return foodPosition;
}

void updateSnake(Vector2 direction) {
    Vector2 newHead = Vector2Add(snake[snakeHead], direction);
    snakeHead = ((snakeHead)-1 + MAX_SNAKE_LENGTH) % MAX_SNAKE_LENGTH;

    snake[snakeHead] = newHead;

    if (addSegmentToSnake) {
        addSegmentToSnake = false;
    } else {
        snakeEnd = ((snakeEnd)-1 + MAX_SNAKE_LENGTH) % MAX_SNAKE_LENGTH;
    }

    // printf("SnakeHead Index : %d, SnakeEnd index: %d\n", *snakeHead,
    // *snakeEnd);
}

bool checkCollisionWithFood(Vector2 foodPosition) {
    if (Vector2Equals(snake[snakeHead], foodPosition)) return true;
    return false;
}

int main() {
    Vector2 direction = {1, 0};

    Color green = {173, 204, 96, 255};
    Color darkGreen = {43, 51, 24, 255};
    Color purple = {143, 67, 238, 255};

    Vector2 foodPosition = generateRandomFoodPosition();

    enum GameScreen currentScreen = START_SCREEN;

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Snake Game");

    Texture2D startButton = LoadTexture("Assets/Images/start_button.png");
    Texture2D exitButton = LoadTexture("Assets/Images/exit_button.png");

    Texture2D food = LoadTexture("Assets/Images/food.png");
    InitAudioDevice();

    Sound eatSound = LoadSound("Assets/Sounds/eat.mp3");
    Sound wallSound = LoadSound("Assets/Sounds/wall.mp3");

    SetTargetFPS(5);

    // Start-Buttion
    int posX = SCREEN_WIDTH / 2 - (startButton.width / 2);
    int posY = SCREEN_HEIGHT / 2 - startButton.height / 2;
    Rectangle rect = {posX, posY, startButton.width, startButton.height};

    // Actual Game
    while (!WindowShouldClose() && exitGame == false) {
        switch (currentScreen) {
            case START_SCREEN: {
                BeginDrawing();
                ClearBackground(LIGHTGRAY);
                DrawTexture(startButton, posX, posY, WHITE);
                EndDrawing();
                // Check if mouse is clicked on this button
                Vector2 mousePosition = GetMousePosition();
                bool mousePressed = IsMouseButtonPressed(MOUSE_BUTTON_LEFT);

                if (CheckCollisionPointRec(mousePosition, rect) && mousePressed) {
                    currentScreen = GAMEPLAY;
                }
            } break;
            case GAMEPLAY: {
                BeginDrawing();
                ClearBackground(LIGHTGRAY);

                // Game Name
                DrawText("Retro Snake", OFFSET, OFFSET - borderThickness - 40, 40, purple);
                // Score
                DrawText(TextFormat("%i", score), OFFSET, SCREEN_HEIGHT - OFFSET + borderThickness + 5, 40, purple);

                // Game border
                DrawRectangleLinesEx((Rectangle){OFFSET - borderThickness, OFFSET - borderThickness,
                                                 GAME_WIDTH + 2 * borderThickness, GAME_HEIGHT + 2 * borderThickness},
                                     borderThickness, DARKGRAY);

                // Draw horizontal lines
                for (unsigned int i = 1; i < CELL_COUNT; i++) {
                    int startX = CELL_SIZE * i;
                    DrawLine(OFFSET + startX, OFFSET + 0, OFFSET + startX, SCREEN_HEIGHT - OFFSET, GRAY);
                }
                for (unsigned int i = 1; i < CELL_COUNT; i++) {
                    int startY = CELL_SIZE * i;
                    DrawLine(OFFSET + 0, OFFSET + startY, SCREEN_WIDTH - OFFSET, OFFSET + startY, GRAY);
                }

                // Draw Food
                DrawTexture(food, OFFSET + foodPosition.x * CELL_SIZE, OFFSET + foodPosition.y * CELL_SIZE, BLACK);

                // Drawing the snake
                int index = snakeHead;
                while (index != snakeEnd) {
                    int x = snake[index].x;
                    int y = snake[index].y;
                    DrawRectangleRounded(
                        (Rectangle){OFFSET + x * CELL_SIZE, OFFSET + y * CELL_SIZE, CELL_SIZE, CELL_SIZE}, 0.5, 5,
                        DARKGRAY);
                    index++;
                    index = index % MAX_SNAKE_LENGTH;  // Circular Array Snake
                }
                // The loop misses the snakeEnd, hence printing it separately
                DrawRectangleRounded((Rectangle){OFFSET + snake[snakeEnd].x * CELL_SIZE,
                                                 OFFSET + snake[snakeEnd].y * CELL_SIZE, CELL_SIZE, CELL_SIZE},
                                     0.5, 5, DARKGRAY);

                EndDrawing();

                //  ---- EVENT HANDLING ----
                if (IsKeyPressed(KEY_UP) && direction.y != 1) {
                    direction = (Vector2){0, -1};
                }
                if (IsKeyPressed(KEY_DOWN) && direction.y != -1) {
                    /*Vector2 newHead = * {snake[snakeHead].x, snake[snakeHead].y + 1};*/
                    /*Vector2 newHead = Vector2Add(snake[snakeHead], (Vector2){0, 1});
                     * // Equivalent to above*/
                    direction = (Vector2){0, 1};
                }
                if (IsKeyPressed(KEY_RIGHT) && direction.x != -1) {
                    direction = (Vector2){1, 0};
                }
                if (IsKeyPressed(KEY_LEFT) && direction.x != 1) {
                    direction = (Vector2){-1, 0};
                }

                // Eating Food
                if (checkCollisionWithFood(foodPosition) == true) {
                    score++;
                    PlaySound(eatSound);
                    printf("Eating food\n");
                    foodPosition = generateRandomFoodPosition();
                    addSegmentToSnake = true;
                }
                updateSnake(direction);

                //  ---- Game Over Conditions -------
                // 1 : snake outside of window
                if (snake[snakeHead].x >= CELL_COUNT || snake[snakeHead].x < 0 || snake[snakeHead].y >= CELL_COUNT ||
                    snake[snakeHead].y < 0) {
                    PlaySound(wallSound);
                    currentScreen = END_SCREEN;
                }
                // 2 : Snake collision with tail

            } break;
            case END_SCREEN: {
                BeginDrawing();
                // No need to clear the screen
                const char *msg = "You lose";
                int msg_width = MeasureText(msg, 40);
                DrawText(msg, SCREEN_WIDTH / 2 - msg_width / 2, SCREEN_HEIGHT / 2, 40, purple);

                EndDrawing();
            } break;
            default:
                break;
        }
    }

    UnloadSound(eatSound);
    UnloadSound(wallSound);
    CloseAudioDevice();
    UnloadTexture(food);
    UnloadTexture(startButton);
    UnloadTexture(exitButton);

    CloseWindow();
    return 0;
}
