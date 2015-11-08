#include "CGame.h"
#include "Drawing.h"

Game::Game()
{
	numOfDeadPlayers = 0;
	numberOfPlayers = 0;
	current_player = 0;
	game_ready_to_start = false;
	// default settings:
	menuChoice.push_back( PLAYER );
	menuChoice.push_back( AI );
	menuChoice.push_back( NONE );
	menuChoice.push_back( NONE );
}

Game::Game( const Map& newMap, const Line& newLine, const Reader& newReader, const std::vector<Coordinates>& coordinates ):
	map( newMap ), reader( newReader ), finishLine( newLine ), startCoordinates( coordinates ), numOfDeadPlayers( 0 )
{
	numOfDeadPlayers = 0;
	numberOfPlayers = 0;
	current_player = 0;
	game_ready_to_start = false;
	// default settings:
	menuChoice.push_back( PLAYER );
	menuChoice.push_back( AI );
	menuChoice.push_back( NONE );
	menuChoice.push_back( NONE );
}

Game::~Game()
{
}

bool inBoxOnAxis( int firstPoint, int secondPoint, int thirdPoint, int forthPoint )
{
	if( firstPoint > secondPoint ) {
		std::swap( firstPoint, secondPoint );
	}
	if( thirdPoint > forthPoint ) {
		std::swap( thirdPoint, forthPoint );
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

void Game::turnOfPlayer( size_t num, int direction, int& numOfCrushedCar )
{
	players[num].move( direction, map.getSize() );
	if( !players[num].directionIsValid( map.getSize() ) && !finishLineIntersectsWithPlayer( num ) ) {
		// Смысл: если на скорости пересек финиш и выехал за пределы поля ЗА финишом - считается, что победил
		players[num].die();
		++numOfDeadPlayers;
		return;
	}
	numOfCrushedCar = playerCrashedIntoCar( num );
	if( numOfCrushedCar != -1 ) {
		players[num].goToStart();
		players[numOfCrushedCar].goToStart();
		return;
	}
	if( playerOutOfTrack( num ) ) {
		players[num].die();
		++numOfDeadPlayers;
		return;
	}
	return;
}

void Game::initPlayers()
{
	if( numberOfPlayers > startCoordinates.size() ) {
		throw std::runtime_error( "Too many players on field" );
	}
	for( size_t i = 0; i < numberOfPlayers; ++i ) {	// Все игроки на стартовых позициях, которые были нанесены на карту
		players.push_back( Player( startCoordinates[i], true ) );
	}
}

void Game::calculateNumOfPlayers() 
{
	for( size_t i = 0; i < 4; ++i ) {
		if( menuChoice[i] != NONE ) {
			++numberOfPlayers;
		}
	}
}

void Game::start( int argc, char* argv[] )
{
	Drawing drawing_module( this, map.getMapInOpenGLView() );
	drawing_module.startDrawing( argc, argv ); // Визуализатор
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

bool Game::playerIsAlive( int player )
{
	return players[player].playerIsAlive();
}

void Game::resetSettings()
{
	numOfDeadPlayers = 0;
	numberOfPlayers = 0;
	current_player = 0;
	game_ready_to_start = false;
	players.clear();
}

void Game::toNextPlayer()
{
	++current_player; // Поиск живых игроков
	current_player %= numberOfPlayers;
	while( !players[current_player].playerIsAlive() ) {
		++current_player;
		current_player %= numberOfPlayers;
	}
}