#pragma once
#ifndef NODE_20100118
#define NODE_20100118
#include "CMap.h"
#include "CPlayer.h"
#include "CReader.h"
#include "Car.h"
#include "Common.h"
#include <vector>
#include <algorithm>
#include <iostream>
#include <memory>
#include <map>
#include <fstream>
class Drawing;

const int DEFAULT_WINDOW_WIDTH = 500;
const int DEFAULT_WINDOW_HEIGHT = 500;
const int DEFAULT_STEPS_PER_CELL = 20;

struct PointsInformation {
	PointsInformation( const bool condition, const Coordinates& oldCoordinates, const Coordinates& newCoordinate ):
		isAlive( condition ), previousCoordinates( oldCoordinates ), currentCoordinates( newCoordinate )
	{
	}

	bool isAlive;
	Coordinates previousCoordinates;
	Coordinates currentCoordinates;
};

enum TType {
	NONE, AI, PLAYER
};

class Game {
public:
	Game();

	Game( const Map& map, const Line& finishLine, const Reader& reader, const std::vector<Coordinates>& coordinates );

	~Game();

	int current_player;
	int numOfDeadPlayers;
	std::vector<TType> menuChoice;
	bool game_ready_to_start;
	int numberOfPlayers;
	void start( int argc, char* argv[] );
	void turnOfPlayer( size_t num, int dir, int& numOfCrushedCar );
	void Game::initPlayers();
	PointsInformation getPlayersBasePoints( size_t num ); // Отдаем Frontend у
	void calculateNumOfPlayers();
	bool playerIsAlive( int player );
	int getPlayerOnFinish();
	void resetSettings();

private:
	Map map;
	Reader reader;
	std::vector<Player> players;
	Line finishLine;
	std::vector<Coordinates> startCoordinates;
	bool finishLineIntersectsWithPlayer( size_t num );
	int playerCrashedIntoCar( size_t num );
	bool playerOutOfTrack( size_t num );
};

#endif /* NODE_20100118 */