#include "player.h"
extern int grid[15][20];

Player::Player()
{
	//Set scaled pixmap from .qrc file
    setPixmap(QPixmap(":/entities/player").scaled(32, 32));
	setFlags(this->flags() | QGraphicsPixmapItem::ItemIsFocusable);
}

void Player::setDir(dirF dir)
{
	this->dir = dir;
}

void Player::keyPressEvent(QKeyEvent* event)
{
	if (event->key() == Qt::Key_W) {
		setDir(UP);
	}
	else if (event->key() == Qt::Key_D) {
        setDir(RIGHT);
	}
	else if (event->key() == Qt::Key_A) {
		setDir(LEFT);
	}
	else if (event->key() == Qt::Key_S) {
        setDir(DOWN);
	}
    //Get the players position
    int x = pos().x() / 32;
    int y = pos().y() / 32;

    //Check the direction the player is moving in
    switch (dir) {
    case UP:
        //If the tile above the player is empty, move the player up
        if (grid[y - 1][x] == 0) {
            setPos(x * 32, (y - 1) * 32);
        }
        break;
    case DOWN:
        //If the tile below the player is empty, move the player down
        if (grid[y + 1][x] == 0) {
            setPos(x * 32, (y + 1) * 32);
        }
        break;
    case LEFT:
        //If the tile to the left of the player is empty, move the player left
        if (grid[y][x - 1] == 0) {
            setPos((x - 1) * 32, y * 32);
        }
        break;
    case RIGHT:
        //If the tile to the right of the player is empty, move the player right
        if (grid[y][x + 1] == 0) {
            setPos((x + 1) * 32, y * 32);
        }
        break;
    }
}
