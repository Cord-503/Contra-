#ifndef PLAYER_H
#define PLAYER_H

#include "raylib.h"


class Player {
public:
    Vector2 velocity;
    Rectangle rect;
    int curLife;

    bool onPlatform = false;
    bool facingRight = true;

    Player();
    void Update();
    void Draw();

    bool WantsToShoot();
    bool isR;

    bool invulnerable = false;
    float invulnerableTimer = 0.0f;
    float invulnerableDuration = 0.5f;
};

#endif // PLAYER_H