#ifndef CONTRA_H
#define CONTRA_H

#include "raylib.h"
#include "Player.h"
#include "Enemy.h"
#include "Bullet.h"

class Game {
public:
    Player player;
    Enemy enemies[50];
    Bullet bullets[50];
    int score;

#define MAX_LIVES 5

    bool gamePaused = false;
    bool gameOver = false;
    bool levelWon = false;

    static const int screenWidth = 800;
    static const int screenHeight = 450;

    int bulletsBurst = 1;
    int fired = 0;
    float cool = 0.15f;
    float lastBurst = 0.0f;

    float lastEnemySpawnX = 0;

    int hScore = 0;

    void Main();
    void Start();

    void EvalCurFrame();
    void DrawCurFrame();
    void Update();

    bool CheckIfHit();
    void DrawGround();

    bool lifeBallVisible = true;

};

#endif
