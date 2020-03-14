#ifndef BGRENDERER_H
#define BGRENDERER_H

#include<memory>

using namespace std;

class Level;
class QPainter;

class BgRenderer
{
public:
    BgRenderer(int levelWidth, int levelHeight, int width, int height, int distance);
    void render(QPainter& painter, int xCam, int yCam);
    shared_ptr<Level> level;
private:
    int width;
    int height;
    int distance;
};

#endif // BGRENDERER_H
