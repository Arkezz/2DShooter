#include "player.h"

Player::Player()
{
	//Set scaled pixmap from .qrc file
	setPixmap(QPixmap(":/entities/player.png").scaled(48, 48));
}