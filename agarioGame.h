#pragma once
#include "gamecamera.h"
#include "gameEngine.h"
#include "rigidbody.h"
#include "box.h"
#include "agarioGameMap.h"
#include "playerOrb.h"
#include <Windows.h>
class agarioGame :
	public gameEngine
{
public:
	static void Game();
	agarioGame(gamecamera * gamecamera, CHAR_INFO * fb, playerOrb * players, int numberOfPlayers, agarioGameMap* map);
	agarioGame(box * gamecamera, CHAR_INFO * fb);
	bool OnUpdate(float elapsedtime);
	void OnClose();
	bool OnCreate();
	void CollisionDetection();
	void PrintStats(gamecamera & cam);
	bool OnFrame();
	
protected:
	gamecamera* gameCamera;
	agarioGameMap* gameMap;
	playerOrb* gamePlayers;
	int numberOfPlayers;

private:
	~agarioGame();
	struct Position {
	public:
		Position(int mouseCalcCenterX, int mouseCalcCenterY, int currentMouseX, int currentMouseY) {
			//this->MouseCalcCenterX = mouseCalcCenterX;
			//this->MouseCalcCenterY = mouseCalcCenterY;
			SetCalcPos(mouseCalcCenterX, mouseCalcCenterY);
			//this->CurrentMouseX = currentMouseX;
			//this->CurrentMouseY = currentMouseY;
			UpdatePos(currentMouseX, currentMouseY);
		}
		Position() {
			this->MouseCalcCenterX = 1;
			this->CurrentMouseX = 1;
			this->MouseCalcCenterY = 1;
			this->CurrentMouseY = 1;
		}
	private:
		float MouseCalcCenterX;
		float MouseCalcCenterY;
		int CurrentMouseX;
		int CurrentMouseY;
	public:
		void UpdatePos(int x, int y) {
			CurrentMouseX = x;
			CurrentMouseY = y;
		}
		void SetCalcPos(int x, int y) {
			MouseCalcCenterX = x + 0.5f;
			MouseCalcCenterY = y + 0.5f;
		}
		int getCurrentMouseX() { return CurrentMouseX; }
		int getCurrentMouseY() { return CurrentMouseY; }
		int getCalcX() { return MouseCalcCenterX; }
		int getCalcY() { return MouseCalcCenterY; }
		float Distance() {
			return rigidbody::distance(MouseCalcCenterX, CurrentMouseX, MouseCalcCenterY, CurrentMouseY);
		}
		float Degrees() {
			return 180 + rigidbody::radtodeg(atan2(MouseCalcCenterY - CurrentMouseY, MouseCalcCenterX - CurrentMouseX));
		}
		float Radians() {
			return atan2(MouseCalcCenterY - CurrentMouseY, MouseCalcCenterX - CurrentMouseX);
		}
	} mousePos;
};

