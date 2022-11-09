#include "enemy.h"
extern int grid[15][20];

Enemy::Enemy()
{
	anim_index = 0;
	//Idleanim vector
	idleAnim.push_back(QPixmap(":/enemy1/idle_0"));
	idleAnim.push_back(QPixmap(":/enemy1/idle_1"));
	idleAnim.push_back(QPixmap(":/enemy1/idle_2"));
	idleAnim.push_back(QPixmap(":/enemy1/idle_3"));

	attackAnim.push_back(QPixmap(":/enemy1/attack_0"));
	attackAnim.push_back(QPixmap(":/enemy1/attack_1"));
	attackAnim.push_back(QPixmap(":/enemy1/attack_2"));
	attackAnim.push_back(QPixmap(":/enemy1/attack_3"));

	idleTimer = new QTimer(this);
	connect(idleTimer, SIGNAL(timeout()), this, SLOT(animHandler()));
	setPixmap(QPixmap(":/enemy1/idle_0"));
	attackTimer = new QTimer(this);
	connect(attackTimer, SIGNAL(timeout()), this, SLOT(attackHandler()));
	idleTimer->start(200);
}

//animhandler
void Enemy::animHandler()
{
	if (anim_index == 3)
	{
		anim_index = 0;
	}
	else
	{
		anim_index++;
	}
	setPixmap(idleAnim[anim_index].transformed(QTransform().scale(-1, 1)).scaled(64, 64));
}

//attackHandler
void Enemy::attackHandler()
{
	//Start the timer for the attack animation if anim index is 0
	if (anim_index == 0)
	{
		attackTimer->start(200);
	}
	//If anim index is 3, stop the timer
	if (anim_index == 3)
	{
		attackTimer->stop();
		anim_index = 0;
	}
	else {
		anim_index++;
	}
	setPixmap(attackAnim[anim_index].transformed(QTransform().scale(-1, 1)).scaled(64, 64));
}

// pathfinding algorithim for enemy using graphs and nodes
void Enemy::pathFinding() {
}