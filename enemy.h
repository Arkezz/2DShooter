#ifndef ENEMY_H
#define ENEMY_H
#include <QGraphicsItem>
#include <QTimer>
#include "qobject.h"
class Enemy : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
	Enemy();
	void pathFinding();

public slots:
    void animHandler();
private:
	int health;
	QVector<QPixmap> idleAnim;
	int anim_index;
    QTimer* idleTimer;
};

#endif // ENEMY_H
