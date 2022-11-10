#include "enemy.h"
extern int grid[15][20];
const int tileLen = 32;
const int enemyLen = 64;
Enemy::Enemy()
{
	anim_index = 0;
	//add shadow effect
	QGraphicsDropShadowEffect* effect = new QGraphicsDropShadowEffect;
	setGraphicsEffect(effect);
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
	setPixmap(idleAnim[anim_index].transformed(QTransform().scale(-1, 1)).scaled(enemyLen, enemyLen));
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
}