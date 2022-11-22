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
    enum objectType {heart, bullet, shield };
	Collectibles(objectType);
	objectType getType();

public slots:
	void animHandler();

private:
	objectType barobject;
};

#endif // COLLECTIBLES_H
