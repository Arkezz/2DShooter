#ifndef PLAYER_H
#define PLAYER_H

#include <QGraphicsItem>
#include <QKeyEvent>
#include <QVector>
#include <QTimer>
#include "qobject.h"

class Player : public QObject, public QGraphicsPixmapItem
{
	Q_OBJECT
public:
	Player();
	virtual ~Player() {};
	//Direction player is facing
	enum dirF { UP, DOWN, LEFT, RIGHT };
	bool isIdle;
	//Set the direction of the player
	void setDir(dirF dir);

public slots:
	void animateIdle();
protected:
	//Keypressevent handler
	void keyPressEvent(QKeyEvent* event);

private:
	dirF dir;
	QVector<QPixmap> anim[4];
	int anim_index;
	QTimer* idleTimer;
    QTimer* keyPressTimer;
};

#endif // PLAYER_H
