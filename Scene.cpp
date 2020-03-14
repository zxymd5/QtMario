#include "Scene.h"

map<int, bool> Scene::keys;

Scene::Scene()
{

}

Scene::~Scene()
{

}

void Scene::toggleKey(int key, bool isPressed)
{
    if(keys.find(key) != keys.end())
    {
        keys[key] = isPressed;
    } else {
        keys.insert(pair<int, bool>(key, isPressed));
    }
}

void Scene::init()
{

}

void Scene::tick()
{

}

void Scene::render(QPainter &painter)
{

}
