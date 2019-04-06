#pragma once
#include "playerOrb.h"
#include "gamecamera.h"
#include "box.h"
#include "agarioGameMap.h"

gamecamera::gamecamera(box & screen, playerOrb & player, agarioGameMap & gameMap)
{
	this->screen = &screen;
	this->player = &player;
	this->gameMap = &gameMap;
}

gamecamera::~gamecamera()
{
}

void gamecamera::updateView()
{
	if (absMinX() <= 0.0f) {
		positionX = 0.0f;
	} else
		if (absMinX() > 0.0f && absMaxX() < gameMap->getMapSize()) {
			positionX = minX();
		}

	if (absMinY() <= 0.0f) {
		positionY = 0.0f;
	} else
		if (absMinY() > 0.0f && absMaxY() < gameMap->getMapSize()) {
			positionY = minY();
		}
}

float gamecamera::maxX()
{
	float fAbsMaxX = absMaxX();
	float fAbsMapX = gameMap->getMapSize();
	return fAbsMaxX > fAbsMapX ? fAbsMapX : fAbsMaxX;
}

float gamecamera::absMaxX()
{
	return (player->maxX() + (player->getRadius(/*player->getDrawScale()*/)));
}

float gamecamera::maxY()
{
	float fAbsMaxY = absMaxY();
	float fAbsMapY = gameMap->getMapSize();
	return fAbsMaxY > fAbsMapY ? fAbsMapY : fAbsMaxY;
}

float gamecamera::absMaxY()
{
	return (player->maxY() + (player->getRadius(/*player->getDrawScale()*/)));
}

float gamecamera::minX()
{
	float fAbsMinX = absMinX();
	return (fAbsMinX > 0.0f ? fAbsMinX : 0.0f);
}

float gamecamera::absMinX()
{
	float val = (player->minX() - (player->getRadius(/*player->getDrawScale()*/)));
	return val;
}

float gamecamera::minY()
{
	float fAbsMinY = absMinY();
	return (fAbsMinY > 0.0f ? fAbsMinY : 0.0f);
}

float gamecamera::absMinY()
{
	return (player->minY() - (player->getRadius(/*player->getDrawScale()*/)));
}

float gamecamera::sizeX()
{
	return (player->getDiameter() * 2);
}

float gamecamera::sizeY()
{
	return (player->getDiameter() * 2);
}

float gamecamera::centerX()
{
	return (positionX + radius());
}

float gamecamera::centerY()
{
	return (positionY + radius());
}

float gamecamera::diameter()
{
	return (player->getDiameter() * 2);
}

float gamecamera::radius()
{
	return (player->getDiameter());
}

float gamecamera::scale()
{
	return (this->sizeX() / screen->getMin());
	//return (player->getDrawScale());
}
