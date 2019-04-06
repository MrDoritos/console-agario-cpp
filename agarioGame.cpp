#pragma once
#include "agarioGame.h"
#include "boxsize.h"
#include "playerOrb.h"
#include "gamecamera.h"
#include "agarioGameMap.h"
#include "gameEngine.h"
#include "box.h"
#include <string>
#include <Windows.h>

void agarioGame::Game()
{
	boxsize cameraSize(0,0,60,60);
	int framebuffersize = cameraSize.characters();
	char* framebuffer = (char*)alloca(sizeof(char) * framebuffersize);
	box gamecamerab(cameraSize, framebuffer, framebuffersize, false);
	gamecamera* cam = (gamecamera*)alloca(sizeof(gamecamera));
	CHAR_INFO* charinfoframebuffer = (CHAR_INFO*)alloca(sizeof(CHAR_INFO) * framebuffersize);
	playerOrb* players = (playerOrb*)alloca(sizeof(playerOrb) * 2);
	*cam = gamecamera(gamecamerab, players[0], *(agarioGameMap*)(void*)nullptr);
	agarioGameMap map(100);
	agarioGame game(cam, charinfoframebuffer, players, 2, &map);
	game.Start();
}

agarioGame::agarioGame(gamecamera* gamecamerad, CHAR_INFO* fb, playerOrb* players, int numberOfPlayers, agarioGameMap* map) : 
gameEngine(gamecamerad->screen, GetConsoleWindow(), fb, false)/*,
gameMap(100),
gameCamera(*gamecamera, gamePlayers[0], gameMap)*/
{
	SetTargetTicksPerSecond(100);
	this->gameCamera = gamecamerad;
	this->gamePlayers = players;
	this->numberOfPlayers = numberOfPlayers;
	this->gameMap = map;
	consoleOut = GetStdHandle(STD_OUTPUT_HANDLE);
	consoleIn = GetStdHandle(STD_INPUT_HANDLE);
	ConstructConsole(gamecamerad->screen->sizeX, gamecamerad->screen->sizeY, 8, 8, (wchar_t*)L"Raster");
	//SetConsoleActiveScreenBuffer(consoleOut = CreateBuffer());
	doClear = true;
}

bool agarioGame::OnUpdate(float elapsedtime)
{
	if (keys[VK_ESCAPE].bReleased) {
		Close();
	}
	else if (keys[VK_OEM_MINUS].bReleased || keys['-'].bReleased) {
		gamePlayers[0].setScore(gamePlayers[0].getScore() - 1.0f);
	}
	else if (keys[VK_ADD].bReleased || keys['+'].bReleased) {
		gamePlayers[0].setScore(gamePlayers[0].getScore() + 1.0f);
	}
	CollisionDetection();
	//Sum shit

	float scale = gameCamera->scale();
	float playerScreenMinX = gamePlayers[0].minX() / scale;
	float playerScreenMinY = gamePlayers[0].minY() / scale;

	float cameraRadius = gameCamera->radius() / scale;
	float cameraDiameter = cameraRadius * 2;
	float cameraScreenMinX = (gameCamera->minX() / scale);// - cameraRadius;
	float cameraScreenMaxX = cameraScreenMinX + (cameraRadius / 2);
	float cameraScreenMinY = (gameCamera->minY() / scale);// -cameraRadius;
	float cameraScreenMaxY = cameraScreenMinY + (cameraRadius / 2);


	float playerRadius = gamePlayers[0].getRadius(scale);
	float playerDiameter = playerRadius * 2;

	float playerCenterX = playerScreenMinX + playerRadius;
	float playerCenterY = playerScreenMinY + playerRadius;

	//
	mousePos.SetCalcPos((playerCenterX) - cameraScreenMinX, (playerCenterY) - cameraScreenMinY);
	mousePos.UpdatePos(getMouseX(), getMouseY());
	gamePlayers[0].degChangeAngle(mousePos.Degrees());
	gamePlayers[0].update(elapsedtime);
	//gamePlayers[1].update(0.020f);
	return false;
}

void agarioGame::OnClose()
{
	
}

bool agarioGame::OnCreate()
{
	gamePlayers[0] = playerOrb(*gameCamera, *gameMap, *this, 15.0f);
	gamePlayers[1] = playerOrb(*gameCamera, *gameMap, *this);
	*gameMap = agarioGameMap(100);
	*gameCamera = gamecamera(*gameCamera->screen, gamePlayers[0], *gameMap);
	gameCamera->updateView();
	mousePos.SetCalcPos(gameCamera->screen->getscaleX(0.5f, 1), gameCamera->screen->getscaleY(0.5f, 1));
	return false;
}

void agarioGame::CollisionDetection() {
	for (int i = 0; i < numberOfPlayers; i++) {
		gamePlayers[i].consumee = false;
		gamePlayers[i].consumer = false;
		gamePlayers[i]._collides = false;
	}
	int count = numberOfPlayers;
	int count0 = count - 1;
	bool res = false;
	playerOrb *aP, *bP;
	for (int i = 0; i < count0; i++) {
		aP = &gamePlayers[i];
		for (int b = i + 1; b < count; b++) {
			bP = &gamePlayers[b];
			if (aP->collides(bP)) {
				aP->_collides = true;
				bP->_collides = true;
				if (res = (aP->getScore() > bP->getScore() || aP->getScore() < bP->getScore())) {
					if (aP->getScore() > bP->getScore()) {
						//Consume
						bP->consumee = !(aP->consumer = false);
					}
					else if (aP->getScore() < bP->getScore()) {
						//Consume
						aP->consumee = !(bP->consumer = false);
					}
				}
			}
		}
	}
}

void agarioGame::PrintStats(gamecamera& cam) {
	box& b = *cam.screen;
	playerOrb& player = *cam.player;

	//Player position	
	//y == 0
	b.set("Position: ", 0, 0);
	b.set(player.getPositionX(), 5, 12, 0);
	b.set(player.getPositionY(), 5, 17, 0);
	
#if !(defined DEBUG && defined _DEBUG)
	b.set("Collides: ", 0, 1);
	if (player._collides)
	{
		b.set("true", 12, 1);
	}
	else {
		b.set("false", 12, 1);
	}
#else

#endif


	//y == 1
#if defined DEBUG || defined _DEBUG
	b.set("Cam Pos:  ", 0, 1);
	b.set(cam.minX(), 5, 12, 1);
	b.set(cam.maxX(), 5, 17, 1);
	//y == 2
	b.set("Cam center: ", 0, 2);
	b.set(cam.centerX(), 5, 12, 2);
	b.set(cam.centerY(), 5, 17, 2);
	//y == 3
	b.set("Cam Max: ", 0, 3);
	b.set(cam.maxX(), 5, 12, 3);
	b.set(cam.maxY(), 5, 17, 3);
	//y == 4
	b.set("Diameter: ", 0, 4);
	b.set(player.getDiameter(), 5, 12, 4);
	//y == 5
	b.set("Radius: ", 0, 5);
	b.set(player.getRadius(), 5, 12, 5);
	//y == 6
	b.set("CamMinAbs:", 0, 6);
	b.set(cam.absMinX(), 5, 12, 6);
	b.set(cam.absMinY(), 5, 17, 6);
	//y == 7
	b.set("CamMaxAbs:", 0, 7);
	b.set(cam.absMaxX(), 5, 12, 7);
	b.set(cam.absMaxY(), 5, 17, 7);
	//y == 8
	b.set("CamDiameter:", 0, 8);
	b.set(cam.diameter(), 5, 12, 8);
	//y==9
	b.set("CamRadius:", 0, 9);
	b.set(cam.radius(), 5, 12, 9);
	//y==10
	b.set("Degrees:", 0, 10);
	b.set(player.getDegAngle(), 5, 12, 10);
	//y==11
	b.set("Collides: ", 0, 11);
	if (player._collides)
	{
		b.set("true", 12, 11);
	}
	else {
		b.set("false", 12, 11);
	}
#endif
}

bool agarioGame::OnFrame()
{
	gameCamera->screen->clear();
	gameCamera->updateView();
	gamePlayers[0].drawToCamera();
	gamePlayers[1].drawToCamera();
	//Position
	//box& b = *gameCamera->screen;
	PrintStats(*gameCamera);

	//gameCamera->screen->set("=", mousePos.getCurrentMouseX(), mousePos.getCurrentMouseY());
	//gameCamera->screen->set("+", mousePos.getCalcX(), mousePos.getCalcY());
	/*
	b.set((char*)&"Position: ", 11, 0, 0);
	b.set(gamePlayers[0].getPositionX(), 6, 0, 1);
	b.set(gamePlayers[0].getPositionY(), 6, 6, 1);
	b.set((char*)&"Camera:   ", 11, 0, 2);
	b.set(gameCamera->minX(), 6, 0, 3);
	b.set(gameCamera->minY(), 6, 6, 3);
	*/


	return true;
}


agarioGame::~agarioGame()
{
}
