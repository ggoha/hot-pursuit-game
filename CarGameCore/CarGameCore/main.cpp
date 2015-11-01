#include <string>
#include "CGame.h"
#include "loader.h"

const std::string MAP_PATH = "../map.txt";

int main( int argc, char* argv[] )
{
	Reader reader;
	try {
		MapFileInput input = reader.readFile( MAP_PATH ); // todo: ��������� � loader
		Map newMap( input.size, input.field ); 
		Game newGame( newMap, input.finishLine, reader, input.startPositions );
		Loader loader( MAP_PATH, "cars_file.txt", argc, argv );
		//newGame.start(); /*����� : �������� ���� ����������*/
	} catch( std::exception const &e ) {
		std::cerr << e.what() << std::endl;
	}

	return 0;
}