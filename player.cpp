#include "player.h"
extern int grid[15][20];

Player::Player()
{
	health = 3;
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
	anim[DOWN].push_back(QPixmap(":/entities/down_5"));

	idleAnim[RIGHT].push_back(QPixmap(":/entities/sideIdle_0"));
	idleAnim[RIGHT].push_back(QPixmap(":/entities/sideIdle_1"));
	idleAnim[RIGHT].push_back(QPixmap(":/entities/sideIdle_2"));
	idleAnim[RIGHT].push_back(QPixmap(":/entities/sideIdle_3"));
	idleAnim[RIGHT].push_back(QPixmap(":/entities/sideIdle_4"));

	idleAnim[UP].push_back(QPixmap(":/entities/upIdle_0"));
	idleAnim[UP].push_back(QPixmap(":/entities/upIdle_1"));
	idleAnim[UP].push_back(QPixmap(":/entities/upIdle_2"));
	idleAnim[UP].push_back(QPixmap(":/entities/upIdle_3"));
	idleAnim[UP].push_back(QPixmap(":/entities/upIdle_4"));

	idleAnim[LEFT].push_back(QPixmap(":/entities/sideIdle_0").transformed(QTransform().scale(-1, 1)).scaled(128, 128));
	idleAnim[LEFT].push_back(QPixmap(":/entities/sideIdle_1").transformed(QTransform().scale(-1, 1)).scaled(128, 128));
	idleAnim[LEFT].push_back(QPixmap(":/entities/sideIdle_2").transformed(QTransform().scale(-1, 1)).scaled(128, 128));
	idleAnim[LEFT].push_back(QPixmap(":/entities/sideIdle_3").transformed(QTransform().scale(-1, 1)).scaled(128, 128));
	idleAnim[LEFT].push_back(QPixmap(":/entities/sideIdle_4").transformed(QTransform().scale(-1, 1)).scaled(128, 128));

	idleAnim[DOWN].push_back(QPixmap(":/entities/downIdle_0"));
	idleAnim[DOWN].push_back(QPixmap(":/entities/downIdle_1"));
	idleAnim[DOWN].push_back(QPixmap(":/entities/downIdle_2"));
	idleAnim[DOWN].push_back(QPixmap(":/entities/downIdle_3"));
	idleAnim[DOWN].push_back(QPixmap(":/entities/downIdle_4"));

	setPixmap(anim[RIGHT][anim_index].transformed(QTransform().scale(-1, 1)).scaled(128, 128));
	//Make a timer to animate the idle animation
	keyPressTimer = new QTimer(this);
	idleTimer = new QTimer(this);
	moveTimer = new QTimer(this);
	connect(idleTimer, SIGNAL(timeout()), this, SLOT(animHandler()));
	connect(keyPressTimer, &QTimer::timeout, [=]() {
		idleTimer->start(200);
		});
	idleTimer->start(200);
}

void Player::setDir(dirF dir)
{
	this->dir = dir;
}

int Player::getHealth()
{
	return health;
}

void Player::shoot() {
	//Checks all the tiles in the direction the player is facing if it finds an enemy do damage to the enemy
	if (dir == RIGHT) {
		for (int i = 0; i < 20; i++) {
			if (grid[(int)(y() / 64)][(int)((x() + 64) / 64) + i] == 2) {
				//Do damage to enemy
				break;
			}
		}
	}
	else if (dir == LEFT) {
		for (int i = 0; i < 20; i++) {
			if (grid[(int)(y() / 64)][(int)((x() - 64) / 64) - i] == 2) {
				//Do damage to enemy
				break;
			}
		}
	}
	else if (dir == UP) {
		for (int i = 0; i < 20; i++) {
			if (grid[(int)((y() - 64) / 64) - i][(int)(x() / 64)] == 2) {
				//Do damage to enemy
				break;
			}
		}
	}
	else if (dir == DOWN) {
		for (int i = 0; i < 20; i++) {
			if (grid[(int)((y() + 64) / 64) + i][(int)(x() / 64)] == 2) {
				//Do damage to enemy
				break;
			}
		}
	}
}

void Player::animHandler()
{
	if (isIdle)
	{
		anim_index++;
		if (anim_index >= idleAnim[dir].size())
			anim_index = 0;
		setPixmap(idleAnim[dir][anim_index].transformed(QTransform().scale(-1, 1)).scaled(128, 128));
	}
	else
	{
		anim_index++;
		if (anim_index >= anim[dir].size())
			anim_index = 0;
		setPixmap(anim[dir][anim_index].transformed(QTransform().scale(-1, 1)).scaled(128, 128));
	}
}

//Collision handler
void Player::collisionHandler()
{
	//Check the colliding items with the player
	QList<QGraphicsItem*> colliding_items = collidingItems();
	for (int i = 0; i < colliding_items.size(); i++) {
		//If the colliding item is an enemy, do damage to the enemy
		if (typeid(*(colliding_items[i])) == typeid(Enemy)) {
			health--;
			emit drawUi();
			//Animate the player getting hit
			setPixmap(QPixmap(":/entities/playerHit").transformed(QTransform().scale(-1, 1)).scaled(128, 128));
			//Move the player in the opposite direction
			if (dir == RIGHT) {
				setPos(x() - 64, y());
			}
			else if (dir == LEFT) {
				setPos(x() + 64, y());
			}
			else if (dir == UP) {
				setPos(x(), y() + 64);
			}
			else if (dir == DOWN) {
				setPos(x(), y() - 64);
			}
		}
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
	move();
}

//Move player function handle animations and collisions and dont let player go outside the map
void Player::move() {
	//Create frame interpolation for walking animation and movement
	{
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
			}
			collisionHandler();
		}
		//Once the movement ends set the player back to idle and start the idle animation
		isIdle = true;
		keyPressTimer->start(200);
	}
}