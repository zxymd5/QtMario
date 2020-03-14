#include "CoinAnim.h"
#include "Art.h"
#include "LevelScene.h"
#include "Sparkle.h"
#include <math.h>

CoinAnim::CoinAnim(int xTile, int yTile)
{
    sheet = Art::level;
    wPic = hPic = 32;

    x = xTile * 32;
    y = yTile * 32 - 32;
    xa = 0;
    ya = -6.0f;
    xPic = 0;
    yPic = 2;
}

void CoinAnim::move()
{
    if (life-- < 0)
    {
        Sprite::levelScene->removeSprite(this);
        for (int xx = 0; xx < 2; xx++)
            for (int yy = 0; yy < 2; yy++)
                Sprite::levelScene->addSprite(new Sparkle(x + xx * 16 + rand() % 16,
                                                          y + yy * 16 + rand() % 16, 0, 0, 0, 2, 5));
    }

    xPic = life & 3;

    x += xa;
    y += ya;
    ya += 2;
}
