#ifndef LEVELSCENE_H
#define LEVELSCENE_H

#include "Scene.h"
#include "Level.h"
#include<memory>
#include<vector>
#include<array>
using namespace std;

class Sprite;
class MarioComponent;
class LevelRenderer;
class BgRenderer;
class Mario;
class Shell;
class Fireball;

class LevelScene : public Scene
{
public:
    LevelScene();
    LevelScene(MarioComponent* renderer, int levelDifficulty, int levelType);
    virtual ~LevelScene();

    virtual void init();
    virtual void tick();
    virtual void render(QPainter& painter);
    void bump(int x, int y, bool canBreakBricks);
    void bumpInto(int x, int y);
    void addSprite(Sprite* sprite);
    void removeSprite(Sprite* sprite);
    bool findSprite(Sprite* sprite);
    void addAndRemoveSprite();
    void checkShellCollide(Shell* shell);
    void checkFireballCollide(Fireball* fireball);
    void renderBlackout(QPainter& painter, int x, int y, int radius);
    void drawStringDropShadow(QPainter& painter, string text, int x, int y, int c);
    void drawString(QPainter& painter, string text, int x, int y, int c);

    shared_ptr<Level> level;
    shared_ptr<Mario> mario;
    float xCam = 0, yCam = 0, xCamO, yCamO;
    bool paused{false};
    int startTime{0};
    int fireballsOnScreen = 0;
private:
    vector<shared_ptr<Sprite>> sprites;
    vector<Sprite*> spritesToAdd;
    vector<Sprite*> spritesToRemove;
    shared_ptr<LevelRenderer> layer;
    array<shared_ptr<BgRenderer>, 2> bgLayer = {nullptr, nullptr};
    vector<Shell*> shellsToCheck;
    vector<Fireball*> fireballsToCheck;
    MarioComponent* renderer;
    long long llTick = 0;
    int timeLeft;
    int levelType;
    int levelDifficulty;
};

#endif // LEVELSCENE_H
