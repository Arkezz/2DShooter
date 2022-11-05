#include "player.h"
extern int grid[15][20];

Player::Player()
{
	//Set scaled pixmap from .qrc file
	anim_index = 2;
	setFlags(this->flags() | QGraphicsPixmapItem::ItemIsFocusable);

	anim[RIGHT].push_back(QPixmap(":/entities/side_0"));
	anim[RIGHT].push_back(QPixmap(":/entities/side_1"));
	anim[RIGHT].push_back(QPixmap(":/entities/side_2"));
	anim[RIGHT].push_back(QPixmap(":/entities/side_3"));
	anim[RIGHT].push_back(QPixmap(":/entities/side_4"));
	anim[RIGHT].push_back(QPixmap(":/entities/side_5"));

    anim[UP].push_back(QPixmap(":/entities/up_0"));
    anim[UP].push_back(QPixmap(":/entities/up_1"));
    anim[UP].push_back(QPixmap(":/entities/up_2"));
    anim[UP].push_back(QPixmap(":/entities/up_3"));
    anim[UP].push_back(QPixmap(":/entities/up_4"));
    anim[UP].push_back(QPixmap(":/entities/up_5"));

	anim[LEFT].push_back(QPixmap(":/entities/side_0"));
	anim[LEFT].push_back(QPixmap(":/entities/side_1"));
	anim[LEFT].push_back(QPixmap(":/entities/side_2"));
	anim[LEFT].push_back(QPixmap(":/entities/side_3"));
	anim[LEFT].push_back(QPixmap(":/entities/side_4"));
	anim[LEFT].push_back(QPixmap(":/entities/side_5"));

	anim[DOWN].push_back(QPixmap(":/entities/down_0"));
	anim[DOWN].push_back(QPixmap(":/entities/down_1"));
	anim[DOWN].push_back(QPixmap(":/entities/down_2"));
	anim[DOWN].push_back(QPixmap(":/entities/down_3"));
	anim[DOWN].push_back(QPixmap(":/entities/down_4"));

	setPixmap(anim[RIGHT][anim_index]);
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
			//Play down anims flipped vertically
			if (anim_index < anim[UP].size() - 1) {
				anim_index++;
			}
			else {
				anim_index = 0;
			}
            setPixmap(anim[UP][anim_index].scaled(128,128));
		}
		break;
	case DOWN:
		//If the tile below the player is empty, move the player down
		if (grid[y + 1][x] == 0) {
            setPos(x * 32, (y + 1) * 32);
			//Play the down anims
			if (anim_index < 4) {
				anim_index++;
			}
			else {
				anim_index = 0;
			}
			setPixmap(anim[DOWN][anim_index].scaled(128, 128));
		}
		break;
	case LEFT:
		//If the tile to the left of the player is empty, move the player left
        if (grid[y][x - 1] == 0) {
            setPos((x - 1) * 32, y * 32);
			//Play the side animS
			anim_index++;
			if (anim_index > 5) {
				anim_index = 0;
			}
			setPixmap(anim[LEFT][anim_index].scaled(128, 128));
		}
		break;
	case RIGHT:
		//If the tile to the right of the player is empty, move the player right
		if (grid[y][x + 1] == 0) {
            setPos((x + 1) * 32, y * 32);
			//Play the side animS flipped around
            anim_index++;
			if (anim_index > 5) {
				anim_index = 0;
			}
			setPixmap(anim[RIGHT][anim_index].transformed(QTransform().scale(-1, 1)).scaled(128, 128));
		}
		break;
	}
}
