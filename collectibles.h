#ifndef COLLECTIBLES_H
#define COLLECTIBLES_H

#include <QObject>
#include <QGraphicsItem>
#include <QGraphicsDropShadowEffect>

class Collectibles : public QObject, public QGraphicsPixmapItem
{
	Q_OBJECT
public:
    enum objectType { bullet };
	Collectibles(objectType);
	objectType getType();

private:
	objectType barobject;
};

#endif // COLLECTIBLES_H
