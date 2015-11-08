#include "Drawing.h"

Map Drawing::map; // static data members must be explicitly defined in exactly one compilation unit
std::vector<Car> Drawing::cars;
Game* Drawing::game;
bool Drawing::menu;


Drawing::Drawing( Game* _game, Map &map_data )
{
	map = map_data;
	game = _game;
	menu = true;
}

void Drawing::reshape( int width, int height )
{
	glViewport( 0, 0, width, height ); // set view block

	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	gluOrtho2D( 0, width, 0, height ); // set coordinates 

	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();
	map.Calculate(); // recalculate map
}

void Drawing::timer( int value )
{
	glutPostRedisplay(); // marks the current window as needing to be redisplayed
	glutTimerFunc( 1, timer, 0 ); // registers a timer callback to be triggered in a specified number of milliseconds.
}


void Drawing::display()
{
	//	glClearColor(1.0, 1.0, 1.0, 0.0); // clear background to white
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT ); // clear buffers

	if( !menu ) {
		glClearColor( 1.0, 1.0, 1.0, 0.0 ); // clear background to white
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT ); // clear buffers

		bool map_reloaded = !map.Need_to_reload();
		map.Draw(); // draw the map
		float cell_size = map.Get_cell_size(); // calculate the size of cell
		for( int j = 0; j < cars[0].frames_per_step; j++ ) {
			map.Draw();
			for( size_t i = 0; i < cars.size(); i++ ) {
				if( !game->playerIsAlive( i ) ) { // Машина умерла - не рисуем
					continue;
				}
				cars[i].Draw( map.Get_cell_size(), map.Get_indent() ); // draw car
			}
			glFlush();
			glutSwapBuffers();
		}
		glFlush(); // flush changes
		if( map_reloaded ) {
			glutSwapBuffers(); // if map wasn't reloaded (and buffers weren't swapped), swap buffers
		}
	} else {
		drawMenu();
		glFlush(); // flush changes
		glutSwapBuffers();
	}

}

void Drawing::renderBitmapString( float x, float y, float z, void *font, char *string )
{
	char *c;
	glRasterPos3f( x, y, z );
	for( c = string; *c != '\0'; c++ ) {
		glutBitmapCharacter( font, *c );
	}
}

void Drawing::drawMenu()
{

	float height = glutGet( GLUT_WINDOW_HEIGHT ),
		width = glutGet( GLUT_WINDOW_WIDTH );
	int size = game->menuChoice.size();
	void* font = GLUT_BITMAP_TIMES_ROMAN_24;
	glColor3f( 255, 255, 255 );
	renderBitmapString( width*0.33, height*0.9, 0, font, "Please, choose players" );
	//рисуем подсвеку выделеного пункта меню
	for( int i = 0; i < size; ++i ) {
		switch( game->menuChoice[i] ) {
			case PLAYER:
				glBegin( GL_QUADS ); //Begin quadrilateral coordinates
				glVertex3f( i*width / size, height*0.55, 0 );
				glVertex3f( ( i + 1 )*width / size, height*0.55, 0 );
				glVertex3f( ( i + 1 )*width / size, height*0.65, 0 );
				glVertex3f( i*width / size, height*0.65, 0 );
				glEnd();
				break;
			case AI:
				glBegin( GL_QUADS ); //Begin quadrilateral coordinates
				glVertex3f( i*width / size, height*0.45, 0 );
				glVertex3f( ( i + 1 )*width / size, height*0.45, 0 );
				glVertex3f( ( i + 1 )*width / size, height*0.55, 0 );
				glVertex3f( i*width / size, height*0.55, 0 );
				glEnd();
				break;
			case NONE:
				glBegin( GL_QUADS ); //Begin quadrilateral coordinates
				glVertex3f( i*width / size, height*0.35, 0 );
				glVertex3f( ( i + 1 )*width / size, height*0.35, 0 );
				glVertex3f( ( i + 1 )*width / size, height*0.45, 0 );
				glVertex3f( i*width / size, height*0.45, 0 );
				glEnd();
				break;
		}
	}
	//рисуем кнопку далее
	glBegin( GL_QUADS ); //Begin quadrilateral coordinates
	glVertex3f( 0, height * 0, 0 );
	glVertex3f( width, height * 0, 0 );
	glVertex3f( width, height*0.25, 0 );
	glVertex3f( 0, height*0.25, 0 );
	glEnd();
	glColor3f( 0, 0, 0 );
	glBegin( GL_TRIANGLES ); //Begin triangle coordinates
	glVertex3f( width*0.33, height*0.08, 0 );
	glVertex3f( width*0.33, height*0.16, 0 );
	glVertex3f( width*0.66, height*0.12, 0 );
	glEnd(); //End triangle coordinates
	//рисуем выбор типа
	for( int i = 0; i < size; ++i ) {
		switch( i ) {
			case( 0 ) : glColor3f( 255, 0, 0 ); break;
			case( 1 ) : glColor3f( 0, 255, 0 ); break;
			case( 2 ) : glColor3f( 0, 0, 255 ); break;
			case( 3 ) : glColor3f( 245, 255, 0 ); break;
		};
		char str[2];
		itoa( i + 1, str, 10 );
		renderBitmapString( i*width / size, height*0.7, 0, font, str );
		renderBitmapString( i*width / size, height*0.6, 0, font, "PLAYER" );
		renderBitmapString( i*width / size, height*0.5, 0, font, "AI" );
		renderBitmapString( i*width / size, height*0.4, 0, font, "NONE" );
	}
}

// load image from file to texture
void Drawing::Load_texture( const char* filename, GLuint& texture )
{
	// create and choose texture
	glGenTextures( 1, &texture );
	glBindTexture( GL_TEXTURE_2D, texture );
	// read from file
	int width, height;
	int channels;
	unsigned char* image = SOIL_load_image( filename, &width, &height, &channels, SOIL_LOAD_RGBA );
	// save data to texture
	gluBuild2DMipmaps( GL_TEXTURE_2D, 4, width, height, GL_RGBA, GL_UNSIGNED_BYTE, image );
	glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE,
			   ( float ) GL_MODULATE );
	SOIL_free_image_data( image );
	// unchoose texture
	glBindTexture( GL_TEXTURE_2D, 0 );
}

void Drawing::load()
{
	//load textures for map
	Load_texture( "..\\images\\road.png", map.texture_road ); // road
	Load_texture( "..\\images\\forest.png", map.texture_board ); // board
	//load textures for cars (depends on color)
	std::string car_filename;
	for( size_t i = 0; i < cars.size(); i++ ) {
		switch( cars[i].Get_color() ) {
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
		Load_texture( car_filename.c_str(), cars[i].texture );
	}
}

void Drawing::OnMove( int direction )
{
	int winner = game->getPlayerOnFinish();
	if( winner != -1 ) {
		OnWin( winner );
		return;
	}
	int currentPlayer = game->current_player;
	if( game->game_ready_to_start && game->playerIsAlive( currentPlayer ) ) {
		int numOfCrushedCar;
		game->turnOfPlayer( currentPlayer, direction, numOfCrushedCar );
		if( game->numberOfPlayers == game->numOfDeadPlayers ) {
			OnDeathAll();
			return;
		}
		PointsInformation pointInfo = game->getPlayersBasePoints( currentPlayer );
		cars[game->current_player].MoveTo( Coord( pointInfo.currentCoordinates.x, pointInfo.currentCoordinates.y ),
										   !pointInfo.isAlive );

		if( numOfCrushedCar != -1 ) { // Вторая машина отправляется на старт
			PointsInformation crushedCurPointInfo = game->getPlayersBasePoints( numOfCrushedCar );
			cars[numOfCrushedCar].MoveTo( Coord( crushedCurPointInfo.currentCoordinates.x, crushedCurPointInfo.currentCoordinates.y ),
										  !crushedCurPointInfo.isAlive );
		}

		if( !pointInfo.isAlive ) {
			OnDeath( currentPlayer );
		}

		++game->current_player; // Поиск живых игроков
		game->current_player %= game->numberOfPlayers;
		while( !game->playerIsAlive( game->current_player ) ) {
			++game->current_player;
			game->current_player %= game->numberOfPlayers;
		}
	}
}

void Drawing::OnWin( int winner )
{
	wchar_t buffer[256];
	::wsprintf( buffer, L"Игрок №%d победил! Хотите начать заново?", winner + 1 );
	int result = ::MessageBox( nullptr, buffer, L"Победа!", MB_OKCANCEL );
	if( result == 0 ) {
		throw std::runtime_error( "OnMove: can't show message box" );
	}
	if( result == 1 ) {
		// ok
		game->resetSettings();
		menu = true;
		cars.clear();
	}
	if( result == 2 ) {
		// cancel
		exit( 0 );
	}
}

void Drawing::OnDeathAll()
{
	int result = ::MessageBox( nullptr, L"Все мертвы. Хотите начать заново?", L"Все мертвы", MB_OKCANCEL );
	if( result == 0 ) {
		throw std::runtime_error( "OnDeathAll: can't show message box" );
	}
	if( result == 1 ) {
		// ok
		game->resetSettings();
		menu = true;
		cars.clear();
	}
	if( result == 2 ) {
		// cancel
		exit( 0 );
	}
}

void Drawing::OnDeath( int currentPlayer )
{
	wchar_t buffer[256];
	::wsprintf( buffer, L"Игрок №%d мертв", currentPlayer + 1 );
	if( ::MessageBox( nullptr, buffer, L"Авария", MB_OK ) == 0 ) {
		throw std::runtime_error( "OnDeathAll: can't show message box" );
	}
}

void Drawing::normalKeyHandler( unsigned char key, int x, int y )
{
	switch( key ) {
		case 55:
			OnMove( 7 );
			break;
		case 56:
			OnMove( 8 );
			break;
		case 57:
			OnMove( 9 );
			break;
		case 52:
			OnMove( 4 );
			break;
		case 53:
			OnMove( 5 );
			break;
		case 54:
			OnMove( 6 );
			break;
		case 49:
			OnMove( 1 );
			break;
		case 50:
			OnMove( 2 );
			break;
		case 51:
			OnMove( 3 );
			break;
	}
}

int Drawing::clickButton( int x, int y )
{
	float height = glutGet( GLUT_WINDOW_HEIGHT ),
		width = glutGet( GLUT_WINDOW_WIDTH );
	y = height - y;
	int size = game->menuChoice.size();
	//обработка меню
	for( int i = 0; i < size; i++ ) {
		if( x > i*width / size && x < ( i + 1 )*width / size && y > height*0.55 && y < height*0.65 )
			game->menuChoice[i] = PLAYER;
		if( x > i*width / size && x < ( i + 1 )*width / size && y > height*0.45 && y < height*0.55 )
			game->menuChoice[i] = AI;
		if( x > i*width / size && x < ( i + 1 )*width / size && y > height*0.35 && y < height*0.45 )
			game->menuChoice[i] = NONE;
	}
	// Обработка "Далее"
	if( y < height*0.25 ) { // Нажали "Далее"
		game->calculateNumOfPlayers();
		game->initPlayers();
		initCars();
		for( int i = 0; i < game->numberOfPlayers; i++ ) {
			PointsInformation pi = game->getPlayersBasePoints( i );
			Coord c;
			c.x = pi.currentCoordinates.x;
			c.y = pi.currentCoordinates.y;
			cars[i].current_coords = c;
			cars[i].next_coords = c;
			cars[i].current_angle = 0;
		}
		game->game_ready_to_start = true;
		return 1;
	} else
		return -1;
}

void Drawing::initCars()
{
	for( size_t i = 0; i < game->numberOfPlayers; ++i ) {
		cars.push_back( Car( Green ) ); // todo: цветовая заглушка
	}
	load(); // Загрузка текстур + машинок
}

void Drawing::mouseButton( int button, int state, int x, int y )
{
	if( button == GLUT_LEFT_BUTTON )
		// when the button is released
		if( state == GLUT_UP ) {
			if( clickButton( x, y ) > 0 )
				menu = false;
		}
}

void Drawing::startDrawing( int argc, char * argv[] )
{
	glutInit( &argc, argv );
	glutInitDisplayMode( GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA );
	glutInitWindowSize( 800, 600 );
	glutCreateWindow( "Rock'n'Roll Race" );

	glutKeyboardFunc( normalKeyHandler ); // Обработка нажатий клавиш от игроков
	glutMouseFunc( mouseButton );

	glutTimerFunc( 1, timer, 0 ); // каждую милисекунду говорит что окно нуждается в перерисовке
	glutReshapeFunc( reshape ); // invoke reshape при изменении окна
	glutDisplayFunc( display ); // если нужно сделать redisplay (callback на redisplay присылается из timer)

	glutMainLoop();
}