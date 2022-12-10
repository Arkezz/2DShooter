#include "player.h"
extern int grid[15][20];
const int tileLen = 32;
const int playerLen = 64;

Player::Player()
{
    // Initialize member variables

    // Fill the animation vectors with appropriate images
    animFiller();

    // Set initial direction
    dir = RIGHT;
    setPixmap(anim[RIGHT][anim_index].transformed(QTransform().scale(-1, 1)).scaled(playerLen, playerLen));

    // Player is idle initially
    isIdle = true;

    // Set initial health and ammo
    health = 3;
    ammo = 0;

    // Set initial state
    status = normal;

    // Initialize timers
    idleTimer = new QTimer(this);
    keyPressTimer = new QTimer(this);
    moveTimer = new QTimer(this);

    // Connect timer signals and slots
    connect(idleTimer, SIGNAL(timeout()), this, SLOT(animHandler()));
    connect(keyPressTimer, &QTimer::timeout, this, [=]() {
        idleTimer->start(200);
        });

    // Create sound manager object
    sounds = new soundManager();

     // Create drop shadow effect
    setFlags(this->flags() | QGraphicsPixmapItem::ItemIsFocusable);
    shadow = new QGraphicsDropShadowEffect;
    setGraphicsEffect(shadow);

	idleTimer->start(200);
}

void Player::reset() {
	health = 3;
	ammo = 0;
	dir = RIGHT;
	isIdle = true;
	status = normal;
    anim_index = 2;
    idle_index = 0;

	setPixmap(anim[RIGHT][anim_index].transformed(QTransform().scale(-1, 1)).scaled(playerLen, playerLen));
}

void Player::animHandler()
{
    if (isIdle)
       {
           idle_index++;
           // Check if the idle animation index is within bounds
           if (idle_index >= idleAnim[dir].size())
               idle_index = 0;

           // Set the current pixmap to the appropriate idle animation frame
           setPixmap(idleAnim[dir][idle_index].transformed(QTransform().scale(-1, 1)).scaled(playerLen, playerLen));
       }
}

void Player::pickUp() {
	//Play pickup anim

	pickup_index++;
	if (pickup_index >= pickupAnim.size())
        pickup_index = 0;
	//Play the anim according to the direction
	if (dir == LEFT) {
		setPixmap(pickupAnim[pickup_index].scaled(playerLen, playerLen));
	}
	else if (dir == RIGHT)
	{
		setPixmap(pickupAnim[pickup_index].transformed(QTransform().scale(-1, 1)).scaled(playerLen, playerLen));
	}
	//If up or down randomly do the left or right anim
    else
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
    for (int i = 0; i < 6; i++) {
            anim[RIGHT].push_back(QPixmap(":/entities/side_" + QString::number(i)));
            anim[UP].push_back(QPixmap(":/entities/up_" + QString::number(i)));
            anim[LEFT].push_back(QPixmap(":/entities/side_" + QString::number(i)));
            anim[DOWN].push_back(QPixmap(":/entities/down_" + QString::number(i)));
        }
    for (int i = 0; i < 5; i++) {
         idleAnim[RIGHT].push_back(QPixmap(":/entities/sideIdle_" + QString::number(i)));
         idleAnim[UP].push_back(QPixmap(":/entities/upIdle_" + QString::number(i)));
         idleAnim[LEFT].push_back(QPixmap(":/entities/sideIdle_" + QString::number(i)).transformed(QTransform().scale(-1, 1)).scaled(playerLen, playerLen));
         idleAnim[DOWN].push_back(QPixmap(":/entities/downIdle_" + QString::number(i)));
         pickupAnim.push_back(QPixmap(":/entities/pickup_" + QString::number(i)));
     }
}
