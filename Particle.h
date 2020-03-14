#ifndef PARTICLE_H
#define PARTICLE_H

#include "Sprite.h"

class Particle : public Sprite
{
public:
    Particle(int x, int y, float xa, float ya);
    Particle(int x, int y, float xa, float ya, int xPic, int yPic);
    void move();
    int life;
};

#endif // PARTICLE_H
