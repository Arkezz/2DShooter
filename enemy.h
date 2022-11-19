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

public slots:
	void animHandler();
	void attackHandler();
private:
	int health;
	QVector<QPixmap> idleAnim;
	QVector<QPixmap> attackAnim;
	int anim_index;
	int attack_index;
	QTimer* idleTimer;
	QTimer* attackTimer;
};

#endif // ENEMY_H
