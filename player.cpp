#include "player.h"
extern int grid[15][20];

Player::Player()
{
	dir = RIGHT;
	anim_index = 2;
	setFlags(this->flags() | QGraphicsPixmapItem::ItemIsFocusable);
	isIdle = true;

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

	setPixmap(anim[RIGHT][anim_index].transformed(QTransform().scale(-1, 1)).scaled(128, 128));
	//Make a timer to animate the idle animation
	keyPressTimer = new QTimer(this);
	idleTimer = new QTimer(this);
	connect(idleTimer, SIGNAL(timeout()), this, SLOT(animateIdle()));
	connect(keyPressTimer, &QTimer::timeout, [=]() {
		idleTimer->start(200);
		});
	idleTimer->start(200);
}

void Player::setDir(dirF dir)
{
	this->dir = dir;
}

void Player::animateIdle()
{
	if (isIdle)
	{
		anim_index++;
		if (anim_index >= anim[dir].size())
			anim_index = 0;
		setPixmap(anim[dir][anim_index].transformed(QTransform().scale(-1, 1)).scaled(128, 128));
	}
}

void Player::keyPressEvent(QKeyEvent* event)
{
	idleTimer->stop();
	if (event->key() == Qt::Key_W) {
		setDir(UP);
		isIdle = false;
	}
	else if (event->key() == Qt::Key_D) {
		setDir(RIGHT);
		isIdle = false;
	}
	else if (event->key() == Qt::Key_A) {
		setDir(LEFT);
		isIdle = false;
	}
	else if (event->key() == Qt::Key_S) {
		setDir(DOWN);
		isIdle = false;
	}
	//Get the players position
	int x = pos().x() / 32;
	int y = pos().y() / 32;
	if (!isIdle)
	{
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
				setPixmap(anim[UP][anim_index].scaled(128, 128));
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

	//Start a new timer once 1 second passes atnewecondtimeresoncet th1laysecpasses the player is idle
	isIdle = true;
	keyPressTimer->start(1000);
	//If a second passes and the player is still idle, start the idle animation
}