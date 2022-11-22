#include "enemy.h"
extern int grid[15][20];
const int tileLen = 32;
const int enemyLen = 64;
Enemy::Enemy()
{
	//Make the enmy focusable
	setFlag(QGraphicsItem::ItemIsFocusable);
	animFiller();
	anim_index = 0;
	attack_index = 0;
	death_index = 0;
	health = 2;
	QGraphicsDropShadowEffect* effect = new QGraphicsDropShadowEffect;
	setGraphicsEffect(effect);

	setPixmap(QPixmap(":/enemy1/idle_0"));
	idleTimer = new QTimer(this);
	attackTimer = new QTimer(this);
	connect(idleTimer, SIGNAL(timeout()), this, SLOT(animHandler()));
	connect(attackTimer, SIGNAL(timeout()), this, SLOT(attackHandler()));
	idleTimer->start(200);

	//If the enmy is pressed on the screen, it will attack
	connect(this, SIGNAL(pressed()), this, SLOT(attack()));
}

void Enemy::loseHealth() {
	this->health = 0;
}

int Enemy::getHealth() {
	return this->health;
}

//animhandler
void Enemy::animHandler()
{
	if (attack_index == 3)
	{
		attack_index = 0;
	}
	else
	{
		attack_index++;
	}
	setPixmap(idleAnim[attack_index].transformed(QTransform().scale(-1, 1)).scaled(enemyLen, enemyLen));

	idleTimer->stop();
}

void Enemy::deathHandler() {
    death_index++;
    if (death_index >= deathAnim.size())
        return;
    setPixmap(deathAnim[death_index].transformed(QTransform().scale(-1, 1)).scaled(enemyLen, enemyLen));
}

//attackHandler
void Enemy::attackHandler()
{
	while (anim_index != 3) {
		anim_index++;
	}
	//According to the direction of the enemy, the attack animation will be played
	setPixmap(attackAnim[anim_index].transformed(QTransform().scale(-1, 1)).scaled(enemyLen, enemyLen));
	anim_index = 0;
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
}
