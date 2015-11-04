﻿#include "CGame.h"
#include "Drawing.h"

Game::Game()
{
	numOfDeadPlayers = 0;
	menuChoice.push_back(PLAYER);
	menuChoice.push_back(AI);
	menuChoice.push_back(NONE);
	menuChoice.push_back(NONE);
}

Game::Game( const Map& newMap, const Line& newLine, const Reader& newReader, const std::vector<Coordinates>& coordinates,
			const std::vector<Car>& newCars ) :
	map( newMap ), reader( newReader ), finishLine( newLine ), startCoordinates( coordinates ), numOfDeadPlayers( 0 ), cars( newCars )
{
	numOfDeadPlayers = 0;
	menuChoice.push_back(PLAYER);
	menuChoice.push_back(AI);
	menuChoice.push_back(NONE);
	menuChoice.push_back(NONE);
}

Game::~Game()
{
}

bool inBoxOnAxis( int firstPoint, int secondPoint, int thirdPoint, int forthPoint )
{
	if( firstPoint > secondPoint ) {
		std::swap ( firstPoint, secondPoint );
	}
	if( thirdPoint > forthPoint ) {
		std::swap ( thirdPoint, forthPoint );
	}
	return std::max( firstPoint, thirdPoint ) <= std::min( secondPoint, forthPoint );
}

int area( Coordinates firstPoint, Coordinates secondPoint, Coordinates thirdPoint )
{
	return ( secondPoint.x - firstPoint.x ) * ( thirdPoint.y - firstPoint.y )
		- ( secondPoint.y - firstPoint.y ) * ( thirdPoint.x - firstPoint.x );
}

bool isIntersects( Coordinates& firstPoint, Coordinates& secondPoint, Coordinates& thirdPoint, Coordinates& fourthPoint )
{
	return inBoxOnAxis( firstPoint.x, secondPoint.x, thirdPoint.x, fourthPoint.x )
		&& inBoxOnAxis( firstPoint.y, secondPoint.y, thirdPoint.y, fourthPoint.y )
		&& ( area( firstPoint, secondPoint, thirdPoint )
			 * area( firstPoint, secondPoint, fourthPoint ) ) <= 0
		&& ( area( thirdPoint, fourthPoint, firstPoint )
			 * area( thirdPoint, fourthPoint, secondPoint ) ) <= 0;
}

bool Game::finishLineIntersectsWithPlayer( size_t num )
{
	// Происходит проверка:
	// 1. Проекции отрезков на оси пересекаются
	// 2. Считается ориентированная площадь треугольников. Нужно, чтобы эти площади были разных знаков.
	Coordinates playersPreviousCoordinates = players[num].getPreviousPosition();
	Coordinates playersCoordinates = players[num].getPosition();
	return isIntersects( playersPreviousCoordinates, playersCoordinates, finishLine.firstPoint, finishLine.secondPoint );
}

int vectorMul( const Line& firstVec, const Line& secondVec )
{
	return ( firstVec.secondPoint - firstVec.firstPoint ).x * ( secondVec.secondPoint - secondVec.firstPoint ).y
		- ( firstVec.secondPoint - firstVec.firstPoint ).y * ( secondVec.secondPoint - secondVec.firstPoint ).x;
}


double distance( const Coordinates& firstPoint, const Coordinates& secondPoint )
{
	return sqrt( ( firstPoint.x - secondPoint.x ) * ( firstPoint.x - secondPoint.x ) + ( firstPoint.y - secondPoint.y ) * ( firstPoint.y - secondPoint.y ) );
}

bool pointOnFinish( const Coordinates& point, const Line& line )
{
	return distance( point, line.firstPoint ) + distance( point, line.secondPoint ) - distance( line.firstPoint, line.secondPoint ) < 0.000001;
}

int Game::getPlayerOnFinish()
{
	for( size_t i = 0; i < players.size(); ++i ) {
		if( pointOnFinish( players[i].getPreviousPosition(), finishLine ) ) {
			continue;
		}
		if( finishLineIntersectsWithPlayer( i )
			&& vectorMul( finishLine, Line( players[i].getPreviousPosition(), players[i].getPosition() ) ) > 0 ) {
			players[i].cheat();
			continue;
		}
		if( finishLineIntersectsWithPlayer( i )
			&& vectorMul( finishLine, Line( players[i].getPreviousPosition(), players[i].getPosition() ) ) < 0 ) {
			if( !players[i].isCheater() ) {
				return ( int ) i;
			} else {
				players[i].reduceCheat();
				continue;
			}
		}
	}
	return -1;
}

int Game::playerCrashedIntoCar( size_t num )
{
	for( size_t i = 0; i < players.size(); ++i ) {
		if( i != num && players[i].getPosition() == players[num].getPosition() ) {
			return ( int ) i;
		}
	}
	return -1;
}

bool Game::playerOutOfTrack( size_t num )
{
	Coordinates playersPreviousCoordinates = players[num].getPreviousPosition();
	Coordinates playersCoordinates = players[num].getPosition();

	int minX = std::min( playersPreviousCoordinates.x, playersCoordinates.x ),
		maxX = std::max( playersPreviousCoordinates.x, playersCoordinates.x ),
		minY = std::min( playersPreviousCoordinates.y, playersCoordinates.y ),
		maxY = std::max( playersPreviousCoordinates.y, playersCoordinates.y );

	Coordinates realCoordinates( playersCoordinates.x * 10 + 5, playersCoordinates.y * 10 + 5 ),
		realPreviousCoordinates( playersPreviousCoordinates.x * 10 + 5, playersPreviousCoordinates.y * 10 + 5 );

	for( int i = minX; i <= maxX; ++i ) {
		for( int j = minY; j <= maxY; ++j ) {
			if( map.isEmpty( i, j ) ) {
				continue;
			}
			Coordinates firstPoint( i * 10, j * 10 ),
				secondPoint( ( i + 1 ) * 10, j * 10 ),
				thirdPoint( ( i + 1 ) * 10, ( j + 1 ) * 10 ),
				fourthPoint( i * 10, ( j + 1 ) * 10 );

			if( isIntersects( realPreviousCoordinates, realCoordinates, firstPoint, secondPoint ) ||
				isIntersects( realPreviousCoordinates, realCoordinates, secondPoint, thirdPoint ) ||
				isIntersects( realPreviousCoordinates, realCoordinates, thirdPoint, fourthPoint ) ||
				isIntersects( realPreviousCoordinates, realCoordinates, fourthPoint, firstPoint ) ) {
				return true;
			}
		}
	}
	return false;
}

void Game::turnOfPlayer( size_t num )
{
	int direction = reader.readPlayersChoice( num );

	players[num].move( direction, map.getSize() );

	if( !players[num].directionIsValid( map.getSize() ) && !finishLineIntersectsWithPlayer( num ) ) {
		// Смысл: если на скорости пересек финиш и выехал за пределы поля ЗА финишом - считается, что победил
		players[num].die();
		++numOfDeadPlayers;
		std::cout << "Player " << num + 1 << " is dead" << std::endl;
		return;
	}

	int crashedPlayer = playerCrashedIntoCar( num );
	if( crashedPlayer != -1 ) {
		players[num].goToStart();
		clearPlayersState( crashedPlayer );
		players[crashedPlayer].goToStart();
		showPlayersState( crashedPlayer );
		return;
	}
	if( playerOutOfTrack( num ) ) {
		players[num].die();
		++numOfDeadPlayers;
		std::cout << "Player " << num + 1 << " is dead" << std::endl;
		return;
	}
}

void Game::initPlayersPositionsInMap()
{
	for( size_t i = 0; i < players.size(); ++i ) {
		Coordinates currentCoordinates = players[i].getPosition();
		map.setPosition( currentCoordinates.x, currentCoordinates.y );
	}
}

void Game::clearPlayersState( size_t num ) // Стирает изображение игрока с поля
{
	Coordinates old = players[num].getPreviousPosition();
	Coordinates now = players[num].getPosition();
	map.clearPosition( old.x, old.y );
	map.clearPosition( now.x, now.y );
}

void Game::showPlayersState( size_t num ) // Рисует изображение игрока на поле
{
	Coordinates previousCoordinates = players[num].getPreviousPosition();
	Coordinates currentCoordinates = players[num].getPosition();
	map.setPosition( previousCoordinates.x, previousCoordinates.y );
	map.setPosition( currentCoordinates.x, currentCoordinates.y );
}


void Game::initPlayers( int numberOfPlayers )
{
	if( numberOfPlayers > startCoordinates.size() ) {
		throw std::runtime_error( "Too many players on field" );
	}
	for( size_t i = 0; i < numberOfPlayers; ++i ) {	// Все игроки на стартовых позициях, которые были нанесены на карту
		players.push_back( Player( startCoordinates[i], true ) );
	}
}

void Game::start( int argc, char* argv[] )
{
	/*visual start*/
	Drawing drawing_module(this, map, cars );
	drawing_module.draw( argc, argv ); // Main визуализатора
	/*visual end*/

	/*int n = 2; // Debug option
	initPlayers( n ); 
	initPlayersPositionsInMap(); // На карте проставляются координаты машинок
	std::cout << "Game has been started. Gl hf!" << std::endl;
	int player;
	while( ( player = getPlayerOnFinish() ) == -1 ) { // -1 - никто пока к финишу не пришел
		for( size_t i = 0; i < players.size(); ++i ) {
			if( players[i].playerIsAlive() ) {
				clearPlayersState( i );
				turnOfPlayer( i ); // todo: AI: занести его в players[] и с помощью API получать следующих ход(см документацию) 
				if( players[i].playerIsAlive() ) { // Если не вылетел за границы
					showPlayersState( i );
				}
				map.print(); // Вывод поля на консоль
			}
		}
		if( numOfDeadPlayers == players.size() ) {
			break;
		}
	}
	if( numOfDeadPlayers == players.size() ) {
		fatalFinish();
		return;
	}
	finish( player );*/
}

void Game::finish( size_t winner )
{
	std::cout << "Player number " << winner + 1 << " is winner! Congratulations!!!" << std::endl;
}

void Game::fatalFinish()
{
	std::cout << "All players are dead! Congratulations ^_^" << std::endl;
}

PointsInformation Game::getPlayersBasePoints( size_t num )  // Frontend: Для Frontend'a - получение точек для отрисовки
{
	if( num < 0 || num >= players.size() ) {
		std::string error = "Game::getPlayersBasePoints: ";
		if( num < 0 ) {
			error += "num < 0";
		} else {
			error += "num > number of players";
		}
		throw std::invalid_argument( error );
	}
	return PointsInformation( players[num].playerIsAlive(), players[num].getPreviousPosition(), players[num].getPosition() );
}