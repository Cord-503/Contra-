#include "Contra.h"
#include "iostream"

float deltaTime = 0.0f;
float mapOffset = 0.0f;

void Game::Main()
{
    InitWindow(screenWidth, screenHeight, "2D final Contra");

    Start();

    SetTargetFPS(60);
    while (!WindowShouldClose())
    {
        Update();
    }
    CloseWindow();
}

void Game::Start() {
    gameOver = false;
    mapOffset = 0;
    score = 0;

    const int distanceE = 50;
    int lastEPX = screenWidth;

    for (auto& enemy : enemies) {
        int distance = GetRandomValue(distanceE, distanceE * 2);
        lastEPX += distance;
        enemy.rect.x = lastEPX - mapOffset;
        enemy.rect.y = 380;
        enemy.rect.width = 20;
        enemy.rect.height = 20;
        enemy.health = 2;
        enemy.active = true;
        enemy.speed = GetRandomValue(-10, 10);
    }
}

void Game::EvalCurFrame()
{
    if (gameOver)
    {
        if (IsKeyPressed(KEY_ENTER))
        {
            Start();
            gameOver = false;

            player.rect.x = 100;
            player.rect.y = 200;
            player.curLife = 3;
        }
        return;
    }

    if (levelWon)
    {
        if (IsKeyPressed(KEY_ENTER))
        {
            Start();

            levelWon = false;
            player.rect.x = 100;
            player.rect.y = 200;
            player.curLife = 3;
        }
        return;
    }

    if (IsKeyPressed(KEY_P))
        gamePaused = !gamePaused;

    if (gamePaused) return;

    if (player.curLife == 0) {
        gameOver = true;
    }
    else
    {
        if (player.rect.x + player.rect.width >= 1800 - mapOffset) {
            levelWon = true;
        }
    }
}

void Game::DrawCurFrame()
{
    BeginDrawing();
    ClearBackground(RAYWHITE);

    DrawRectangle(screenWidth / 2 - 128, screenHeight / 2 - 128, 256, 256, LIGHTGRAY);
    DrawRectangle(screenWidth / 2 - 112, screenHeight / 2 - 112, 224, 224, RAYWHITE);
    DrawText("raylib", screenWidth / 2 - 44, screenHeight / 2 + 48, 50, LIGHTGRAY);

    if (gameOver)
    {
        DrawText("GG! Press Enter to play again",
            GetScreenWidth() / 2 - MeasureText("GG! Press Enter to play again", 30) / 2,
            GetScreenHeight() / 2 - 15,
            30, GRAY);

    }
    else if (levelWon)
    {
        DrawText("You Won! Ohhhhhhhhhhhh!!!!!!",
            GetScreenWidth() / 2 - MeasureText("You Won! Ohhhhhhhhhhhh!!!!!!", 30) / 2,
            GetScreenHeight() / 2 - 20,
            30, GRAY);

        DrawText("GG! Press Enter to play again",
            GetScreenWidth() / 2 - MeasureText("GG! Press Enter to play again", 20) / 2,
            GetScreenHeight() / 2 + 15,
            20, GRAY);
    }

    else
    {
        player.Draw();

        if (lifeBallVisible) {
            Vector2 ballPosition = { 575 - mapOffset, 270 };
            DrawCircleV(ballPosition, 10, GREEN);
            DrawText("Life++", ballPosition.x - MeasureText("Life++", 10) / 2, ballPosition.y - 5, 10, BLACK);
        }

        for (auto& enemy : enemies) {
            enemy.Draw();
        }

        for (auto& bullet : bullets) {
            bullet.Draw();
        }

        DrawText(TextFormat("Score: %04i", score), 10, 10, 20, BLACK);
        DrawText(TextFormat("High Score: %04i", hScore), 10, 30, 20, BLACK);

        DrawRectangleRounded(Rectangle{ 570, 10, 210, 50 }, 0.5f, 5, DARKGRAY);
        DrawText("Life", 585, 18, 12, WHITE);

        for (int i = 0; i < MAX_LIVES; i++)
        {
            if (i < player.curLife)
                DrawRectangle(580 + 40 * i, 35, 30, 10, GRAY);
            else
                DrawRectangle(580 + 40 * i, 35, 30, 10, LIGHTGRAY);
        }

        DrawGround();

        // Goal
        DrawLine(1800 - mapOffset, 0, 1800 - mapOffset, screenHeight, RED);  // Goal
        DrawText("Finish", 1730 - mapOffset, 80, 20, ORANGE);
    }
    EndDrawing();
}

void Game::Update()
{
    float previousTime = GetTime();
    EvalCurFrame();

    if (!gamePaused && !gameOver && !levelWon) {
        mapOffset += 2; 
        player.rect.x -= 1;

        if (lifeBallVisible) {
            Rectangle ballRect = { 575 - mapOffset - 10, 270 - 10, 20, 20 };
            if (CheckCollisionRecs(player.rect, ballRect)) {
                player.curLife++;
                lifeBallVisible = false;
            }
        }

        player.Update();
        if (player.rect.x < 0 || player.rect.x > screenWidth) {
            gameOver = true;

        }

        for (auto& enemy : enemies) {
            enemy.Update();
            enemy.rect.x += 3;

            if (CheckCollisionRecs(player.rect, enemy.rect) && enemy.active && !player.invulnerable) {
                player.invulnerable = true;
                player.invulnerableTimer = player.invulnerableDuration;
                player.curLife--;
            }
        }

        lastBurst += deltaTime;

        if (player.WantsToShoot() && fired < bulletsBurst && lastBurst >= cool) {
            int bulletsToFire = 1 - fired;
            for (int i = 0; i < bulletsToFire; ++i) {
                for (auto& bullet : bullets) {
                    if (!bullet.active) {
                       /* std::cout << "not" << std::endl;*/
                        bullet.active = true;
                        bullet.rect.x = player.rect.x + (player.isR ? player.rect.width : 0);
                        bullet.rect.y = player.rect.y + player.rect.height / 2;

                        bullet.speed.x = (player.isR ? 8.0f : -8.0f); // Adjust speed here

                        fired++;
                        break;
                    }
                }
            }

            if (fired >= bulletsBurst) {
                lastBurst = 0.0f;
                fired = 0;
            }
        }

        for (auto& bullet : bullets) {
            if (bullet.active) {
                bullet.rect.x += bullet.speed.x;

                if (bullet.rect.x < 0 || bullet.rect.x > screenWidth) {
                    bullet.active = false;
                }

                // Check collision with enemies
                for (auto& enemy : enemies) {
                    if (enemy.active && CheckCollisionCircleRec(Vector2{ bullet.rect.x, bullet.rect.y }, bullet.rect.width / 2, enemy.rect)) {
                        //std::cout << "Collision detected between bullet and enemy!" << std::endl;
                        bullet.active = false;
                        enemy.health -= 1;
                        if (enemy.health <= 0) {
                            enemy.active = false;

                            score += 10; // score add
                            if (score > hScore) {
                                hScore = score;
                            }
                        }
                        break;
                    }
                }
            }
        }
    }

    DrawCurFrame();
    float currentTime = GetTime();
    deltaTime = currentTime - previousTime;
}

bool Game::CheckIfHit() {
    return false;
}

void Game::DrawGround() {
    DrawLine(0, 420, screenWidth, 420, BLACK);

    // Add platforms here
    Rectangle platforms[] = { {200 - mapOffset, 340, 120, 10}, {500 - mapOffset, 290, 150, 10}, {1000 - mapOffset, 340, 150, 10}, {1200 - mapOffset, 300, 150, 10}, {1450 - mapOffset, 220, 150, 10} };

    for (auto& platform : platforms) {
        DrawRectangleRec(platform, BLACK);

        if (CheckCollisionRecs(player.rect, platform)) {
            // If auto fix the player on the platforms
            if (player.velocity.y > 0 /*&& (player.rect.y <= platform.y - player.rect.height)*/) {
                player.onPlatform = true;
                player.rect.y = platform.y - player.rect.height;
                player.velocity.y = 0;

            }
        }
    }
}