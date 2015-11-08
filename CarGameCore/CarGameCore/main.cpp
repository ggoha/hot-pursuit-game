#include <string>
#include "CGame.h"

const std::string MAP_PATH = "../map.txt";

int main( int argc, char* argv[] )
{
	Reader reader;
	try {
		MapFileInput input = reader.readData( MAP_PATH );
		Map newMap( input.size, input.field );
		Game newGame( newMap, input.finishLine, reader, input.startPositions );
		newGame.start( argc, argv );
	}
	catch( std::exception const &e ) {
		std::cerr << e.what() << std::endl;
	}

	return 0;
}