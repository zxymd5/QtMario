#include "LoseScene.h"
#include "MarioComponent.h"
#include "Art.h"

LoseScene::LoseScene(MarioComponent *component)
{
    this->component = component;
}

void LoseScene::render(QPainter &painter)
{
    QBrush brush(QColor(0xa0, 0x70, 0x70));
    painter.fillRect(QRectF(0, 0, 640, 480), brush);
    int f = llTick/3%10;
    if (f>=6) f = 10-f;
    painter.drawImage(QRectF(320-96, 200-64,
                             Art::gameOver[f][0].width() * 2,
                      Art::gameOver[f][0].height() * 2), Art::gameOver[f][0]);
    drawString(painter, scrollMessage, 320-scrollMessage.length()*8, 320, 0);
}

void LoseScene::tick()
{
    llTick++;
    if (!wasDown && keys[Qt::Key_S])
    {
        component->toTitle();
    }
    if (keys[Qt::Key_S])
    {
        wasDown = false;
    }
}

void LoseScene::drawString(QPainter &painter, string text, int x, int y, int c)
{
    for(int i = 0; i < text.length(); i++) {
        QImage img = Art::font[text[i] - 32][c];
        QRectF rect(x + i * img.width() * 2, y,
                    img.width() * 2, img.height() * 2);
        painter.drawImage(rect, img);
    }
}
