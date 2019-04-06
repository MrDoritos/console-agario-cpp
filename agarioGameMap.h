#pragma once
class agarioGameMap
{
public:
	agarioGameMap(int mapSize);
	~agarioGameMap();
	int getMapSize() { return mapSize; }
private:
	int mapSize;
};

