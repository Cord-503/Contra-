#include "Player.h"

#define PLAYERX 100
#define PLAYERY 200
#define PLAYERW 20
#define PLAYERH 40

Player::Player() : rect{ 100, 200, 20, 40 }, curLife(3), invulnerable(false), invulnerableTimer(0.0f), velocity{ 0, 0 } {}

void Player::Update() {
    const float gravity = 0.5f;
    const float moveSpeed = 5.0f;
    const float jumpSpeed = -10.0f;
    const float ground = 380.0f;
    bool onGround = rect.y >= ground;

    if (IsKeyDown(KEY_RIGHT)) {
        isR = true; 
        rect.x += moveSpeed;
    }
        
    if (IsKeyDown(KEY_LEFT)) {
        rect.x -= moveSpeed; 
        isR = false;
    }

    // jump
    if (IsKeyPressed(KEY_SPACE) && (onGround || onPlatform))
        velocity.y = jumpSpeed;

    if (velocity.y != 0) {
        onPlatform = false;
    }

    // gravity
    velocity.y += gravity;
    rect.y += velocity.y;

    if (rect.y > ground) {
        rect.y = ground;
        velocity.y = 0;
    }
    if (invulnerable) {
        invulnerableTimer -= GetFrameTime();
        if (invulnerableTimer <= 0) {
            invulnerable = false;
        }
    }
}

void Player::Draw() {
    Color color = invulnerable ? GOLD : BLUE;
    DrawRectangleRec(rect, color);
}

bool Player::WantsToShoot() {
    return IsKeyDown(KEY_F);
}
