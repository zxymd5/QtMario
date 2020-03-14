#ifndef LEVELRENDERER_H
#define LEVELRENDERER_H
#include<QPainter>

class Level;

class LevelRenderer
{
public:
    LevelRenderer(Level* level, int width, int height);

    void render(QPainter& painter, long long tick, int xCam, int yCam);
    void renderExit0(QPainter& painter, long long tick, int xCam, int yCam, bool bar);
    void renderExit1(QPainter& painter, int xCam, int yCam);
    int width;
    int height;
private:
    Level* level;
};

#endif // LEVELRENDERER_H
