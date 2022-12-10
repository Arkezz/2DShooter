#include "enemy.h"
extern int grid[15][20];
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
	idleTimer->start(400);
}

void Enemy::reset() {
	//Set everything back to default
	{
		idleTimer->stop();
		move_index = 0;
		idle_index = 0;
		attack_index = 0;
		death_index = 0;
		health = 2;
		setPixmap(QPixmap(":/enemy1/idle_0").transformed(QTransform().scale(-1, 1)).scaled(enemyLen, enemyLen));
		idleTimer->start(400);
	}
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

void Enemy::animFiller() {
	//Populate the vectors using for loops
	for (int i = 0; i < 6; i++) {
		moveAnim.push_back(QPixmap(":/enemy1/walk_" + QString::number(i)));
	}

	for (int i = 0; i < 4; i++) {
		attackAnim.push_back(QPixmap(":/enemy1/attack_" + QString::number(i)));
	}

	for (int i = 0; i < 4; i++) {
		idleAnim.push_back(QPixmap(":/enemy1/idle_" + QString::number(i)));
	}

	for (int i = 0; i < 5; i++) {
		deathAnim.push_back(QPixmap(":/enemy1/death_" + QString::number(i)));
	}
}