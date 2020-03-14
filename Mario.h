#ifndef MARIO_H
#define MARIO_H

#include "Sprite.h"
#include<QDateTime>
#include<string>
using namespace std;

class BulletBill;
class Enemy;
class Shell;

class Mario : public Sprite
{
public:
    Mario();
    void setLarge(bool large, bool fire);
    void blink(bool on);
    void calcPic();
    void move() override;
    bool move(float xa, float ya);
    void win();
    void die();

    void stomp(BulletBill * bill);
    void stomp(Enemy* enemy);
    void stomp(Shell* shell);
    void kick(Shell* shell);
    void getHurt();
    void getFlower();
    void getMushroom();

    static void get1Up();
    static void getCoin();
    static void resetStatic();

    static bool large;
    static bool fire;
    static int coins;
    static int lives;
    static string levelString;
    int facing;
    bool wasOnGround = false;
    bool onGround = false;
    int width = 8;
    int height = 48;
    int xDeathPos, yDeathPos;
    int deathTime = 0;
    int winTime = 0;
    Sprite* carried = nullptr;
private:
    bool isBlocking(float _x, float _y, float xa, float ya);

    static Mario* instance;
    static float GROUND_INERTIA;
    static float AIR_INERTIA;
    bool lastLarge;
    bool lastFire;
    bool newLarge;
    bool newFire;
    float runTime = 0.00f;
    bool mayJump = false;
    bool ducking = false;
    bool sliding = false;
    int jumpTime = 0;
    float xJumpSpeed;
    float yJumpSpeed;
    bool canShoot = false;

    int powerUpTime = 0;
    int invulnerableTime = 0;
};

#endif // MARIO_H
