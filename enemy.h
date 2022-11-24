#ifndef ENEMY_H
#define ENEMY_H
#include <QGraphicsItem>
#include <QTimer>
#include <QGraphicsDropShadowEffect>
#include "qobject.h"
class Enemy : public QObject, public QGraphicsPixmapItem
{
	Q_OBJECT
public:
	Enemy();
	void pathFinding();
	void loseHealth() { health -= 1; }
	void setHealth(int h) { health = h; }
	int getHealth() { return health; }
	void animFiller();

signals:
	void collisionHandler();

public slots:
	void animHandler();
	void attackHandler();
	void deathHandler();
	void move();
private:
	int health;
	QVector<QPixmap> moveAnim;
	QVector<QPixmap> idleAnim;
	QVector<QPixmap> attackAnim;
	QVector<QPixmap> deathAnim;
	int move_index;
	int idle_index;
	int attack_index;
	int death_index;
	QTimer* idleTimer;
	QTimer* attackTimer;
	QTimer* moveTimer;
};

#endif // ENEMY_H
