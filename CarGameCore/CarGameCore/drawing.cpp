#include "Drawing.h"
#include "CGame.h"

Map Drawing::map; // static data members must be explicitly defined in exactly one compilation unit
std::vector<Car> Drawing::cars;
Game* Drawing::game;
bool Drawing::menu;


Drawing::Drawing(Game* _game, Map &map_data, std::vector<Car> &_cars)
{
	map = map_data;
	cars = _cars;
	game = _game;
	menu = true;
}

void Drawing::reshape(int width, int height)
{
	glViewport(0, 0, width, height); // set view block

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, width, 0, height); // set coordinates 

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	map.Calculate(); // recalculate map
}

void Drawing::timer(int value)
{
	glutPostRedisplay(); // marks the current window as needing to be redisplayed
	glutTimerFunc(1, timer, 0); // registers a timer callback to be triggered in a specified number of milliseconds.
}


void Drawing::display()
{
//	glClearColor(1.0, 1.0, 1.0, 0.0); // clear background to white
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear buffers

	if (!menu)
	{
		glClearColor( 1.0, 1.0, 1.0, 0.0 ); // clear background to white
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT ); // clear buffers

		bool map_reloaded = !map.Need_to_reload();
		map.Draw(); // draw the map
		float cell_size = map.Get_cell_size(); // calculate the size of cell
		for( int j = 0; j < cars[0].frames_per_step; j++ ) {
			map.Draw();
			for( size_t i = 0; i < cars.size(); i++ ) {
				cars[i].Draw( map.Get_cell_size(), map.Get_indent() ); // draw car
			}
			glFlush();
			glutSwapBuffers();
		}
		glFlush(); // flush changes
		if( map_reloaded ) {
			glutSwapBuffers(); // if map wasn't reloaded (and buffers weren't swapped), swap buffers
		}
	}
	else
	{
		drawMenu();
		glFlush(); // flush changes
		glutSwapBuffers();
	}

}

void Drawing::renderBitmapString(float x, float y, float z, void *font, char *string)
{
	char *c;
	glRasterPos3f(x, y, z);
	for (c = string; *c != '\0'; c++) {
		glutBitmapCharacter(font, *c);
	}
}

void Drawing::drawMenu()
{

	float height = glutGet(GLUT_WINDOW_HEIGHT),
		width = glutGet(GLUT_WINDOW_WIDTH);
	int size = game->menuChoice.size();
	void* font = GLUT_BITMAP_TIMES_ROMAN_24;
	glColor3f(255, 255, 255);
	renderBitmapString(width*0.33, height*0.9, 0, font, "Please, choose players");
	//рисуем подсвеку выделеного пункта меню
	for (int i = 0; i < size; ++i)
	{
		switch (game->menuChoice[i])
		{
		case PLAYER:
			glBegin(GL_QUADS); //Begin quadrilateral coordinates
			glVertex3f(i*width / size, height*0.55, 0);
			glVertex3f((i + 1)*width / size, height*0.55, 0);
			glVertex3f((i + 1)*width / size, height*0.65, 0);
			glVertex3f(i*width / size, height*0.65, 0);
			glEnd();
			break;
		case AI:
			glBegin(GL_QUADS); //Begin quadrilateral coordinates
			glVertex3f(i*width / size, height*0.45, 0);
			glVertex3f((i + 1)*width / size, height*0.45, 0);
			glVertex3f((i + 1)*width / size, height*0.55, 0);
			glVertex3f(i*width / size, height*0.55, 0);
			glEnd();
			break;
		case NONE:
			glBegin(GL_QUADS); //Begin quadrilateral coordinates
			glVertex3f(i*width / size, height*0.35, 0);
			glVertex3f((i + 1)*width / size, height*0.35, 0);
			glVertex3f((i + 1)*width / size, height*0.45, 0);
			glVertex3f(i*width / size, height*0.45, 0);
			glEnd();
			break;
		}
	}
	//рисуем кнопку далее
	glBegin(GL_QUADS); //Begin quadrilateral coordinates
	glVertex3f(0, height*0, 0);
	glVertex3f(width, height*0, 0);
	glVertex3f(width, height*0.25, 0);
	glVertex3f(0, height*0.25, 0);
	glEnd();
	glColor3f(0, 0, 0);
	glBegin(GL_TRIANGLES); //Begin triangle coordinates
	glVertex3f(width*0.33, height*0.08, 0);
	glVertex3f(width*0.33, height*0.16, 0);
	glVertex3f(width*0.66, height*0.12, 0);
	glEnd(); //End triangle coordinates
	//рисуем выбор типа
	for (int i = 0; i < size; ++i)
	{
		switch (i)
		{
			case(0) : glColor3f(255, 0, 0); break;
			case(1) : glColor3f(0, 255, 0); break;
			case(2) : glColor3f(0, 0, 255); break;
			case(3) : glColor3f(245, 255, 0); break;
		};
		char str[2];
		itoa(i+1, str, 10);
		renderBitmapString(i*width / size, height*0.7, 0, font, str);
		renderBitmapString(i*width / size, height*0.6, 0, font, "PLAYER");
		renderBitmapString(i*width / size, height*0.5, 0, font, "AI");
		renderBitmapString(i*width / size, height*0.4, 0, font, "NONE");
	}
}

// load image from file to texture
void Drawing::Load_texture(const char* filename, GLuint& texture)
{
	// create and choose texture
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	// read from file
	int width, height;
	int channels;
	unsigned char* image = SOIL_load_image(filename, &width, &height, &channels, SOIL_LOAD_RGBA);
	// save data to texture
	gluBuild2DMipmaps(GL_TEXTURE_2D, 4, width, height, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE,
		(float)GL_MODULATE);
	SOIL_free_image_data(image);
	// unchoose texture
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Drawing::load()
{
	//load textures for map
	Load_texture("..\\images\\road.png", map.texture_road); // road
	Load_texture("..\\images\\forest.png", map.texture_board); // board
	//load textures for cars (depends on color)
	std::string car_filename;
	for (size_t i = 0; i < cars.size(); i++) {
		switch (cars[i].Get_color()) {
		case Red:
			car_filename = "..\\images\\car_red.png";
			break;
		case Blue:
			car_filename = "..\\images\\car_blue.png";
			break;
		case Green:
			car_filename = "..\\images\\car_green.png";
			break;
		default:
			car_filename = "..\\images\\car_red.png";
		}
		Load_texture(car_filename.c_str(), cars[i].texture);
	}
}

void Drawing::OnMove(int direction, Size size) //  size - размер карты - нужен для обработки хода
{
	if( game->game_ready_to_start ) {  
		game->current_player++;
		game->current_player %= game->numberOfPlayers;
		game->turnOfPlayer( game->current_player, direction );
		PointsInformation pi = game->getPlayersBasePoints( game->current_player );
		Coord c;
		c.x = pi.currentCoordinates.x;
		c.y = pi.currentCoordinates.y;
		cars[game->current_player].MoveTo( c, !pi.isAlive );
	}
}

void Drawing::normalKeyHandler(unsigned char key, int x, int y)
{
	switch (key) {
	case 55:
		OnMove(7, map.getSize());
		break;
	case 56:
		OnMove(8, map.getSize());
		break;
	case 57:
		OnMove(9, map.getSize());
		break;
	case 52:
		OnMove(4, map.getSize());
		break;
	case 53:
		OnMove(5, map.getSize());
		break;
	case 54:
		OnMove(6, map.getSize());
		break;
	case 49:
		OnMove(1, map.getSize());
		break;
	case 50:
		OnMove(2, map.getSize());
		break;
	case 51:
		OnMove(3, map.getSize());
		break;
	case 27:
		menu = false;
		break;
	}

}

int Drawing::clickButton(int x, int y)
{
	float height = glutGet(GLUT_WINDOW_HEIGHT),
		width = glutGet(GLUT_WINDOW_WIDTH);
	y = height - y;
	int size = game->menuChoice.size();
	//обработка меню
	for (int i = 0; i < size; i++)
	{
		if (x > i*width / size && x < (i + 1)*width / size && y > height*0.55 && y < height*0.65)
			game->menuChoice[i] = PLAYER;
		if (x > i*width / size && x < (i + 1)*width / size && y > height*0.45 && y < height*0.55)
			game->menuChoice[i] = AI;
		if (x > i*width / size && x < (i + 1)*width / size && y > height*0.35 && y < height*0.45)
			game->menuChoice[i] = NONE;
	}
	game->initPlayers();
	for( int i = 0; i < game->numberOfPlayers; i++ ) {
		PointsInformation pi = game->getPlayersBasePoints(i);
		Coord c;
		c.x = pi.currentCoordinates.x;
		c.y = pi.currentCoordinates.y;
		cars[i].current_coords = c;
		cars[i].next_coords = c;
		cars[i].current_angle = 0;
	}
	game->game_ready_to_start = true;
	//обработка далее
	if (y < height*0.25)
		return 1;
	else
		return -1;
}

void Drawing::mouseButton(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON)
		// when the button is released
		if (state == GLUT_UP) {
			if (clickButton(x, y)>0)
				menu = false;
		}
}

void Drawing::draw(int argc, char * argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH |GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(800, 600);
	glutCreateWindow("Rock'n'Roll Race");

	glutKeyboardFunc(normalKeyHandler); // Обработка нажатий клавиш от игроков
	glutMouseFunc(mouseButton);

	load(); // Загрузка текстур + машинок

	glutTimerFunc(1, timer, 0); // каждую милисекунду говорит что окно нуждается в перерисовке
	glutReshapeFunc(reshape); // invoke reshape при изменении окна
	glutDisplayFunc(display); // если нужно сделать redisplay (callback на redisplay присылается из timer)

	glutMainLoop();
}