#ifndef BULLET_H
#define BULLET_H

#include "raylib.h"

class Bullet {
public:
    Rectangle rect;
    Vector2 speed;
    bool active;
    int type;


    bool isR = true;
    Bullet();
    void Update();
    void Draw();
};

#endif // BULLET_H
