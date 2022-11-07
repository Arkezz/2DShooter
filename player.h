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
	enum dirF { UP, DOWN, LEFT, RIGHT };
	void setDir(dirF dir);
	void shoot();
	void move();

public slots:
	void animateIdle();
protected:
    void keyPressEvent(QKeyEvent*);
    //Always keep focus on the player
    void focusOutEvent(QFocusEvent*) { setFocus(); };

private:
	dirF dir;
	bool isIdle;
	QVector<QPixmap> anim[4];
	QVector<QPixmap> idleAnim[4];
	int anim_index;
	QTimer* idleTimer;
	QTimer* keyPressTimer;
	QTimer* moveTimer;
};

#endif // PLAYER_H
