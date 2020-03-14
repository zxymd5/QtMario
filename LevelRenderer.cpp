#include "LevelRenderer.h"
#include "Level.h"
#include "Art.h"
#include<math.h>
#include<iostream>

LevelRenderer::LevelRenderer(Level *level, int width, int height)
{
    this->width = width;
    this->height = height;
    this->level = level;
}

void LevelRenderer::render(QPainter &painter, long long tick, int xCam, int yCam)
{
    //从xCam,yCam处开始
    int xTileStart = xCam / 32;
    int yTileStart = yCam / 32;
    int xTileEnd = (xCam + width) / 32;
    int yTileEnd = (yCam + height) / 32;
    for (int x = xTileStart; x <= xTileEnd; x++)
    {
        for (int y = yTileStart; y <= yTileEnd; y++)
        {
            int b = level->getBlock(x, y) & 0xff;
            if (((Level::TILE_BEHAVIORS[b]) & Level::BIT_ANIMATED) == 0)
            {
                QRectF rect((x << 5) - xCam, (y << 5) - yCam, 2 * Art::level[b % 16][b / 16].width(), 2 * Art::level[b % 16][b / 16].height());
                painter.drawImage(rect, Art::level[b % 16][b / 16]);
            }
            if (((Level::TILE_BEHAVIORS[b]) & Level::BIT_ANIMATED) > 0)
            {
                int animTime = (tick / 3) % 4;

                if ((b % 16) / 4 == 0 && b / 16 == 1)
                {
                    animTime = (tick / 2 + (x + y) / 8) % 20;
                    if (animTime > 3) animTime = 0;
                }
                if ((b % 16) / 4 == 3 && b / 16 == 0)
                {
                    animTime = 2;
                }
                int yo = 0;
                if (x >= 0 && y >= 0 && x < level->width && y < level->height) yo = level->data[x][y];
                if(yo > 0) {
                    int m = 5;
                }
                if (yo > 0) yo = (int) (sin((yo) / 4.0f * M_PI) * 16);
                QRectF rect((x << 5) - xCam, (y << 5) - yCam - yo, 2 * Art::level[(b % 16) / 4 * 4 + animTime][b / 16].width(), 2 * Art::level[(b % 16) / 4 * 4 + animTime][b / 16].height());
                painter.drawImage(rect, Art::level[(b % 16) / 4 * 4 + animTime][b / 16]);
            }
        }
    }

}

void LevelRenderer::renderExit0(QPainter &painter, long long tick, int xCam, int yCam, bool bar)
{
    for (int y = level->yExit - 8; y < level->yExit; y++)
    {
        int col = (y == level->yExit - 8 ? 4 : 5);
        QRectF rect((level->xExit << 5) - xCam - 32, (y << 5) - yCam,
                    2 * Art::level[12][col].width(),
                2 * Art::level[12][col].height());
        painter.drawImage(rect, Art::level[12][col]);
    }
    int yh = level->yExit * 32 - (int) ((sin(tick / 20) * 0.5 + 0.5) * 7 * 32) - 16;
    if (bar)
    {
        QRectF rectFirst((level->xExit << 5) - xCam - 32, yh - yCam,
                    2 * Art::level[12][3].width(),
                2 * Art::level[12][3].height());
        painter.drawImage(rectFirst, Art::level[12][3]);
        QRectF rectSecond((level->xExit << 5) - xCam, yh - yCam,
                    2 * Art::level[13][3].width(),
                2 * Art::level[13][3].height());
        painter.drawImage(rectSecond, Art::level[13][3]);
    }
}

void LevelRenderer::renderExit1(QPainter &painter, int xCam, int yCam)
{
    for (int y = level->yExit - 8; y < level->yExit; y++)
    {
        int col = (y == level->yExit - 8 ? 4 : 5);
        QRectF rect((level->xExit << 5) - xCam + 32, (y << 5) - yCam,
                    2 * Art::level[13][col].width(),
                2 * Art::level[13][col].height());
        painter.drawImage(rect, Art::level[13][col]);
    }
}
