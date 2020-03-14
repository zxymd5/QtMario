#ifndef MARIO_IMPROVEDNOISE
#define MARIO_IMPROVEDNOISE

#include<stdio.h>

class ImprovedNoise
{
public:
	ImprovedNoise();
	double noise(double x, double y, double z);
	double fade(double t);
	double lerp(double t, double a, double b);
	double grad(int hash, double x, double y, double z);
	double perlinNoise(double x, double y);
	void shuffle();
private:
	int p[512];
};

#endif // !MARIO_IMPROVEDNOISE
