#ifndef PLAYER_H
#define PLAYER_H

#include <QGraphicsItem>
#include <QKeyEvent>
#include <QVector>
#include <QTimer>
#include "qobject.h"
#include "soundmanager.h"
#include "enemy.h"

class Player : public QObject, public QGraphicsPixmapItem
{
	Q_OBJECT
public:
	Player();
	virtual ~Player() {};
	enum dirF { UP, DOWN, LEFT, RIGHT };
	void setDir(dirF dir);
	int getHealth();
	void setHealth(int);
	void shoot();
	void move();
	void collisionHandler();
	bool isWall();
	void createGraph();

signals:
	void drawUi();
	void openSettings();

public slots:
	void animHandler();
protected:
	void keyPressEvent(QKeyEvent*);
	//Always keep focus on the player
	void focusOutEvent(QFocusEvent*) { setFocus(); };

private:
	dirF dir;
	bool isIdle;
	int health;
	int ammo;
	QVector<QPixmap> anim[4];
	QVector<QPixmap> idleAnim[4];
	int anim_index;
	QTimer* idleTimer;
	QTimer* keyPressTimer;
	QTimer* moveTimer;
	soundManager* sounds;
};

#endif // PLAYER_H
