#include "enemy.h"
extern int grid[15][20];
const int tileLen = 32;
const int enemyLen = 64;
Enemy::Enemy()
{
	//Make the enmy focusable
	setFlag(QGraphicsItem::ItemIsFocusable);
	animFiller();
	move_index = 0;
	idle_index = 0;
	attack_index = 0;
	death_index = 0;
	health = 2;
	QGraphicsDropShadowEffect* effect = new QGraphicsDropShadowEffect;
	setGraphicsEffect(effect);

	setPixmap(QPixmap(":/enemy1/idle_0").transformed(QTransform().scale(-1, 1)).scaled(enemyLen, enemyLen));
	idleTimer = new QTimer(this);
	attackTimer = new QTimer(this);
	moveTimer = new QTimer(this);
	connect(idleTimer, SIGNAL(timeout()), this, SLOT(animHandler()));
	connect(attackTimer, SIGNAL(timeout()), this, SLOT(attackHandler()));
	connect(moveTimer, SIGNAL(timeout()), this, SLOT(move()));
    //moveTimer->start(500);
	idleTimer->start(400);
}

void Enemy::animHandler()
{
	if (idle_index == 3)
	{
		idle_index = 0;
	}
	else
	{
		idle_index++;
	}
	setPixmap(idleAnim[idle_index].transformed(QTransform().scale(-1, 1)).scaled(enemyLen, enemyLen));
}

void Enemy::deathHandler() {
	death_index++;
	if (death_index >= deathAnim.size())
		return;
	setPixmap(deathAnim[death_index].transformed(QTransform().scale(-1, 1)).scaled(enemyLen, enemyLen));
	idleTimer->stop();
	moveTimer->stop();
}

void Enemy::attackHandler()
{
	while (attack_index != 3) {
		attack_index++;
	}
	//According to the direction of the enemy, the attack animation will be played
	setPixmap(attackAnim[attack_index].transformed(QTransform().scale(-1, 1)).scaled(enemyLen, enemyLen));
	attack_index = 0;
}

void Enemy::move() {
	idleTimer->stop();
	int x = this->x() / tileLen;
	int y = this->y() / tileLen;
	int dir = rand() % 4;
	switch (dir) {
	case 0:
		if (grid[y - 1][x] > 15) {
			this->setPos(this->x(), this->y() - tileLen);
			//Play the move animation
			move_index++;
			if (move_index >= moveAnim.size())
				move_index = 0;
			setPixmap(moveAnim[move_index].transformed(QTransform().scale(-1, 1)).scaled(enemyLen, enemyLen));
		}
		break;
	case 1:
		if (grid[y + 1][x] > 15) {
			this->setPos(this->x(), this->y() + tileLen);
			//Play the move animation
			move_index++;
			if (move_index >= moveAnim.size())
				move_index = 0;
			setPixmap(moveAnim[move_index].transformed(QTransform().scale(-1, 1)).scaled(enemyLen, enemyLen));
		}
		break;
	case 2:
		if (grid[y][x - 1] > 15) {
			this->setPos(this->x() - tileLen, this->y());
			//Play the move animation
			move_index++;
			if (move_index >= moveAnim.size())
				move_index = 0;
			setPixmap(moveAnim[move_index].transformed(QTransform().scale(-1, 1)).scaled(enemyLen, enemyLen));
		}
		break;
	case 3:
		if (grid[y][x + 1] > 15) {
			this->setPos(this->x() + tileLen, this->y());
			//Play the move animation
			move_index++;
			if (move_index >= moveAnim.size())
				move_index = 0;
			setPixmap(moveAnim[move_index].transformed(QTransform().scale(-1, 1)).scaled(enemyLen, enemyLen));
		}
		break;
	}

	emit collisionHandler();
}

// pathfinding algorithim for enemy using graphs and nodes
void Enemy::pathFinding() {
	//Apply Dijkstra's algorithm to find the shortest path to the player
}

void Enemy::animFiller() {
	idleAnim.push_back(QPixmap(":/enemy1/idle_0"));
	idleAnim.push_back(QPixmap(":/enemy1/idle_1"));
	idleAnim.push_back(QPixmap(":/enemy1/idle_2"));
	idleAnim.push_back(QPixmap(":/enemy1/idle_3"));

	attackAnim.push_back(QPixmap(":/enemy1/attack_0"));
	attackAnim.push_back(QPixmap(":/enemy1/attack_1"));
	attackAnim.push_back(QPixmap(":/enemy1/attack_2"));
	attackAnim.push_back(QPixmap(":/enemy1/attack_3"));

	deathAnim.push_back(QPixmap(":/enemy1/death_0"));
	deathAnim.push_back(QPixmap(":/enemy1/death_1"));
	deathAnim.push_back(QPixmap(":/enemy1/death_2"));
	deathAnim.push_back(QPixmap(":/enemy1/death_3"));
	deathAnim.push_back(QPixmap(":/enemy1/death_4"));

	moveAnim.push_back(QPixmap(":/enemy1/walk_0"));
	moveAnim.push_back(QPixmap(":/enemy1/walk_1"));
	moveAnim.push_back(QPixmap(":/enemy1/walk_2"));
	moveAnim.push_back(QPixmap(":/enemy1/walk_3"));
	moveAnim.push_back(QPixmap(":/enemy1/walk_4"));
	moveAnim.push_back(QPixmap(":/enemy1/walk_5"));
}
