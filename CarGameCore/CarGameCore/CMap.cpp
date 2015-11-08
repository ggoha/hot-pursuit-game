#include "CMap.h"

Map::Map()
{
	size.first = 0;
	size.second = 0;
}

Map::Map( const Size& newSize, const Field& newMap ) :
	size( newSize ), map( newMap )
{
}

Map::~Map()
{
}

void Map::print()
{
	for( size_t i = 0; i < size.second; ++i ) {
		for( size_t j = 0; j < size.first; ++j ) {
			std::cout << map[i][j] << " ";
		}
		std::cout << std::endl;
	}
}

void Map::setPosition( int x, int y )
{
	map[y][x] = 5;
}

void Map::clearPosition( int x, int y )
{
	map[y][x] = 0;
}

Size Map::getSize()
{
	return this->size;
}

bool Map::isEmpty( int i, int j )
{
	return ( map[j][i] != 1 );
}

/*visual part*/

Map::Map( Field& newMap )
{
	map = newMap;
	cell_size = 0;
	indent = WCoord( 0, 0 );
	texture_board = 0;
	texture_map = 0;
	texture_road = 0;
	glGenTextures( 1, &texture_map );
	glGenTextures( 1, &texture_board );
	glGenTextures( 1, &texture_road );
	need_reload = true;
}

bool Map::Need_to_reload()
{
	return need_reload;
}

void Map::Calculate()
{
	int n = map.size(), m = map[0].size();
	float height = glutGet( GLUT_WINDOW_HEIGHT ),
		width = glutGet( GLUT_WINDOW_WIDTH );
	cell_size = fmin( height / n, width / m ); // the length of one little square - "cell"
	indent.x = ( width - cell_size * m ) / 2; // indent from left and right window sides
	indent.y = ( height - cell_size * n ) / 2;  // indent from top and bottom window sides
	need_reload = true; // need to reload map
}

void Map::save_texture()
{
	// write window to array of pixels
	unsigned long imageSize;
	GLint viewPort[4];
	glGetIntegerv( GL_VIEWPORT, viewPort );
	GLint width = viewPort[2];
	GLint height = viewPort[3];
	imageSize = ( ( width + ( ( 4 - ( width % 4 ) ) % 4 ) )*height * 3 );
	std::shared_ptr<GLbyte> data = std::shared_ptr<GLbyte>( new GLbyte[imageSize] );
	glReadBuffer( GL_FRONT );
	glReadPixels( 0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, data.get() );

	// write pixels to texture of map
	glBindTexture( GL_TEXTURE_2D, texture_map );
	gluBuild2DMipmaps( GL_TEXTURE_2D, 3, width, height, GL_RGB, GL_UNSIGNED_BYTE, data.get() );
	glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE,
			   ( float ) GL_MODULATE );
}

void Map::reload()
{
	int n = map.size(), m = map[0].size();
	for( int i = 0; i < n; i++ ) {
		for( int j = 0; j < m; j++ ) {
			glEnable( GL_TEXTURE_2D );
			switch (map[i][j])
			{
			case 1: glBindTexture(GL_TEXTURE_2D, texture_board); break; // load a texture of board (forest)
			case 0: glBindTexture(GL_TEXTURE_2D, texture_road); break; // load a texture of board (road)
			case 2: glBindTexture(GL_TEXTURE_2D, texture_finish); break; // load a texture of board (start)
			case 3: glBindTexture(GL_TEXTURE_2D, texture_finish); break; // load a texture of board (finish)
			}
			//calculate coordinates
			float left = j * cell_size + indent.x;
			float right = ( j + 1 ) * cell_size + indent.x;
			float bottom = i * cell_size + indent.y;
			float top = ( i + 1 ) * cell_size + indent.y;
			//draw a cell with texture (board or road)
			glBegin( GL_POLYGON );
			glTexCoord2f( 0.0f, 0.0f ); glVertex3f( left, bottom, 0.0f );
			glTexCoord2f( 1.0f, 0.0f ); glVertex3f( right, bottom, 0.0f );
			glTexCoord2f( 1.0f, 1.0f ); glVertex3f( right, top, 0.0f );
			glTexCoord2f( 0.0f, 1.0f ); glVertex3f( left, top, 0.0f );
			glEnd();
			glDisable(GL_TEXTURE_2D);
		}
	}
	glutSwapBuffers();
	save_texture(); // save the whole window with map to texture
	need_reload = false;
}

Map Map::getMapInOpenGLView() {
	Map m(size, map);
	m.map.resize( size.first );
	for( int i = 0; i < size.first; i++ ) {
		m.map[i].resize(size.second);
		for( int j = 0; j < size.second; j++ ){
			m.map[i][j] = map[size.first - 1 - i][j];
		}
	}
	return m;
}

void Map::Draw()
{
	if( need_reload ) {
		reload();
		return;
	}
	// load window from texture
	glEnable( GL_TEXTURE_2D );
	int height = glutGet( GLUT_WINDOW_HEIGHT ),
		width = glutGet( GLUT_WINDOW_WIDTH );
	// choose texture
	glBindTexture( GL_TEXTURE_2D, texture_map );
	// draw a polygon of window size with texture
	glBegin( GL_POLYGON );
	glTexCoord2f( 0.0f, 0.0f ); glVertex3f( 0, 0, 0.0f );
	glTexCoord2f( 1.0f, 0.0f ); glVertex3f( width, 0, 0.0f );
	glTexCoord2f( 1.0f, 1.0f ); glVertex3f( width, height, 0.0f );
	glTexCoord2f( 0.0f, 1.0f ); glVertex3f( 0, height, 0.0f );
	glEnd();
	glDisable(GL_TEXTURE_2D);

}

float Map::Get_cell_size()
{
	return cell_size;
}

WCoord Map::Get_indent()
{
	return indent;
}
