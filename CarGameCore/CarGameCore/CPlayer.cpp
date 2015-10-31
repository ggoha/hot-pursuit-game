#include "CPlayer.h"

Player::Player()
{
	isAlive = true;
	inertia.x = 0;
	inertia.y = 0;
	cheated = 0;
}

Player::Player( const Coordinates& newCoordinates, const bool state ) :
	initial_position( newCoordinates ), previous_position( newCoordinates ), position( newCoordinates ), isAlive( state )
{
	inertia.x = 0;
	inertia.y = 0;
	cheated = 0;
}

Player::~Player()
{
}

Coordinates Player::getPosition()
{
	return this->position;
};

void Player::goToStart()
{
	this->position = this->initial_position;
	this->previous_position = this->initial_position;
	this->inertia.x = 0;
	this->inertia.y = 0;
};

Coordinates Player::convertFromDirectionCode( int directionCode )
{
	Coordinates direction( 0, 0 );
	switch( directionCode ) {
		case SW:
			direction.x = -1;
			direction.y = +1;
			break;
		case S:
			direction.x = 0;
			direction.y = +1;
			break;
		case SE:
			direction.x = +1;
			direction.y = +1;
			break;
		case W:
			direction.x = -1;
			direction.y = 0;
			break;
		case C:
			direction.x = 0;
			direction.y = 0;
			break;
		case E:
			direction.x = +1;
			direction.y = 0;
			break;
		case NW:
			direction.x = -1;
			direction.y = -1;
			break;
		case N:
			direction.x = 0;
			direction.y = -1;
			break;
		case NE:
			direction.x = +1;
			direction.y = -1;
			break;
		default:
			throw std::invalid_argument( "Unknown direction code" );
	}
	return direction;
}

void Player::move( int direction_code, Size size )
{
	Coordinates direction = convertFromDirectionCode( direction_code );
	this->moveInDirection( direction );
};

void Player::moveInDirection( Coordinates direction )
{
	Coordinates move;
	move.x = this->inertia.x + direction.x;
	move.y = this->inertia.y + direction.y;

	Coordinates pure_inertial; // Точка, куда по инерции приходим
	pure_inertial.x = this->position.x + this->inertia.x;
	pure_inertial.y = this->position.y + this->inertia.y;

	this->inertia = move;

	this->previous_position = this->position;

	this->position.x = this->position.x + move.x;
	this->position.y = this->position.y + move.y;
}

Coordinates Player::getPreviousPosition()
{
	return this->previous_position;
};

void Player::die()
{
	isAlive = false;
};

bool Player::directionIsValid( const Size& size )
{
	if( 0 <= position.x && position.x <= size.first - 1 &&
		0 <= position.y && position.y <= size.second - 1 ) {
		return true;
	} else {
		return false;
	}
}

bool Player::playerIsAlive()
{
	return isAlive;
}

void Player::cheat()
{
	++cheated;
}

bool Player::isCheater()
{
	return cheated > 0;
}

void Player::reduceCheat()
{
	--cheated;
}