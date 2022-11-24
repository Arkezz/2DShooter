#include "collectibles.h"

Collectibles::Collectibles(objectType type)
{
	barobject = type;
	//add shadow effect
	QGraphicsDropShadowEffect* effect = new QGraphicsDropShadowEffect;
	setGraphicsEffect(effect);

	//Set the pixmap according to the type
    if (barobject == heart)
       {
           setPixmap(QPixmap(":/ui/fullHeart"));
       }
       if (barobject == bullet)
       {
           setPixmap(QPixmap(":/ui/bullet"));
       }
       if (barobject == shield)
       {
           setPixmap(QPixmap(":/ui/shield"));
       }
       if(barobject == exit){
           setPixmap(QPixmap(":/ui/exit"));
       }
}

void Collectibles::animHandler() {
	QPropertyAnimation* animation = new QPropertyAnimation(this, "pos");
	animation->setDuration(1500);
	animation->setStartValue(QPointF(x(), y() + 5));
	animation->setEndValue(QPointF(x(), y() - 5));
	animation->setLoopCount(1);

	//Second animation takes it back to the original position
	QPropertyAnimation* animation2 = new QPropertyAnimation(this, "pos");
	animation2->setDuration(1500);
	animation2->setStartValue(QPointF(x(), y() - 5));
	animation2->setEasingCurve(QEasingCurve::InOutQuad);
	animation2->setEndValue(QPointF(x(), y() + 5));
	animation2->setLoopCount(1);

	QSequentialAnimationGroup* group = new QSequentialAnimationGroup;
	group->addAnimation(animation);
	group->addAnimation(animation2);
	group->setLoopCount(-1);
	group->start();
}
