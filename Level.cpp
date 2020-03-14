#include "Level.h"
#include "SpriteTemplate.h"
#include<QDebug>

const char* Level::BIT_DESCRIPTIONS[] = {
										"BLOCK UPPER",
										"BLOCK ALL",
										"BLOCK LOWER",
										"SPECIAL",
										"BUMPABLE",
										"BREAKABLE",
										"PICKUPABLE",
										"ANIMATED"
};

uint8_t Level::TILE_BEHAVIORS[256];

Level::Level()
{

}

Level::Level(int width, int height)
{
	this->width = width;
	this->height = height;
	xExit = 10;
	yExit = 10;

    map.resize(width);
    data.resize(width);
    spriteTemplates.resize(width);
    for(int i = 0; i < width; i++) {
        map[i].resize(height);
        data[i].resize(height);
        spriteTemplates[i].resize(height);
    }
    for(int i = 0; i < width; i++) {
        for(int j = 0; j < height; j++) {
            map[i][j] = 0;
            data[i][j] = 0;
            spriteTemplates[i][j] = nullptr;
        }
    }

}

Level::~Level()
{
    int m = 0;
}

uint8_t Level::getBlock(int x, int y)
{
	if (x < 0) {
		x = 0;
	}
	if (y < 0) {
		y = 0;
	}
	if (x >= width)
	{
		x = width - 1;
	}
	if (y >= height)
	{
		y = height - 1;
    }
	return map[x][y];
}

void Level::setBlock(int x, int y, uint8_t b)
{
	if (x < 0 || y < 0 || x >= width || y >= height) {
		return;
    }

    map[x][y] = b;
}

void Level::setBlockData(int x, int y, uint8_t b)
{
    if (x < 0) return;
    if (y < 0) return;
    if (x >= width) return;
    if (y >= height) return;
    data[x][y] = b;
}

void Level::loadBehaviors()
{
    TILE_BEHAVIORS[0] = 0;
    TILE_BEHAVIORS[1] = 20;
    TILE_BEHAVIORS[2] = 28;
    TILE_BEHAVIORS[3] = 0;
    TILE_BEHAVIORS[4] = 130;
    TILE_BEHAVIORS[5] = 130;
    TILE_BEHAVIORS[6] = 130;
    TILE_BEHAVIORS[7] = 130;
    TILE_BEHAVIORS[8] = 2;
    TILE_BEHAVIORS[9] = 2;
    TILE_BEHAVIORS[10] = 2;
    TILE_BEHAVIORS[11] = 2;
    TILE_BEHAVIORS[12] = 2;
    TILE_BEHAVIORS[13] = 0;
    TILE_BEHAVIORS[14] = 138;
    TILE_BEHAVIORS[15] = 0;
    TILE_BEHAVIORS[16] = 162;
    TILE_BEHAVIORS[17] = 146;
    TILE_BEHAVIORS[18] = 154;
    TILE_BEHAVIORS[19] = 162;
    TILE_BEHAVIORS[20] = 146;
    TILE_BEHAVIORS[21] = 146;
    TILE_BEHAVIORS[22] = 154;
    TILE_BEHAVIORS[23] = 146;
    TILE_BEHAVIORS[24] = 2;
    TILE_BEHAVIORS[25] = 0;
    TILE_BEHAVIORS[26] = 2;
    TILE_BEHAVIORS[27] = 2;
    TILE_BEHAVIORS[28] = 2;
    TILE_BEHAVIORS[29] = 0;
    TILE_BEHAVIORS[30] = 2;
    TILE_BEHAVIORS[31] = 0;
    TILE_BEHAVIORS[32] = 192;
    TILE_BEHAVIORS[33] = 192;
    TILE_BEHAVIORS[34] = 192;
    TILE_BEHAVIORS[35] = 192;
    TILE_BEHAVIORS[36] = 0;
    TILE_BEHAVIORS[37] = 0;
    TILE_BEHAVIORS[38] = 0;
    TILE_BEHAVIORS[39] = 0;
    TILE_BEHAVIORS[40] = 2;
    TILE_BEHAVIORS[41] = 2;
    TILE_BEHAVIORS[42] = 0;
    TILE_BEHAVIORS[43] = 0;
    TILE_BEHAVIORS[44] = 0;
    TILE_BEHAVIORS[45] = 0;
    TILE_BEHAVIORS[46] = 2;
    TILE_BEHAVIORS[47] = 0;
    TILE_BEHAVIORS[48] = 0;
    TILE_BEHAVIORS[49] = 0;
    TILE_BEHAVIORS[50] = 0;
    TILE_BEHAVIORS[51] = 0;
    TILE_BEHAVIORS[52] = 0;
    TILE_BEHAVIORS[53] = 0;
    TILE_BEHAVIORS[54] = 0;
    TILE_BEHAVIORS[55] = 0;
    TILE_BEHAVIORS[56] = 2;
    TILE_BEHAVIORS[57] = 2;

    for (int i = 58; i < 128; i++) {
        TILE_BEHAVIORS[i] = 0;
    }

    TILE_BEHAVIORS[128] = 2;
    TILE_BEHAVIORS[129] = 2;
    TILE_BEHAVIORS[130] = 2;
    TILE_BEHAVIORS[131] = 0;
    TILE_BEHAVIORS[132] = 1;
    TILE_BEHAVIORS[133] = 1;
    TILE_BEHAVIORS[134] = 1;
    TILE_BEHAVIORS[135] = 0;
    TILE_BEHAVIORS[136] = 2;
    TILE_BEHAVIORS[137] = 2;
    TILE_BEHAVIORS[138] = 2;
    TILE_BEHAVIORS[139] = 0;
    TILE_BEHAVIORS[140] = 2;
    TILE_BEHAVIORS[141] = 2;
    TILE_BEHAVIORS[142] = 2;
    TILE_BEHAVIORS[143] = 0;
    TILE_BEHAVIORS[144] = 2;
    TILE_BEHAVIORS[145] = 0;
    TILE_BEHAVIORS[146] = 2;
    TILE_BEHAVIORS[147] = 0;
    TILE_BEHAVIORS[148] = 0;
    TILE_BEHAVIORS[149] = 0;
    TILE_BEHAVIORS[150] = 0;
    TILE_BEHAVIORS[151] = 0;
    TILE_BEHAVIORS[152] = 2;
    TILE_BEHAVIORS[153] = 2;
    TILE_BEHAVIORS[154] = 2;
    TILE_BEHAVIORS[155] = 0;
    TILE_BEHAVIORS[156] = 2;
    TILE_BEHAVIORS[157] = 2;
    TILE_BEHAVIORS[158] = 2;
    TILE_BEHAVIORS[159] = 0;
    TILE_BEHAVIORS[160] = 2;
    TILE_BEHAVIORS[161] = 2;
    TILE_BEHAVIORS[162] = 2;
    TILE_BEHAVIORS[163] = 0;
    TILE_BEHAVIORS[164] = 0;
    TILE_BEHAVIORS[165] = 0;
    TILE_BEHAVIORS[166] = 0;
    TILE_BEHAVIORS[167] = 0;
    TILE_BEHAVIORS[168] = 2;
    TILE_BEHAVIORS[169] = 2;
    TILE_BEHAVIORS[170] = 2;
    TILE_BEHAVIORS[171] = 0;
    TILE_BEHAVIORS[172] = 2;
    TILE_BEHAVIORS[173] = 2;
    TILE_BEHAVIORS[174] = 2;
    TILE_BEHAVIORS[175] = 0;
    TILE_BEHAVIORS[176] = 2;
    TILE_BEHAVIORS[177] = 2;
    TILE_BEHAVIORS[178] = 2;
    TILE_BEHAVIORS[179] = 0;
    TILE_BEHAVIORS[180] = 1;
    TILE_BEHAVIORS[181] = 1;
    TILE_BEHAVIORS[182] = 1;

    for (int i = 183; i < 224; i++) {
        TILE_BEHAVIORS[i] = 0;
    }

    TILE_BEHAVIORS[224] = 1;
    TILE_BEHAVIORS[225] = 1;
    TILE_BEHAVIORS[226] = 1;

    for (int i = 227; i < 256; i++) {
        TILE_BEHAVIORS[i] = 0;
    }
}

void Level::setSpriteTemplate(int x, int y, int type, bool winged)
{
    if (x < 0) return;
    if (y < 0) return;
    if (x >= width) return;
    if (y >= height) return;
    spriteTemplates[x][y].reset(new SpriteTemplate(type, winged));
}

SpriteTemplate *Level::getSpriteTemplate(int x, int y)
{
    if (x < 0) return nullptr;
    if (y < 0) return nullptr;
    if (x >= width) return nullptr;
    if (y >= height) return nullptr;
    return spriteTemplates[x][y].get();
}

void Level::printSpriteTemplateInfo()
{
    for(int i = 0; i < width; i++){
        for(int j = 0; j < height; j++) {
            if(spriteTemplates[i][j] != nullptr) {
                qDebug() << "printSpriteTemplateInfo i:" << i << " j:" << j << " type: " << spriteTemplates[i][j].get()->type << endl;
            }
        }
    }
}

void Level::tick()
{
    for (int x = 0; x < width; x++)
    {
        for (int y = 0; y < height; y++)
        {
            if (data[x][y] > 0)
            {
                data[x][y]--;
            }
        }
    }
}

uint8_t Level::getBlockCapped(int x, int y)
{
    if (x < 0) x = 0;
    if (y < 0) y = 0;
    if (x >= width) x = width - 1;
    if (y >= height) y = height - 1;
    return map[x][y];
}

bool Level::isBlocking(int x, int y, float xa, float ya)
{
    uint8_t block = getBlock(x, y);
    bool blocking = ((TILE_BEHAVIORS[block & 0xff]) & BIT_BLOCK_ALL) > 0;
    blocking |= (ya > 0) && ((TILE_BEHAVIORS[block & 0xff]) & BIT_BLOCK_UPPER) > 0;
    blocking |= (ya < 0) && ((TILE_BEHAVIORS[block & 0xff]) & BIT_BLOCK_LOWER) > 0;
    return blocking;
}
