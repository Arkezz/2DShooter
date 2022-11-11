#include "collectibles.h"

Collectibles::Collectibles(objectType type)
{
	barobject = type;

	//Set the pixmap according to the type
	if (barobject == bullet)
	{
        setPixmap(QPixmap(":/ui/fullHeart"));
	}
	//add shadow effect
	QGraphicsDropShadowEffect* effect = new QGraphicsDropShadowEffect;
	setGraphicsEffect(effect);
	//Idleanim vector
	//idleAnim.push_back(QPixmap(":/collectibles/collectibles_0"));
	//idleAnim.push_back(QPixmap(":/collectibles/collectibles_1"));

	//idleTimer = new QTimer(this);
	//connect(idleTimer, SIGNAL(timeout()), this, SLOT(animHandler()));
	//setPixmap(QPixmap(":/collectibles/collectibles_0"));
	//idleTimer->start(200);
}

Collectibles::objectType Collectibles::getType()
{
	return barobject;
}
