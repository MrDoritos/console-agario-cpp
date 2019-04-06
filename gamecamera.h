#pragma once
#include "agarioGameMap.h"
#include "box.h"
#include "boxsize.h"
#include "agarioGame.h"
#include "playerOrb.h"

class gamecamera
{
public:
	gamecamera(box& screen, playerOrb& player, agarioGameMap& gameMap);
	~gamecamera();
	void updateView();
	float maxX();
	float absMaxX();
	float maxY();
	float absMaxY();
	float minX();
	float absMinX();
	float minY();
	float absMinY();
	float sizeX();
	float sizeY();
	float centerX();
	float centerY();
	float diameter();
	float radius();
	float scale();
	char* framebuffer() { return screen->getFramebuffer(); }
	int get(int x, int y) { return screen->get(x, y); }
	box* screen;
	playerOrb* player;
protected:
	agarioGameMap* gameMap;
	float positionX;
	float positionY;
};

