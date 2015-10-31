#pragma once

#include "CMap.h"
#include "CPlayer.h"
#include "CReader.h"
#include <vector>
#include <algorithm>
#include <iostream>
#include <memory>

struct PointsInformation {
	PointsInformation( const bool condition, const Coordinates& oldCoordinates, const Coordinates& newCoordinate ) :
		isAlive( condition ), previousCoordinates( oldCoordinates ), currentCoordinates( newCoordinate )
	{
	}

	bool isAlive;
	Coordinates previousCoordinates;
	Coordinates currentCoordinates;
};


class Game {
public:
	Game();

	Game( const Map& map, const Line& finishLine, const Reader& reader, const std::vector<Coordinates>& coordinates );

	~Game();

	void start();
	void finish( size_t winner );
	void fatalFinish();
	PointsInformation getPlayersBasePoints( size_t num ); // Отдаем Frontend у

private:
	Map map;
	Reader reader;
	size_t numOfDeadPlayers;
	std::vector<Player> players;
	Line finishLine;
	std::vector<Coordinates> startCoordinates;

	void initPlayers( const PlayersInfo& playersInfo );
	int getPlayerOnFinish();
	bool finishLineIntersectsWithPlayer( size_t num );
	void turnOfPlayer( size_t num );
	int playerCrashedIntoCar( size_t num );
	bool playerOutOfTrack( size_t num );
	void initPlayersPositionsInMap();
	void clearPlayersState( size_t num );
	void showPlayersState( size_t num );
};