#ifndef TITLESCENE_H
#define TITLESCENE_H

#include "Scene.h"

#include<array>
#include<memory>
class MarioComponent;
class BgRenderer;

class TitleScene : public Scene
{
public:
    TitleScene(MarioComponent* component);
    virtual void init();
    virtual void tick();
    virtual void render(QPainter& painter);
private:
    MarioComponent* component;
    long long llTick = 0;
    array<shared_ptr<BgRenderer>, 2> bgLayer = {nullptr, nullptr};
    bool wasDown = true;
};

#endif // TITLESCENE_H
