#include "collectibles.h"

Collectibles::Collectibles(objectType type)
{
	barobject = type;
	//add shadow effect
	QGraphicsDropShadowEffect* effect = new QGraphicsDropShadowEffect;
	setGraphicsEffect(effect);

	//Set the pixmap according to the type
	if (barobject == bullet)
	{
		setPixmap(QPixmap(":/ui/fullHeart"));
	}
}

Collectibles::objectType Collectibles::getType()
{
	return barobject;
}

void Collectibles::animHandler(){
    QPropertyAnimation* animation = new QPropertyAnimation(this, "pos");
    animation->setDuration(1000);
    animation->setStartValue(QPointF(x(), y() + 5));
    animation->setEndValue(QPointF(x(), y() - 5));
    animation->setLoopCount(-1);
    animation->start();
}
