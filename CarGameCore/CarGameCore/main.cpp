#include <string>
class Drawing;
#include "CGame.h"
#include "Drawing.h"

const std::string MAP_PATH = "../map.txt";
const std::string CAR_PATH = "cars_file.txt";

int main(int argc, char* argv[])
{
	Reader reader;
	try {
		MapFileInput input = reader.readData(MAP_PATH, CAR_PATH);
		Map newMap(input.size, input.field);
		Game newGame(newMap, input.finishLine, reader, input.startPositions, input.cars);
		newGame.start(argc, argv);
	}
	catch (std::exception const &e) {
		std::cerr << e.what() << std::endl;
	}

	return 0;
}