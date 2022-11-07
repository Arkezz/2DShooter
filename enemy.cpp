#include "enemy.h"
extern int grid[15][20];

Enemy::Enemy()
{
    anim_index =2;
	//Idleanim vector
	idleAnim.push_back(QPixmap(":/enemy1/idle_0"));
	idleAnim.push_back(QPixmap(":/enemy1/idle_1"));
	idleAnim.push_back(QPixmap(":/enemy1/idle_2"));
	idleAnim.push_back(QPixmap(":/enemy1/idle_3"));

	idleTimer = new QTimer(this);
	connect(idleTimer, SIGNAL(timeout()), this, SLOT(animHandler()));
    setPixmap(QPixmap(":/enemy1/idle_0"));
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

// pathfinding algorithim for enemy using graphs and nodes
void Enemy::pathFinding() {
}
