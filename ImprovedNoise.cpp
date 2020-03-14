#include "ImprovedNoise.h"
#include<string.h>
#include<time.h>
#include<stdlib.h>
#include<math.h>

ImprovedNoise::ImprovedNoise()
{
	memset(p, 0, sizeof(p));
	shuffle();
}

double ImprovedNoise::noise(double x, double y, double z)
{
	int X = (int)floor(x) & 255;
	int Y = (int)floor(y) & 255;
	int Z = (int)floor(z) & 255;
	x -= floor(x);
	y -= floor(y);
	z -= floor(z);
	double u = fade(x), // COMPUTE FADE CURVES
		v = fade(y), // FOR EACH OF X,Y,Z.
		w = fade(z);
	int A = p[X] + Y, AA = p[A] + Z, AB = p[A + 1] + Z, // HASH COORDINATES OF
		B = p[X + 1] + Y, BA = p[B] + Z, BB = p[B + 1] + Z; // THE 8 CUBE CORNERS,

	return lerp(w, lerp(v, lerp(u, grad(p[AA], x, y, z), // AND ADD
		grad(p[BA], x - 1, y, z)), // BLENDED
		lerp(u, grad(p[AB], x, y - 1, z), // RESULTS
			grad(p[BB], x - 1, y - 1, z))),// FROM  8
		lerp(v, lerp(u, grad(p[AA + 1], x, y, z - 1), // CORNERS
			grad(p[BA + 1], x - 1, y, z - 1)), // OF CUBE
			lerp(u, grad(p[AB + 1], x, y - 1, z - 1), grad(p[BB + 1], x - 1, y - 1, z - 1))));
}

double ImprovedNoise::fade(double t)
{
	return t * t * t * (t * (t * 6 - 15) + 10);
}

double ImprovedNoise::lerp(double t, double a, double b)
{
	return a + t * (b - a);
}

double ImprovedNoise::grad(int hash, double x, double y, double z)
{
	int h = hash & 15; // CONVERT LO 4 BITS OF HASH CODE
	double u = h < 8 ? x : y, // INTO 12 GRADIENT DIRECTIONS.
		v = h < 4 ? y : h == 12 || h == 14 ? x : z;
	return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
}

double ImprovedNoise::perlinNoise(double x, double y)
{
	double n = 0;

	for (int i = 0; i < 8; i++)
	{
		double stepSize = 64.0 / ((1 << i));
		n += noise(x / stepSize, y / stepSize, 128) * 1.0 / (1 << i);
	}

	return n;
}

void ImprovedNoise::shuffle()
{
	int permutation[256];
	for (int i = 0; i < 256; i++)
	{
		permutation[i] = i;
	}

	srand((int)time(0));
	for (int i = 0; i < 256; i++)
	{
		int j = rand() % (256 - i) + i;
		int tmp = permutation[i];
		permutation[i] = permutation[j];
		permutation[j] = tmp;
		p[i + 256] = p[i] = permutation[i];
	}
}
