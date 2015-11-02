#include "Drawing.h"

Map Drawing::map; // static data members must be explicitly defined in exactly one compilation unit
std::vector<Car> Drawing::cars;

Drawing::Drawing( Map &map_data, std::vector<Car> &_cars )
{
	map = map_data;
	cars = _cars;
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
	glutPostRedisplay();
	glutTimerFunc( 1, timer, 0 );
}


void Drawing::display()
{
	glClearColor( 1.0, 1.0, 1.0, 0.0 ); // clear background to white
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT ); // clear buffers

	bool map_reloaded = !map.Need_to_reload();
	map.Draw(); // draw the map
	float cell_size = map.Get_cell_size(); // calculate the size of cell
	for( size_t i = 0; i < cars.size(); i++ ) {
		cars[i].Draw( map.Get_cell_size(), map.Get_indent() ); // draw car
	}
	glFlush(); // flush changes
	if( map_reloaded ) {
		glutSwapBuffers(); // if map wasn't reloaded (and buffers weren't swapped), swap buffers
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

void Drawing::normalKeyHandler( unsigned char key, int x, int y )
{
	switch( key ) {
		case 55:
			// OnMove(7, map.getSize() );
			break;
		case 56:
			// OnMove(8, map.getSize() );
			break;
		case 57:
			// OnMove(9, map.getSize() );
			break;
		case 52:
			// OnMove(4, map.getSize() );
			break;
		case 53:
			// OnMove(5, map.getSize() );
			break;
		case 54:
			// OnMove(6, map.getSize() );
			break;
		case 49:
			// OnMove(1, map.getSize() );
			break;
		case 50:
			// OnMove(2, map.getSize() );
			break;
		case 51:
			// OnMove(3, map.getSize() );
			break;
	}
}

void Drawing::draw( int argc, char * argv[] )
{
	glutInit( &argc, argv );
	glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGBA );
	glutInitWindowSize( 800, 600 );
	glutCreateWindow( "Rock'n'Roll Race" );

	glutKeyboardFunc( normalKeyHandler ); // Обработка нажатий клавиш от игроков

	load();

	glutTimerFunc( 1, timer, 0 );
	glutReshapeFunc( reshape );
	glutDisplayFunc( display );

	glutMainLoop();
}