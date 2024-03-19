#include "Bullet.h"

Bullet::Bullet() : rect{ 0, 0, 10, 5 }, speed{ 0, 0 }, active(false), type(0) {}

void Bullet::Update() {
    if (active) {
        rect.x += speed.x;
        rect.y += speed.y;

        if (rect.x < 0 || rect.x > GetScreenWidth() || rect.y < 0 || rect.y > GetScreenHeight()) {
            active = false;
        }
    }
}

void Bullet::Draw() {
    if (active) {
        DrawCircle(rect.x, rect.y, rect.width / 2, BLACK);
    }
}
