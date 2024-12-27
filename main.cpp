#include <raylib.h>
#include <iostream>
#include <fstream>
#include <string>

Color Green = Color{38, 185, 154, 255};
Color Dark_Green = Color{20, 160, 133, 255};
Color Light_Green = Color{129, 204, 184, 255};
Color Yellow = Color{243, 213, 91, 255};

int player_score = 0;
int cpu_score = 0;
int player_lives = 3;
int cpu_lives = 3;
int high_score = 0;

class Ball {
public:
    float x, y;
    int speed_x, speed_y;
    int radius;

    void Draw() {
        DrawCircle(x, y, radius, Yellow);
    }

    void Update() {
        x += speed_x;
        y += speed_y;

        if (y + radius >= GetScreenHeight() || y - radius <= 0) {
            speed_y *= -1; // Bounce off the top and bottom walls
        }
    }

    void ResetBall() {
        x = GetScreenWidth() / 2;
        y = GetScreenHeight() / 2;

        int speed_choices[2] = {-1, 1};
        speed_x *= speed_choices[GetRandomValue(0, 1)];
        speed_y *= speed_choices[GetRandomValue(0, 1)];
    }
};

class Paddle {
protected:
    void LimitMovement() {
        if (y <= 0) {
            y = 0;
        }
        if (y + height >= GetScreenHeight()) {
            y = GetScreenHeight() - height;
        }
    }

public:
    float x, y;
    float width, height;
    int speed;

    void Draw() {
        DrawRectangleRounded(Rectangle{x, y, width, height}, 0.8, 0, WHITE);
    }

    void Update() {
        if (IsKeyDown(KEY_UP)) {
            y = y - speed;
        }
        if (IsKeyDown(KEY_DOWN)) {
            y = y + speed;
        }
        LimitMovement();
    }
};

class CpuPaddle : public Paddle {
public:
    void Update(int ball_y, int difficulty) {
        int reactionSpeed = speed;

        // Adjust CPU paddle speed based on difficulty
        switch (difficulty) {
            case 1: // Easy
                reactionSpeed = speed / 2; // Slower reaction
                break;
            case 2: // Medium
                reactionSpeed = speed; // Normal reaction
                break;
            case 3: // Difficult
                reactionSpeed = speed * 2; // Faster reaction
                break;
        }

        if (y + height / 2 > ball_y) {
            y = y - reactionSpeed;
        }
        if (y + height / 2 <= ball_y) {
            y = y + reactionSpeed;
        }
        LimitMovement();
    }
};

Ball ball;
Paddle player;
CpuPaddle cpu;

void LoadHighScore() {
    std::ifstream file("highscore.txt");
    if (file.is_open()) {
        file >> high_score;
        file.close();
    }
}

void SaveHighScore() {
    std::ofstream file("highscore.txt");
    if (file.is_open()) {
        file << high_score;
        file.close();
    }
}

void ShowWinner(const std::string& winner) {
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(Dark_Green);
        DrawText((winner + " has won!").c_str(), 400, 300, 40, WHITE);
        DrawText(TextFormat("High Score: %i", high_score), 400, 350, 30, WHITE);
        DrawText("Press R to restart", 400, 400, 30, WHITE);
        EndDrawing();

        if (IsKeyPressed(KEY_R)) {
            // Reset game state
            player_score = 0;
            cpu_score = 0;
            player_lives = 3;
            cpu_lives = 3;
            ball.ResetBall(); // Reset ball position
            return; // Restart the game
        }
    }
}

void ResetGame() {
    player_score = 0;
    cpu_score = 0;
    player_lives = 3;
    cpu_lives = 3;
    ball.ResetBall(); // Reset ball position
}

int main() {
    LoadHighScore(); // Load the high score from file

    const int screen_width = 1280;
    const int screen_height = 800;
    InitWindow(screen_width, screen_height, "My Pong Game!");
    SetTargetFPS(60);
    
    // Initialize ball
    ball.radius = 20;
    ball.x = screen_width / 2;
    ball.y = screen_height / 2;
    ball.speed_x = 7;
    ball.speed_y = 7;

    // Initialize player paddle
    player.width = 25;
    player.height = 120;
    player.x = screen_width - player.width - 10;
    player.y = screen_height / 2 - player.height / 2;
    player.speed = 6;

    // Initialize CPU paddle
    cpu.height = 120;
    cpu.width = 25;
    cpu.x = 10;
    cpu.y = screen_height / 2 - cpu.height / 2;
    cpu.speed = 6;

    // Difficulty selection
    int difficulty = 2; // Default to Medium
    bool showDifficultyWindow = true;
    while (showDifficultyWindow) {
        BeginDrawing();
        ClearBackground(Dark_Green);

        // Draw the difficulty selection window
        DrawRectangle(screen_width / 4, screen_height / 4, screen_width / 2, screen_height / 2, Light_Green);
        DrawText("Select Difficulty Level", screen_width / 4 + 50, screen_height / 4 + 50, 40, WHITE);
        DrawText("1. Easy", screen_width / 4 + 100, screen_height / 4 + 150, 30, WHITE);
        DrawText("2. Medium", screen_width / 4 + 100, screen_height / 4 + 200, 30, WHITE);
        DrawText("3. Difficult", screen_width / 4 + 100, screen_height / 4 + 250, 30, WHITE);

        // Handle user input for difficulty selection
        if (IsKeyPressed(KEY_ONE)) {
            difficulty = 1;
            showDifficultyWindow = false;
        }
        if (IsKeyPressed(KEY_TWO)) {
            difficulty = 2;
            showDifficultyWindow = false;
        }
        if (IsKeyPressed(KEY_THREE)) {
            difficulty = 3;
            showDifficultyWindow = false;
        }

        EndDrawing();
    }

    while (WindowShouldClose() == false) {
        BeginDrawing();

        // Updating
        ball.Update();
        player.Update();
        cpu.Update(ball.y, difficulty);

        // Checking for collisions
        if (CheckCollisionCircleRec({ball.x, ball.y}, ball.radius, {player.x, player.y, player.width, player.height})) {
            ball.speed_x *= -1;
            player_score++; // Increase player score when hitting the player paddle
        }

        if (CheckCollisionCircleRec({ball.x, ball.y}, ball.radius, {cpu.x, cpu.y, cpu.width, cpu.height})) {
            ball.speed_x *= -1;
            cpu_score++; // Increase CPU score when hitting the CPU paddle
        }

        // Check if the ball touches the walls
        if (ball.x - ball.radius <= 0) { // Ball touches left wall (CPU side)
            cpu_lives--;
            ball.ResetBall();
        }
        if (ball.x + ball.radius >= GetScreenWidth()) { // Ball touches right wall (Player side)
            player_lives--;
            ball.ResetBall();
        }

        // Check for game over conditions
        if (player_lives <= 0) {
            if (player_score > high_score) {
                high_score = player_score; // Update high score if necessary
                SaveHighScore(); // Save new high score to file
            }
            ShowWinner("CPU");
            ResetGame(); // Reset game state for next round
            continue; // Restart the game loop
        }
        if (cpu_lives <= 0) {
            if (player_score > high_score) { // Compare player score to high score
                high_score = player_score; // Update high score if necessary
                SaveHighScore(); // Save new high score to file
            }
            ShowWinner("Player");
            ResetGame(); // Reset game state for next round
            continue; // Restart the game loop
        }

        // Drawing
        ClearBackground(Dark_Green);
        DrawRectangle(screen_width / 2, 0, screen_width / 2, screen_height, Green);
        DrawCircle(screen_width / 2, screen_height / 2, 150, Light_Green);
        DrawLine(screen_width / 2, 0, screen_width / 2, screen_height, WHITE);
        ball.Draw();
        cpu.Draw();
        player.Draw();
        DrawText(TextFormat("CPU Score: %i", cpu_score), screen_width / 4 - 20, 20, 40, WHITE);
        DrawText(TextFormat("Player Score: %i", player_score), 3 * screen_width / 4 - 20, 20, 40, WHITE);
        DrawText(TextFormat("CPU Lives: %i", cpu_lives), screen_width / 4 - 20, 60, 40, WHITE);
        DrawText(TextFormat("Player Lives: %i", player_lives), 3 * screen_width / 4 - 20, 60, 40, WHITE);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
