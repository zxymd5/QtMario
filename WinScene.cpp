#include "WinScene.h"
#include "MarioComponent.h"
#include "Art.h"

WinScene::WinScene(MarioComponent *component)
{
    this->component = component;
}

void WinScene::render(QPainter &painter)
{
    QBrush brush(QColor(0x80, 0x80, 0xa0));
    painter.fillRect(QRectF(0, 0, 640, 480), brush);
    int f = llTick/24%2;
    painter.drawImage(QRectF(320-96, 200-96,
                             Art::endScene[f][0].width() * 2,
                      Art::endScene[f][0].height() * 2), Art::endScene[f][0]);
    drawString(painter, scrollMessage, 320-scrollMessage.length()*8, 320, 0);
}

void WinScene::tick()
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

void WinScene::drawString(QPainter &painter, string text, int x, int y, int c)
{
    for(int i = 0; i < text.length(); i++) {
        QImage img = Art::font[text[i] - 32][c];
        QRectF rect(x + i * img.width() * 2, y,
                    img.width() * 2, img.height() * 2);
        painter.drawImage(rect, img);
    }
}
