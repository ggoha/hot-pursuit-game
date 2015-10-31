#include <string>
#include "CGame.h"

const std::string MAP_PATH = "../map.txt";

int main()
{
	Reader reader;
	try {
		MapFileInput input = reader.readFile( MAP_PATH );
		Map newMap( input.size, input.field );
		Game newGame( newMap, input.finishLine, reader, input.startPositions );
		newGame.start(); /*здесь : Создание окна приложения*/
	} catch( std::exception const &e ) {
		std::cerr << e.what() << std::endl;
	}

	return 0;
}