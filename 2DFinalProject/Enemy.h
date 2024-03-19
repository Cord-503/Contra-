#ifndef ENEMY_H
#define ENEMY_H

#include "raylib.h"

class Enemy {
public:
    Rectangle rect;
    int health;
    bool active;
    float speed;


    Vector2 velocity;
    bool onPlatform;

    Enemy();
    void Update();
    void Draw();
};

#endif
