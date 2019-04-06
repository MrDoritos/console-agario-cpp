#pragma once
#include "playerOrb.h"
#include "agarioGameMap.h"
#include "box.h"
#include "boxsize.h"
#include "gamecamera.h"
#include "rigidbody.h"
//#include "agarioGame.h"
class agarioGame;

playerOrb::playerOrb(gamecamera& gCamera, agarioGameMap& gameMap, agarioGame& game)
{
	this->gameCamera = &gCamera;
	this->gameMap = &gameMap;
	this->game = &game;
	positionX = 10.0f;
	positionY = 10.0f;	
	score = 10.0f;
	this->angle = 0.0f;
	this->velocity = 2.0f;

	this->consumee = false;
	this->consumer = false;
	this->_collides = false;
}

playerOrb::playerOrb(gamecamera& gCamera, agarioGameMap& gameMap, agarioGame& game, float score)
{
	this->gameCamera = &gCamera;
	this->gameMap = &gameMap;
	this->game = &game;
	positionX = 10.0f;
	positionY = 10.0f;
	this->score = score;
	this->angle = 0.0f;
	this->velocity = 2.0f;

	this->consumee = false;
	this->consumer = false;
	this->_collides = false;
}

playerOrb::playerOrb()
{
}

playerOrb::~playerOrb()
{
}

float playerOrb::getRadius(float scale)
{
	return (getDiameter(scale) / 2);
}

float playerOrb::getRadius()
{
	return (getDiameter() / 2);
}

float playerOrb::getDiameter(float scale)
{
	return (score / PI) / scale;
}

float playerOrb::getDiameter()
{
	return (score / PI);
}

float playerOrb::centerX()
{
	return positionX + getRadius();
}

float playerOrb::centerY()
{
	return positionY + getRadius();
}

float playerOrb::centerX(float scale)
{
	return centerX() / scale;
}

float playerOrb::centerY(float scale)
{
	return centerY() / scale;
}

float playerOrb::maxX()
{
	return positionX + getDiameter();
}

float playerOrb::maxY()
{
	return positionY + getDiameter();
}

float playerOrb::minX()
{
	return positionX;
}

float playerOrb::minY()
{
	return positionY;
}

float playerOrb::getDrawScale()
{
	float minScale = gameCamera->radius();
	return minScale / score;
}

void playerOrb::setCenter(float x, float y)
{
	if (inBounds(x, y)) {
		positionX = x - getRadius();
		positionY = y - getRadius();
	}
}

void playerOrb::setCenter(int x, int y)
{
	if (inBounds(x, y)) {
		positionX = x - getRadius();
		positionY = y - getRadius();
	}
}

void playerOrb::setCenterX(int x)
{
	if (inBounds(x, centerY())) {
		positionX = x - getRadius();
	}
}

void playerOrb::setCenterX(float x)
{
	if (inBounds(x, centerY())) {
		positionX = x - getRadius();
	}
}

void playerOrb::setCenterY(int y)
{
	if (inBounds(centerX(), y)) {
		positionY = y - getRadius();
	}
}

void playerOrb::setCenterY(float y)
{
	if (inBounds(centerX(), y)) {
		positionY = y - getRadius();
	}
}

void playerOrb::update(float elapsedTime)
{
	float centerXf = centerX();
	float centerYf = centerY();
	float nextX = (centerXf + elapsedTime * velocity * cos(rigidbody::degtorad(angle)));
	float nextY = (centerYf + elapsedTime * velocity * sin(rigidbody::degtorad(angle)));

	if (inBounds(nextX, nextY)) {
		setCenterX(nextX);
		setCenterY(nextY);
	}
	else {
		if (inBounds(nextX, centerY())) {
			setCenterX(nextX);
		}
		else {
			setCenterX(getRadius());
		}

		if (inBounds(centerX(), nextY)) {
			setCenterY(nextY);
		}
		else {
			setCenterY(getRadius());
		}
	}

}

bool playerOrb::inBounds(float centerX, float centerY)
{
	float minX = centerX - getRadius();
	float minY = centerY - getRadius();
	float maxX = getRadius() + centerX;
	float maxY = getRadius() + centerY;

	return (
		minX >= 0.0f &&
		minY >= 0.0f &&
		maxX >= 0.0f &&
		maxY >= 0.0f &&
		minX < gameMap->getMapSize() &&
		minY < gameMap->getMapSize() &&
		maxX < gameMap->getMapSize() &&
		maxY < gameMap->getMapSize());
}

bool playerOrb::needToDraw()
{
	return true;
}

void playerOrb::drawToCamera()
{
	drawTo(*gameCamera);
}

void playerOrb::drawTo(gamecamera & gameCamera)
{
	int boundsX = gameCamera.screen->sizeX, boundsY = gameCamera.screen->sizeY;
	float scale = gameCamera.scale();
	float playerScreenMinX = minX() / scale;
	float playerScreenMinY = minY() / scale;

	float cameraRadius = gameCamera.radius() / scale;
	float cameraDiameter = cameraRadius * 2;
	float cameraScreenMinX = (gameCamera.minX() / scale);// - cameraRadius;
	float cameraScreenMaxX = cameraScreenMinX + (cameraRadius / 2);
	float cameraScreenMinY = (gameCamera.minY() / scale);// -cameraRadius;
	float cameraScreenMaxY = cameraScreenMinY + (cameraRadius / 2);


	float playerRadius = getRadius(scale);
	float playerDiameter = playerRadius * 2;

	float playerCenterX = playerScreenMinX + playerDiameter;
	float playerCenterY = playerScreenMinY + playerDiameter;

	char* fb = gameCamera.framebuffer();

	//Draw circle

	float step = 0.001f;
	int x, y;

	for (float theta = 0.0f; theta < DOUBLE_PI; theta += step) {
		x = roundf(((playerCenterX + playerRadius * cos(theta))) - cameraScreenMaxX);// -gameCamera.minX();
		y = roundf(((playerCenterY - playerRadius * sin(theta))) - cameraScreenMaxY);// -cameraScreenMinY;
		
		if (x < boundsX && y < boundsY && y >= 0 && x >= 0) {
			fb[gameCamera.get(x, y)] = '#';
		}
	}

	//Draw velocity
#if defined DEBUG || defined _DEBUG
	for (float cradius = 0.0f; cradius < playerRadius; cradius += step) {
		x = (playerCenterX + cradius * cos(rigidbody::degtorad(angle))) - cameraScreenMaxX;
		y = (playerCenterY + cradius * sin(rigidbody::degtorad(angle))) - cameraScreenMaxY;
		if (x < boundsX && y < boundsY && y >= 0 && x >= 0) {
			fb[gameCamera.get(x, y)] = '*';
		}
	}
#endif

	//float scale = gameCamera.scale();
	//int radius = getRadius(scale);
	//float diameter = getDiameter(scale);
	//int fCenterX = (minX()/ scale) + radius, fCenterY = maxY() + radius;
	//int boundsX = gameCamera.screen->sizeX, boundsY = gameCamera.screen->sizeY;
	//char* fB = gameCamera.framebuffer();

	////Draw Orb

	//float step = 0.03f;
	//int x, y;
	//for (float theta = 0.0; theta < DOUBLE_PI; theta += step) {
	//	x = ((fCenterX + radius * cos(theta))) -gameCamera.radius();
	//	y = ((fCenterY - radius * sin(theta))) -gameCamera.radius();
	//	if (x < boundsX && y < boundsY && y >= 0 && x >= 0) {
	//		fB[gameCamera.get(x, y)] = '#';
	//	}
	//}

	//Draw Velocity

	//for (float cradius = 0.0f; cradius < radius; cradius += step) {
	//	x = (fCenterX + cradius * cos(rigidbody::degtorad(angle))) - gameCamera.centerX();
	//	y = (fCenterY + cradius * sin(rigidbody::degtorad(angle))) - gameCamera.centerY();
	//	if (x < boundsX && y < boundsY && y >= 0 && x >= 0) {
	//		fB[gameCamera.get(x, y)] = '*';
	//	}
	//}
}

bool playerOrb::collides(playerOrb & player)
{
	float d = rigidbody::distance(this->centerX(), player.centerX(), this->centerY(), player.centerY());
#if defined DEBUG || defined _DEBUG
	float rad = player.getRadius() + this->getRadius();
	return rad >= d;
#else
	return (player.getRadius() + this->getRadius() >= d);
#endif
}

bool playerOrb::lessthan(playerOrb* player) {
	return (player->score > this->score);
}

float playerOrb::getOverlap(playerOrb* player) {
	if (this->lessthan(player)) {
		return this->getRadius() / 2;
	}
	else {
		return player->getRadius() / 2;
	}
}

bool playerOrb::collides(playerOrb* player) {
	float d = rigidbody::distance(this->centerX(), player->centerX(), this->centerY(), player->centerY());
#if defined DEBUG || defined _DEBUG
	float rad = (player->getRadius() + this->getRadius()) - getOverlap(player);
	return rad >= d;
#else
	return (player->getRadius() + this->getRadius() >= d);
#endif
}
