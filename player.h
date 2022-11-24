#ifndef PLAYER_H
#define PLAYER_H

#include <QGraphicsItem>
#include <QKeyEvent>
#include <QVector>
#include <QTimer>
#include <QGraphicsDropShadowEffect>
#include <QRandomGenerator>
#include "qobject.h"
#include "soundmanager.h"
#include "enemy.h"
#include "collectibles.h"

class Player : public QObject, public QGraphicsPixmapItem
{
	Q_OBJECT
public:
	Player();
	virtual ~Player() {};
	enum dirF { RIGHT, LEFT, UP, DOWN };
	enum state { normal, invincible };

	void setDir(dirF dir) { this->dir = dir; }
	dirF getDir() { return dir; }
	void setHealth(int health) { this->health = health; }
	int getHealth() { return health; }
	void setAmmo(int a) { ammo = a; }
	int getAmmo() { return ammo; }
	void setStatus(state s) { status = s; }
	state getStatus() { return status; }

	void shoot();
	void move();
	void animFiller();
	void pickUp();

signals:
	void drawUi();
	void openSettings();
	void collisionHandler();
	void drawFootsteps();

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
	state status;
	QVector<QPixmap> anim[4];
	QVector<QPixmap> idleAnim[4];
	QVector<QPixmap> pickupAnim;
	int anim_index = 2;
	int idle_index = 0;
	int pickup_index = 0;
	QTimer* idleTimer;
	QTimer* keyPressTimer;
	QTimer* moveTimer;
	QTimer* shieldtime;
	soundManager* sounds;
	QGraphicsDropShadowEffect* shadow;
	QVector<QGraphicsDropShadowEffect> trail;
};

#endif // PLAYER_H
