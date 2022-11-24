#include "player.h"
extern int grid[15][20];
const int tileLen = 32;
const int playerLen = 64;

Player::Player()
{
	animFiller();
	health = 3;
	ammo = 0;
	dir = RIGHT;
	isIdle = true;
	status = normal;
	setFlags(this->flags() | QGraphicsPixmapItem::ItemIsFocusable);
	shadow = new QGraphicsDropShadowEffect;
	setGraphicsEffect(shadow);

	setPixmap(anim[RIGHT][anim_index].transformed(QTransform().scale(-1, 1)).scaled(playerLen, playerLen));

	sounds = new soundManager();
	keyPressTimer = new QTimer(this);
	idleTimer = new QTimer(this);
	moveTimer = new QTimer(this);
	connect(idleTimer, SIGNAL(timeout()), this, SLOT(animHandler()));
	connect(keyPressTimer, &QTimer::timeout, [=]() {
		idleTimer->start(200);
		});
	idleTimer->start(200);
}

void Player::shoot() {
	if (ammo > 0) {
		ammo--;
		sounds->playSound("shoot");
	}
}

void Player::animHandler()
{
	if (isIdle)
	{
		idle_index++;
		if (idle_index >= idleAnim[dir].size())
			idle_index = 0;
		setPixmap(idleAnim[dir][idle_index].transformed(QTransform().scale(-1, 1)).scaled(playerLen, playerLen));
	}
}

void Player::pickUp() {
	//Play pickup anim

	pickup_index++;
	if (pickup_index >= pickupAnim.size())
		anim_index = 0;
	//Play the anim according to the direction
	if (dir == LEFT) {
		setPixmap(pickupAnim[pickup_index].scaled(playerLen, playerLen));
	}
	else if (dir == RIGHT)
	{
		setPixmap(pickupAnim[pickup_index].transformed(QTransform().scale(-1, 1)).scaled(playerLen, playerLen));
	}
	//If up or down randomly do the left or right anim
	else if (dir == UP || dir == DOWN)
	{
		QRandomGenerator* rand = QRandomGenerator::global();
		int randn = rand->bounded(0, 2);
		if (randn == 0)
		{
			setPixmap(pickupAnim[pickup_index].transformed(QTransform().scale(-1, 1)).scaled(playerLen, playerLen));
		}
		else
		{
			setPixmap(pickupAnim[pickup_index].scaled(playerLen, playerLen));
		}
	}
	pickup_index = 0;
}

void Player::keyPressEvent(QKeyEvent* event)
{
	emit drawFootsteps();

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
	else if (event->key() == Qt::Key_Space) {
		shoot();
	}
	else if (event->key() == Qt::Key_Escape) {
		emit openSettings();
	}
	move();
}

void Player::move() {
	{
		//Get the players position
		int x = pos().x() / tileLen;
		int y = pos().y() / tileLen;

		if (!isIdle)
		{
			switch (dir) {
			case UP:
				//If the tile above the player is empty, move the player up
				if (grid[y - 1][x] < 15) {
					setPos(x * tileLen, (y - 1) * tileLen);
					//Play down anims flipped vertically
					if (anim_index < anim[UP].size() - 1) {
						anim_index++;
					}
					else {
						anim_index = 0;
					}
					setPixmap(anim[UP][anim_index].scaled(playerLen, playerLen));
				}
				break;
			case DOWN:
				//If the tile below the player is empty, move the player down
				if (grid[y + 1][x] < 15) {
					setPos(x * tileLen, (y + 1) * tileLen);
					//Play the down anims
					if (anim_index < 4) {
						anim_index++;
					}
					else {
						anim_index = 0;
					}
					setPixmap(anim[DOWN][anim_index].scaled(playerLen, playerLen));
				}
				break;
			case LEFT:
				//If the tile to the left of the player is empty, move the player left
				if (grid[y][x - 1] < 15) {
					setPos((x - 1) * tileLen, y * tileLen);
					//Play the side animS
					anim_index++;
					if (anim_index > 5) {
						anim_index = 0;
					}
					setPixmap(anim[LEFT][anim_index].scaled(playerLen, playerLen));
				}
				break;
			case RIGHT:
				//If the tile to the right of the player is empty, move the player right
				if (grid[y][x + 1] < 15) {
					setPos((x + 1) * tileLen, y * tileLen);
					//Play the side animS flipped around
					anim_index++;
					if (anim_index > 5) {
						anim_index = 0;
					}
					setPixmap(anim[RIGHT][anim_index].transformed(QTransform().scale(-1, 1)).scaled(playerLen, playerLen));
				}
			}
			emit collisionHandler();
		}
		isIdle = true;
		keyPressTimer->start(200);
	}
}

void Player::animFiller() {
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

	idleAnim[LEFT].push_back(QPixmap(":/entities/sideIdle_0").transformed(QTransform().scale(-1, 1)).scaled(playerLen, playerLen));
	idleAnim[LEFT].push_back(QPixmap(":/entities/sideIdle_1").transformed(QTransform().scale(-1, 1)).scaled(playerLen, playerLen));
	idleAnim[LEFT].push_back(QPixmap(":/entities/sideIdle_2").transformed(QTransform().scale(-1, 1)).scaled(playerLen, playerLen));
	idleAnim[LEFT].push_back(QPixmap(":/entities/sideIdle_3").transformed(QTransform().scale(-1, 1)).scaled(playerLen, playerLen));
	idleAnim[LEFT].push_back(QPixmap(":/entities/sideIdle_4").transformed(QTransform().scale(-1, 1)).scaled(playerLen, playerLen));

	idleAnim[DOWN].push_back(QPixmap(":/entities/downIdle_0"));
	idleAnim[DOWN].push_back(QPixmap(":/entities/downIdle_1"));
	idleAnim[DOWN].push_back(QPixmap(":/entities/downIdle_2"));
	idleAnim[DOWN].push_back(QPixmap(":/entities/downIdle_3"));
	idleAnim[DOWN].push_back(QPixmap(":/entities/downIdle_4"));

	pickupAnim.push_back(QPixmap(":/entities/pickup_0"));
	pickupAnim.push_back(QPixmap(":/entities/pickup_1"));
	pickupAnim.push_back(QPixmap(":/entities/pickup_2"));
	pickupAnim.push_back(QPixmap(":/entities/pickup_3"));
	pickupAnim.push_back(QPixmap(":/entities/pickup_4"));
}