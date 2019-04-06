// console_agario_cpp.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "agarioGame.h"
#include "png.h"
#include <Windows.h>
#include "rigidbody.h"
#include "rigidbox.h"
#include "box.h"
#define GETCHAR(xsssss) ((	 xsssss > 0.9f ? '@' :\
							 xsssss > 0.8f ? '%' :\
							 xsssss > 0.7f ? '#' :\
							 xsssss > 0.6f ? '*' :\
							 xsssss > 0.5f ? '+' :\
							 xsssss > 0.4f ? '=' :\
							 xsssss > 0.3f ? '-' :\
							 xsssss > 0.2f ? ':' :\
							 xsssss > 0.1f ? '.' :\
							' '))
#define GETZ(xeey, yeey) (((yeey * CONSOLEWIDTH) + CONSOLEHEIGHT))

#define CONSOLEWIDTH 150
#define CONSOLEHEIGHT 90
#define CONSOLELENGTH (CONSOLEWIDTH * CONSOLEHEIGHT)

void print(png& png);
void write(wchar_t* fb, size_t count);

class imageRender : public gameEngine {
public:
	imageRender(png* image, box* gamecamera, HANDLE Og, CHAR_INFO* fb, char* samplerBuffer)
		:gameEngine(gamecamera, Og, fb, false), dvdpos(0, 0, 0, 0), body()
	{
		velocity = 4.0f;
		size = 0.4f;
		SetTargetTicksPerSecond(900);
		this->image = image;
		this->samplerBuffer = samplerBuffer;
		consoleOut = GetStdHandle(STD_OUTPUT_HANDLE);
		consoleIn = GetStdHandle(STD_INPUT_HANDLE);
		ConstructConsole(gamecamera->sizeX, gamecamera->sizeY, 8, 8, (wchar_t*)L"Raster");
		//SetConsoleActiveScreenBuffer(consoleOut = CreateBuffer());
		doClear = true;
		
	}

	png* image;
	boxsize dvdpos;
	rigidbox body;
	char* samplerBuffer;
	float velocity;
	float size;

	bool OnFrame() {
		/*
		//int offsetx = getscaleXint(0.5f, image->getSizeX());
		bool xIsWider;

		int scaleX = (int)(float(sizeX) / float(image->getSizeX()));
		int scaleY = (int)(float(sizeY) / float(image->getSizeY()));

		for (int y = 0, c = 0; y < sizeY; y++) {
			for (int x = 0; x < sizeX; x++, c++) {
				float value = image->getSample(x, y, this)->value();
				char charz = GETCHAR(value);
				_framebuffer[get(x, y)] = charz;
			}
		}
		*/
		//DrawAt(image, &body);
		
		Draw(&body);
		/*boxsize sizes(10,10,20,20);
		char* fb = (char*)alloca(sizes.characters());
		box b(sizes, fb, sizes.characters(), true);
		b.drawCircle();
		Draw(&b);*/
		return true;
	}

	bool OnCreate() {
		int dvdLenX = this->getscaleXint(size), dvdLenY = this->getscaleYint(size);
		dvdpos = boxsize(this->getscaleXint(0.5f, dvdLenX), this->getscaleYint(0.5f, dvdLenY), dvdLenX, dvdLenY);
		body = rigidbox(velocity, 190.0f, samplerBuffer, &dvdpos, this, true);
		body.sampleImage(*image);
		return true;
	}

	bool OnUpdate(float elapsedTime) {
		if (getKey(VK_ADD)->bReleased) {
			velocity *= 2.0f;
			body.setvelocity(velocity);
		}
		if (getKey(VK_SUBTRACT)->bReleased) {
			velocity /= 2.0f;
			body.setvelocity(velocity);
		}
		if (getKey(VK_ESCAPE)->bReleased) {
			Close();
		}
		//if (++dvdpos.offsetX + dvdpos.sizeX > this->sizeX) {
		//	dvdpos.offsetX = 0;
		//}

		//if (++dvdpos.offsetY + dvdpos.sizeY > this->sizeY) {
		//	dvdpos.offsetY = 0;
		//}


#ifdef _DEBUG
body.update(0.020f);
#else
body.update(elapsedTime);
#endif // DEBUG

		//body.update(elapsedTime);
/*
if (body.lastUpdateHitWall())
		switch (body.getLastWall())
		{
		case 0:
			body.flipV();
			break;
		case 1:
			body.flipH();
			break;
		case 2:
			body.flipV();
			break;
		case 3:
			body.flipH();
			break;
		}
		*/
		return true;
	}

	void DrawAt(png* image, rigidbody* body) {
		boxsize size(0,0,0,0);
		body->getBody(size);
		DrawAt(image, size);
	}

	void Draw(rigidbox * rigidb) {
		boxsize b;
		char* fb = rigidb->getSampledImage();
		char cur;
		rigidb->getBody(b);
		setAlpha(false);
		clear();

		for (int y = b.offsetY; y < b.sizeY + b.offsetY && y < this->sizeY; y++) {
			for (int x = b.offsetX; x < b.sizeX + b.offsetX && x < this->sizeX; x++) {		
				if ((cur = fb[b.get(x - b.offsetX, y - b.offsetY)]) && cur != '\0') {
					_framebuffer[get(x, y)] = cur;
				}
			}
		}
	}

	void Draw(box* boxToDraw) {
		char cur;
		char* fb = boxToDraw->getFramebuffer();
		for (int y = boxToDraw->offsetY; y < boxToDraw->sizeY + boxToDraw->offsetY && y < this->sizeY; y++) {
			for (int x = boxToDraw->offsetX; x < boxToDraw->sizeX + boxToDraw->offsetX && x < this->sizeX; x++) {
				if ((cur = fb[boxToDraw->get(x - boxToDraw->offsetX, y - boxToDraw->offsetY)]) != '\0') {
					_framebuffer[get(x, y)] = cur;
				}
			}
		}
	}

	void DrawAt(png* image, boxsize& pos) {
		for (int y = pos.offsetY; y < pos.sizeY + pos.offsetY && y < CONSOLEHEIGHT; y++) {
			for (int x = pos.offsetX; x < pos.sizeX + pos.offsetX && x < CONSOLEWIDTH; x++) {
				_framebuffer[get(x, y)] = GETCHAR(image->getSample(x - pos.offsetX, y - pos.offsetY, &pos)->negativeValue());
			}
		}
	}
};

int main(int argc, char* argv[])
{
	/*
	const char* dvd = "C:\\Users\\Ian\\source\\repos\\console_agario_cpp\\Debug\\dvd.png";
	char* path = (char*)dvd;
	if (argc > 1) {
		path = argv[1];
	}
	static const char* lp = "dvd.png";
	static const char* fp = "C:\\Users\\Ian\\source\\repos\\console_agario_cpp\\Debug\\dvd.png";
	static const char* en = "C:\\Users\\Ian\\source\\repos\\console_agario_cpp\\Debug\\en.png";
	//png pPng(path);
	png pPng(path);
	std::cout << pPng.getSizeX() << "x" << pPng.getSizeY() << std::endl;
	//print(pPng);
	CHAR_INFO* charInfos = (CHAR_INFO*)alloca(sizeof(CHAR_INFO) * CONSOLELENGTH);
	char* framebuffer = (char*)alloca(sizeof(char) * CONSOLELENGTH);
	char* samplerBuffer = (char*)alloca(sizeof(char) * CONSOLELENGTH);
	boxsize bSize = boxsize(0, 0, CONSOLEWIDTH, CONSOLEHEIGHT);
	box bs = box(bSize, framebuffer, CONSOLELENGTH, false);	
	HANDLE og = GetStdHandle(STD_OUTPUT_HANDLE);
	imageRender iRender(&pPng, &bs, og, charInfos, samplerBuffer);
	iRender.Start();
	*/
	agarioGame::Game();
	return 0;
}

void print(png& png) {
	size_t length = png.getPixelCount();
	length = length > CONSOLELENGTH ? CONSOLELENGTH : length;
	wchar_t* characters = (wchar_t*)alloca(length * sizeof(wchar_t));
	pixel pixel();
	wchar_t hm;
	int maxX = png.getSizeX(), maxY = png.getSizeY();
	int c = 0;
	for (size_t y = 0; y < maxY && y < CONSOLEHEIGHT; y++) {
	for (size_t x = 0; x < maxX && x < CONSOLEWIDTH && c < length; x++, c++) {
			characters[c++] = GETCHAR(png.getPixel(x, y)->value());
		}
	}
	write(characters, length);
}

void write(wchar_t* fb, size_t count) {
	LPDWORD* d = new LPDWORD();
	HANDLE old = GetStdHandle(STD_OUTPUT_HANDLE);
	HANDLE h = gameEngine::CreateBuffer();
	SetConsoleActiveScreenBuffer(h);
	WriteConsoleOutputCharacterW(h, fb, count, { 0,0 }, (LPDWORD)&d);
	Sleep(5000);
	SetConsoleActiveScreenBuffer(old);
	//fb[count - 1] = '\0';
	//std::cout << fb;
}


// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
