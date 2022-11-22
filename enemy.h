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
    void loseHealth();
    int getHealth();
    void animFiller();

public slots:
	void animHandler();
	void attackHandler();
    void deathHandler();
private:
	int health;
	QVector<QPixmap> idleAnim;
	QVector<QPixmap> attackAnim;
    QVector<QPixmap> deathAnim;
	int anim_index;
	int attack_index;
    int death_index;
	QTimer* idleTimer;
	QTimer* attackTimer;
};

#endif // ENEMY_H
