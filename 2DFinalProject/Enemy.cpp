#include "Enemy.h"
#include "raylib.h"
#include <stdlib.h>

Enemy::Enemy() : rect{ 0, 0, 20, 20 }, health(2), active(false), speed(GetRandomValue(-2, 3)) {}

void Enemy::Update() {
    if (active) {
        rect.x += speed;
        Draw();

        if (!onPlatform) {
            const float gravity = 0.5f;
            velocity.y += gravity;
            rect.y += velocity.y;
            
            if (rect.y >= 380) {
                rect.y = 380;
                velocity.y = 0;
            }
        }
    }
}


void Enemy::Draw() {
    if (active) {
        DrawRectangleRec(rect, RED);
    }
}
