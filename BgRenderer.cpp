#include "BgRenderer.h"
#include "Level.h"
#include "Art.h"

#include<QPainter>
#include<QPixmap>

BgRenderer::BgRenderer(int levelWidth, int levelHeight, int width, int height, int distance)
{
    this->distance = distance;
    this->width = width;
    this->height = height;
    this->level.reset(new Level(levelWidth, levelHeight));
}

void BgRenderer::render(QPainter &painter, int xCam, int yCam)
{
    xCam /= distance;
    yCam /= distance;
    int xTileStart = xCam / 64;
    int yTileStart = yCam / 64;
    int xTileEnd = (xCam + width) / 64;
    int yTileEnd = (yCam + height) / 64;

    for(int x = xTileStart; x <= xTileEnd; x++)
    {
        for(int y = yTileStart; y <= yTileEnd; y++)
        {
            int b = level->getBlock(x, y) & 0xff;
            QRectF rect((x << 6) - xCam, (y << 6) - yCam - 32, 2 * Art::bg[b % 8][b / 8].width(), 2 * Art::bg[b % 8][b / 8].height());
            painter.drawImage(rect, Art::bg[b % 8][b / 8]);
        }
    }
}
