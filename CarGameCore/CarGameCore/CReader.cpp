#include "CReader.h"

Reader::Reader()
{
}

Reader::~Reader()
{
}

bool isNumber( const std::string& number )
{
	for( int i = 0; i < number.length(); ++i ) {
		if( number[i] > '9' || number[i] < '0' ) {
			return false;
		}
	}
	return true;
}

bool isValidCell( int cell )
{
	return cell == 0 || cell == 1 || cell == 2;
}

MapFileInput Reader::readFile( const std::string& path )
{
	std::ifstream fin( path );

	if( !fin ) {
		std::string error = "Can't open file ";
		error += path;
		throw std::runtime_error( error );
	}
	
	int n, m;
	fin >> n >> m;
	if( n <= 0 || m <= 0 ) {
		std::string error = "Bad file with map: size of field should be > 0";
		throw std::runtime_error( error );
	}
	Size size( n, m );
	Field gameField;
	std::vector<Coordinates> startPositions;
	gameField.resize( n );
	for( size_t i = 0; i < n; ++i ) {
		for( size_t j = 0; j < m; ++j ) {
			int position;
			fin >> position;
			if( !isValidCell( position ) ) {
				std::string error = "Bad file with map: invalid symbol at ";
				error += std::to_string( j ) + " " + std::to_string( i );
				throw std::runtime_error( error );
			}
			if( position == 2 ) {
				startPositions.push_back( Coordinates( j - 1, i - 1 ) );
			}
			gameField[i].push_back( position );
		}
	}

	int firstPointX, firstPointY, secondPointX, secondPointY;
	fin >> firstPointX >> firstPointY >> secondPointX >> secondPointY;
	if( firstPointX < 0 || firstPointY < 0 || secondPointX < 0 || secondPointY < 0 ) {
		std::string error = "Bad file with map: invalid coordinates of finish line";
		throw std::runtime_error( error );
	}
	Line finishLine( Coordinates( firstPointX, firstPointY ), Coordinates( secondPointX, secondPointY ) );
	return MapFileInput( gameField, size, startPositions, finishLine );
}

Coordinates readCoordinates()
{
	int x, y;
	std::cin >> x >> y;
	if( x <= 0 || y <= 0 ) {
		std::string error = "Bad input: invalid coordinates";
		throw std::runtime_error( error );
	}
	return Coordinates( x - 1, y - 1 );
}

PlayersInfo Reader::readPlayers()
{
	PlayersInfo info;
	std::cout << "Enter number of players:" << std::endl;
	int numberOfPlayers;
	std::cin >> numberOfPlayers;
	if( numberOfPlayers <= 0 ) {
		std::string error = "Bad input: invalid number of players";
		throw std::runtime_error( error );
	}
	info.numberOfPlayers = numberOfPlayers;
	for( int i = 0; i < numberOfPlayers; ++i )
		info.positions.push_back( readCoordinates() );
	return info;
}


int Reader::readPlayersChoice( size_t num )
{
	std::cout << "Now " << num + 1 << " player:" << std::endl;
	int move;
	std::cin >> move;
	if( move < 1 || move > 9 ) {
		std::string error = "Bad input: invalid move";
		throw std::runtime_error( error );
	}
	return move;
}