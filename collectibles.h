#ifndef COLLECTIBLES_H
#define COLLECTIBLES_H

#include <QObject>
#include <QGraphicsItem>
#include <QGraphicsDropShadowEffect>
#include <QPropertyAnimation>
#include <QSequentialAnimationGroup>
#include <QProperty>

class Collectibles : public QObject, public QGraphicsPixmapItem
{
	Q_OBJECT
		Q_PROPERTY(QPointF pos READ pos WRITE setPos)
public:
	enum objectType { heart, bullet, shield, exit };
	Collectibles(objectType);
	objectType getType() { return barobject; }

    //Property animations for the collectible
    QPropertyAnimation* animation;
    QPropertyAnimation* animation2;

    //Animation group to play animations sequentially
    QSequentialAnimationGroup* group;

public slots:
	void animHandler();

private:
    //Type of the collectible
	objectType barobject;


};

#endif // COLLECTIBLES_H
