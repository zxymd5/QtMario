#include "Art.h"
#include <iostream>
#include <QFileInfo>
#include <QDebug>

vector<vector<QImage>> Art::mario;
vector<vector<QImage>> Art::smallMario;
vector<vector<QImage>> Art::fireMario;
vector<vector<QImage>> Art::enemies;
vector<vector<QImage>> Art::items;
vector<vector<QImage>> Art::level;
vector<vector<QImage>> Art::particles;
vector<vector<QImage>> Art::font;
vector<vector<QImage>> Art::bg;
vector<vector<QImage>> Art::map;
vector<vector<QImage>> Art::endScene;
vector<vector<QImage>> Art::gameOver;
QImage Art::logo;
QImage Art::titleScreen;
QImage Art::tip;
vector<string> Art::songs;
vector<string> Art::samples;

void Art::init()
{
    cutImage(":/resources/mariosheet.png", 32, 32, mario);
    cutImage(":/resources/smallmariosheet.png", 16, 16, smallMario);
    cutImage(":/resources/firemariosheet.png", 32, 32, fireMario);
    cutImage(":/resources/enemysheet.png", 16, 32, enemies);
    cutImage(":/resources/itemsheet.png", 16, 16, items);
    cutImage(":/resources/mapsheet.png", 16, 16, level);
    cutImage(":/resources/particlesheet.png", 8, 8, particles);
    cutImage(":/resources/font.gif", 8, 8, font);
    cutImage(":/resources/bgsheet.png", 32, 32, bg);
    cutImage(":/resources/worldmap.png", 16, 16, map);
    cutImage(":/resources/endscene.gif", 96, 96, endScene);
    cutImage(":/resources/gameovergost.gif", 96, 64, gameOver);

    logo.load(":/resources/logo.gif");
    titleScreen.load(":/resources/title.gif");
    tip.load(":/resources/tip.png");

    string basePath = QFileInfo(__FILE__).absolutePath().toStdString();


    samples.resize(14);
    samples[SAMPLE_BREAK_BLOCK] = basePath + "/resources/snd/breakblock.wav";
    samples[SAMPLE_GET_COIN] = basePath + "/resources/snd/coin.wav";
    samples[SAMPLE_MARIO_JUMP] = basePath + "/resources/snd/jump.wav";
    samples[SAMPLE_MARIO_STOMP] = basePath + "/resources/snd/stomp.wav";
    samples[SAMPLE_MARIO_KICK] = basePath + "/resources/snd/kick.wav";
    samples[SAMPLE_MARIO_POWER_UP] = basePath + "/resources/snd/powerup.wav";
    samples[SAMPLE_MARIO_POWER_DOWN] = basePath + "/resources/snd/powerdown.wav";
    samples[SAMPLE_MARIO_DEATH] = basePath + "/resources/snd/death.wav";
    samples[SAMPLE_ITEM_SPROUT] = basePath + "/resources/snd/sprout.wav";
    samples[SAMPLE_CANNON_FIRE] = basePath + "/resources/snd/cannon.wav";
    samples[SAMPLE_SHELL_BUMP] = basePath + "/resources/snd/bump.wav";
    samples[SAMPLE_LEVEL_EXIT] = basePath + "/resources/snd/exit.wav";
    samples[SAMPLE_MARIO_1UP] = basePath + "/resources/snd/1-up.wav";
    samples[SAMPLE_MARIO_FIREBALL] = basePath + "/resources/snd/fireball.wav";

    songs.resize(7);
    songs[0] = basePath + "/resources/mus/smb3map1.mp3";
    songs[1] = basePath + "/resources/mus/smwovr1.mp3";
    songs[2] = basePath + "/resources/mus/smb3undr.mp3";
    songs[3] = basePath + "/resources/mus/smwfortress.mp3";
    songs[4] = basePath + "/resources/mus/smwtitle.mp3";
    songs[5] = basePath + "/resources/mus/smb3ovr1.mp3";
    songs[6] = basePath + "/resources/mus/smwovr2.mp3";
}

void Art::cutImage(const QString &path, int xSize, int ySize, vector<vector<QImage> > &images)
{
    QImage image(path);
    int width = image.width() / xSize;
    int height = image.height() / ySize;

    images.resize(width);

    for(int i = 0; i < width; i++) {
        images[i].resize(height);
    }

    for (int x = 0; x < width; x++)
    {
        for (int y = 0; y < height; y++)
        {
            images.at(x).at(y) = image.copy(x * xSize, y * ySize, xSize, ySize);
        }
    }
}

