#ifndef SCENE_H
#define SCENE_H

#include<map>
#include<QPainter>

using namespace std;

class Scene
{
public:
    static map<int, bool> keys;
    explicit Scene();
    virtual ~Scene();
    void toggleKey(int key, bool isPressed);
    virtual void init();
    virtual void tick();
    virtual void render(QPainter& painter);
};

#endif // SCENE_H
