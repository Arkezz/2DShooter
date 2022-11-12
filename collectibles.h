#ifndef COLLECTIBLES_H
#define COLLECTIBLES_H

#include <QObject>
#include <QGraphicsItem>
#include <QGraphicsDropShadowEffect>
#include <QPropertyAnimation>
#include <QProperty>

class Collectibles : public QObject, public QGraphicsPixmapItem
{
	Q_OBJECT
		Q_PROPERTY(QPointF pos READ pos WRITE setPos)
public:
	enum objectType { bullet };
	Collectibles(objectType);
	objectType getType();
    void animHandler();

private:
	objectType barobject;
};

#endif // COLLECTIBLES_H
