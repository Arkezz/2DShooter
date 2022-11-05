#include "enemy.h"
extern int grid[15][20];

Enemy::Enemy()
{
	setPixmap(QPixmap(":/entities/player"));
}

// pathfinding algorithim for enemy using graphs and nodes
void Enemy::pathFinding() {
}