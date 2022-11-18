#ifndef PLAYER_H
#define PLAYER_H

#include <QGraphicsItem>
#include <QKeyEvent>
#include <QVector>
#include <QTimer>
#include <QGraphicsDropShadowEffect>
#include "qobject.h"
#include "soundmanager.h"

class Player : public QObject, public QGraphicsPixmapItem
{
	Q_OBJECT
public:
	Player();
	virtual ~Player() {};
	enum dirF { RIGHT, LEFT, UP, DOWN };
	void setDir(dirF dir);
	dirF getDir();
	void setHealth(int);
	int getHealth();
	void setAmmo(int);
	int getAmmo();
	void shoot();
	void move();

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
	QVector<QPixmap> anim[4];
	QVector<QPixmap> idleAnim[4];
	int anim_index;
	QTimer* idleTimer;
	QTimer* keyPressTimer;
	QTimer* moveTimer;
	soundManager* sounds;
	QGraphicsDropShadowEffect* shadow;
	QVector<QGraphicsDropShadowEffect> trail;
};

#endif // PLAYER_H
