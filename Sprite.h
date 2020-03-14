#ifndef SPRITE_H
#define SPRITE_H

#include<QImage>
#include<vector>
#include<QPainter>
using namespace std;

class LevelScene;
class SpriteTemplate;
class Shell;
class Mario;
class Fireball;

class Sprite
{
public:
    explicit Sprite();
    virtual ~Sprite();
    virtual void move();
    virtual void tick();
    virtual void tickNoMove();
    virtual void collideCheck();
    virtual void bumpCheck(int xTile, int yTile);
    virtual void release(Mario* mario);
    virtual void render(QPainter& painter);
    virtual bool shellCollideCheck(Shell* shell);
    virtual bool fireballCollideCheck(Fireball* fireball);

    static LevelScene* levelScene;
    float xOld = 0, yOld = 0, x = 0, y = 0, xa = 0, ya = 0;
    int xPic = 0, yPic = 0;
    int wPic = 32;
    int hPic = 32;
    int xPicO, yPicO;
    bool xFlipPic = false;
    bool yFlipPic = false;
    vector<vector<QImage>> sheet;
    bool visible = true;
    int layer = 1;
    SpriteTemplate* spriteTemplate{nullptr};
};

#endif // SPRITE_H
