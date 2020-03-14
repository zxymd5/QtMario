#include "LevelScene.h"
#include "LevelGenerator.h"
#include "Mario.h"
#include "LevelRenderer.h"
#include "BgRenderer.h"
#include "BgLevelGenerator.h"
#include "SpriteTemplate.h"
#include "Sparkle.h"
#include "BulletBill.h"
#include "Particle.h"
#include "CoinAnim.h"
#include "Mushroom.h"
#include "FireFlower.h"
#include "Shell.h"
#include "Fireball.h"
#include "MarioComponent.h"
#include "Art.h"
#include "SimpleAudioEngine.h"

#include<iostream>
#include<algorithm>
#include<math.h>
using namespace std;

LevelScene::LevelScene()
{
}

LevelScene::LevelScene(MarioComponent* renderer, int levelDifficulty, int levelType)
{
    this->renderer = renderer;
    this->levelDifficulty = levelDifficulty;
    this->levelType = levelType;
}

LevelScene::~LevelScene()
{

}

void LevelScene::init()
{
    SimpleAudioEngine::getInstance()->stopBackgroundMusic(true);

    if (levelType==LevelGenerator::TYPE_OVERGROUND) {
        SimpleAudioEngine::getInstance()->playBackgroundMusic(Art::songs[1].c_str(), true);
    }
    else if (levelType==LevelGenerator::TYPE_UNDERGROUND) {
        SimpleAudioEngine::getInstance()->playBackgroundMusic(Art::songs[2].c_str(), true);
    }
    else if (levelType==LevelGenerator::TYPE_CASTLE) {
        SimpleAudioEngine::getInstance()->playBackgroundMusic(Art::songs[3].c_str(), true);
    }

    Level::loadBehaviors();
    level.reset(new Level(320, 15));
    LevelGenerator::createLevel(320, 15, levelDifficulty, levelType, level.get());
    paused = false;
    Sprite::levelScene = this;

    layer.reset(new LevelRenderer(level.get(), 640, 480));

    for(int i = 0; i < 2; i++)
    {
        int scrollSpeed = 8 >> i;
        int w = ((level->width * 32) - 640) / scrollSpeed + 640;
        int h = ((level->height * 32) - 480) / scrollSpeed + 480;
        w = w / 64 + 1;
        h = h / 64 + 1;
        bgLayer[i].reset(new BgRenderer(w, h, 640, 480, scrollSpeed));
        BgLevelGenerator::createLevel(w, h, i == 0, levelType, bgLayer[i]->level.get());
    }

    sprites.clear();
    mario.reset(new Mario());
    sprites.push_back(mario);

    startTime = 1;
    timeLeft = 200 * 15;
}

void LevelScene::tick()
{
    timeLeft--;
    if (timeLeft == 0)
    {
        mario->die();
    }
    xCamO = xCam;
    yCamO = yCam;

    if (startTime > 0)
    {
        startTime++;
    }

    float targetXCam = mario->x - 320;

    xCam = targetXCam;

    if (xCam < 0) xCam = 0;
    if (xCam > level->width * 32 - 640)
        xCam = level->width * 32 - 640;

    fireballsOnScreen = 0;

    for (auto sprite : sprites)
    {
        if (sprite != mario)
        {
            float xd = sprite->x - xCam;
            float yd = sprite->y - yCam;
            if (xd < -128 || xd > 640 + 128 || yd < -128 || yd > 480 + 128)
            {
                removeSprite(sprite.get());
            }
            else
            {
                string name = typeid (*(sprite.get())).name();
                if (name.find("Fireball") != string::npos)
                {
                    fireballsOnScreen++;
                }
            }
        }
    }

    if (paused)
    {
        for (auto sprite : sprites)
        {
            if (sprite == mario)
            {
                sprite->tick();
            }
            else
            {
                sprite->tickNoMove();
            }
        }
    }
    else {
       llTick++;
       level->tick();

       bool hasShotCannon = false;
       int xCannon = 0;

       for (int x = (int) xCam / 32 - 1; x <= (int) (xCam + layer->width) / 32 + 1; x++)
       {
           for (int y = (int) yCam / 32 - 1; y <= (int) (yCam + layer->height) / 32 + 1; y++)
           {
               int dir = 0;

               if (x * 32 + 16 > mario->x + 32) dir = -1;
               if (x * 32 + 16 < mario->x - 32) dir = 1;

               SpriteTemplate* st = level->getSpriteTemplate(x, y);

               if (st != nullptr)
               {
                   if (st->lastVisibleTick != llTick - 1)
                   {
                       if (st->sprite == nullptr || !findSprite(st->sprite))
                       {
                           st->spawn(this, x, y, dir);
                       }
                   }

                   st->lastVisibleTick = llTick;
               }

               if (dir != 0)
               {
                   uint8_t b = level->getBlock(x, y);
                   if (((Level::TILE_BEHAVIORS[b & 0xff]) & Level::BIT_ANIMATED) > 0)
                   {
                       if ((b % 16) / 4 == 3 && b / 16 == 0)
                       {
                           if ((llTick - x * 2) % 100 == 0)
                           {
                               xCannon = x;
                               for (int i = 0; i < 8; i++)
                               {
                                   addSprite(new Sparkle(x * 32 + 16, y * 32 + rand() % 32,
                                                         rand() % 1000 / 1000.00 * dir, 0, 0, 1, 5));
                               }
                               addSprite(new BulletBill(x * 32 + 16 + dir * 16, y * 32 + 30, dir));
                               hasShotCannon = true;
                           }
                       }
                   }
               }
           }
       }

       if (hasShotCannon)
       {
           SimpleAudioEngine::getInstance()->playEffect(Art::samples[Art::SAMPLE_CANNON_FIRE].c_str());
       }

       for (auto sprite : sprites)
       {
           sprite->tick();
       }

       for (auto sprite : sprites)
       {
           sprite->collideCheck();
       }

       for (auto shell : shellsToCheck)
       {
           for (auto sprite : sprites)
           {
               if (sprite.get() != shell && !shell->dead)
               {
                   if (sprite->shellCollideCheck(shell))
                   {
                       if (mario->carried == shell && !shell->dead)
                       {
                           mario->carried = nullptr;
                           shell->die();
                       }
                   }
               }
           }
       }
       shellsToCheck.clear();

       for (auto fireball : fireballsToCheck)
       {
           for (auto sprite : sprites)
           {
               if (sprite.get() != fireball && !fireball->dead)
               {
                   if (sprite->fireballCollideCheck(fireball))
                   {
                       fireball->die();
                   }
               }
           }
       }
       fireballsToCheck.clear();
    }

    addAndRemoveSprite();

    spritesToAdd.clear();
    spritesToRemove.clear();
}

void LevelScene::render(QPainter &painter)
{
    int xCam = (int)(mario->xOld) - 320;
    int yCam = (int) (mario->yOld) -240;
    if (xCam < 0) xCam = 0;
    if (yCam < 0) yCam = 0;
    if (xCam > level->width * 32 - 640) xCam = level->width * 32 - 640;
    if (yCam > level->height * 32 - 480) yCam = level->height * 32 - 480;

    for(auto layer : bgLayer){
        layer->render(painter, xCam, yCam);
    }

    QTransform transform;
    transform.translate(-xCam, -yCam);
    painter.setTransform(transform);
    for(auto sprite : sprites){
        if(sprite->layer == 0)
        {
            sprite->render(painter);
        }
    }
    transform.translate(xCam, yCam);
    painter.setTransform(transform);

    layer->render(painter, llTick, xCam, yCam);
    layer->renderExit0(painter, llTick, xCam, yCam, mario->winTime == 0);

    transform.translate(-xCam, -yCam);
    painter.setTransform(transform);
    for(auto sprite : sprites){
        if(sprite->layer == 1)
        {
            sprite->render(painter);
        }
    }
    transform.translate(xCam, yCam);
    painter.setTransform(transform);

    layer->renderExit1(painter, xCam, yCam);

    char szBuff[128];
    sprintf(szBuff, "MARIO %d", Mario::lives);
    drawStringDropShadow(painter, szBuff, 0, 0, 7);
    drawStringDropShadow(painter, "0000000", 0, 1, 7);

    drawStringDropShadow(painter, "COIN", 14, 0, 7);
    sprintf(szBuff, " %d", Mario::coins);
    drawStringDropShadow(painter, szBuff, 14, 1, 7);

    drawStringDropShadow(painter, "WORLD", 24, 0, 7);
    sprintf(szBuff, " %s", Mario::levelString.c_str());
    drawStringDropShadow(painter, szBuff, 24, 1, 7);

    drawStringDropShadow(painter, "TIME", 35, 0, 7);
    int time = (timeLeft + 15 - 1)/15;
    if (time<0) time = 0;
    sprintf(szBuff, " %d", time);
    drawStringDropShadow(painter, szBuff, 35, 1, 7);

    if(startTime > 0)
    {
        float t = startTime - 2;
        t = t * t * 0.6f;
        renderBlackout(painter, 320, 240, (int) (t));
    }

    if (mario->winTime > 0)
    {
        float t = mario->winTime;
        t = t * t * 0.2f;

        if (t > 640)
        {
            t = 640;
            renderer->levelWon();
        }

        renderBlackout(painter, (int) (mario->xDeathPos - xCam),
                       (int) (mario->yDeathPos - yCam), (int) (640 - t));
    }

    if (mario->deathTime > 0)
    {
        float t = mario->deathTime;
        t = t * t * 0.4f;

        if (t > 640)
        {
            t = 640;
            renderer->levelFailed();
        }

        renderBlackout(painter, (int) (mario->xDeathPos - xCam),
                       (int) (mario->yDeathPos - yCam), (int) (640 - t));
    }
}

void LevelScene::bump(int x, int y, bool canBreakBricks)
{
    uint8_t block = level->getBlock(x, y);

    if ((Level::TILE_BEHAVIORS[block & 0xff] & Level::BIT_BUMPABLE) > 0)
    {
        bumpInto(x, y - 1);
        level->setBlock(x, y, (uint8_t) 4);
        level->setBlockData(x, y, (uint8_t) 4);

        int dir = -1;
        if (((Level::TILE_BEHAVIORS[block & 0xff]) & Level::BIT_SPECIAL) > 0)
        {
            SimpleAudioEngine::getInstance()->playEffect(Art::samples[Art::SAMPLE_ITEM_SPROUT].c_str());
            if (!Mario::large)
            {
                addSprite(new Mushroom(x * 32 + 16, y * 32 + 16));
            }
            else
            {
                addSprite(new FireFlower(x * 32 + 16, y * 32 + 16));
            }
        }
        else
        {
            Mario::getCoin();
            SimpleAudioEngine::getInstance()->playEffect(Art::samples[Art::SAMPLE_GET_COIN].c_str());
            addSprite(new CoinAnim(x, y));
        }
    }

    if ((Level::TILE_BEHAVIORS[block & 0xff] & Level::BIT_BREAKABLE) > 0)
    {
        bumpInto(x, y - 1);
        if (canBreakBricks)
        {
            SimpleAudioEngine::getInstance()->playEffect(Art::samples[Art::SAMPLE_BREAK_BLOCK].c_str());
            level->setBlock(x, y, (uint8_t) 0);
            for (int xx = 0; xx < 2; xx++)
                for (int yy = 0; yy < 2; yy++)
                {
                    addSprite(new Particle(x * 32 + xx * 16 + 8, y * 32 + yy * 16 + 8, (xx * 2 - 1) * 8, (yy * 2 - 1) * 8 - 16));
                }
        }
        else
        {
            level->setBlockData(x, y, (uint8_t) 4);
        }
    }
}

void LevelScene::bumpInto(int x, int y)
{
    uint8_t block = level->getBlock(x, y);
    if (((Level::TILE_BEHAVIORS[block & 0xff]) & Level::BIT_PICKUPABLE) > 0)
    {
        Mario::getCoin();
        SimpleAudioEngine::getInstance()->playEffect(Art::samples[Art::SAMPLE_GET_COIN].c_str());
        level->setBlock(x, y, (uint8_t) 0);
        addSprite(new CoinAnim(x, y + 1));
    }

    for (auto sprite : sprites)
    {
        sprite->bumpCheck(x, y);
    }
}

void LevelScene::addSprite(Sprite *sprite)
{
    spritesToAdd.push_back(sprite);
    sprite->tick();
}

void LevelScene::removeSprite(Sprite *sprite)
{
    spritesToRemove.push_back(sprite);
}

bool LevelScene::findSprite(Sprite *sprite)
{
    bool result = false;
    for(auto it : sprites) {
        if(it.get() == sprite)
        {
            result = true;
            break;
        }
    }
    return result;
}

void LevelScene::addAndRemoveSprite()
{
    for(auto it = spritesToAdd.begin(); it != spritesToAdd.end(); ++it)
    {
        if(!findSprite(*it)) {
            sprites.emplace_back(shared_ptr<Sprite>(*it));
        }
    }
    for(auto sp : spritesToRemove)
    {
        auto it = sprites.begin();
        while (it != sprites.end()) {
            if(it->get() == sp){
                it = sprites.erase(it);
            } else {
                ++it;
            }
        }
    }
}

void LevelScene::checkShellCollide(Shell *shell)
{
    shellsToCheck.emplace_back(shell);
}

void LevelScene::checkFireballCollide(Fireball *fireball)
{
    fireballsToCheck.emplace_back(fireball);
}

void LevelScene::renderBlackout(QPainter &painter, int x, int y, int radius)
{
    if (radius > 640.00) return;

    QPolygonF polygon;
    QBrush brush(QColor(0, 0, 0));

    for (int i = 0; i < 16; i++)
    {
        polygon.push_back(QPointF(x + cos(i * M_PI / 15) * radius, y + sin(i * M_PI / 15) * radius));
    }
    polygon.push_back(QPointF(640.00, y));
    polygon.push_back(QPointF(640.00, 480.00));
    polygon.push_back(QPointF(0.00, 480.00));
    polygon.push_back(QPointF(0.00, y));

    for (int i = 0; i < 16; i++)
    {
        polygon.push_back(QPointF(x - cos(i * M_PI / 15) * radius, y - sin(i * M_PI / 15) * radius));
    }
    polygon.push_back(QPointF(640.00, y));
    polygon.push_back(QPointF(640.00, 0.00));
    polygon.push_back(QPointF(0.00, 0.00));
    polygon.push_back(QPointF(0.00, y));
    QPainterPath path;
    path.addPolygon(polygon);
    painter.fillPath(path, brush);
}

void LevelScene::drawStringDropShadow(QPainter &painter, string text, int x, int y, int c)
{
    drawString(painter, text, x * 16 + 10, y * 16 + 10, 0);
    drawString(painter, text, x * 16 + 8, y * 16 + 8, c);
}

void LevelScene::drawString(QPainter &painter, string text, int x, int y, int c)
{
    for(int i = 0; i < text.length(); i++) {
        QImage img = Art::font[text[i] - 32][c];
        QRectF rect(x + i * img.width() * 2, y,
                    img.width() * 2, img.height() * 2);
        painter.drawImage(rect, img);
    }
}
