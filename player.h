#ifndef PLAYER_H
#define PLAYER_H

#include <QGraphicsItem>
#include <QKeyEvent>
#include <QVector>
#include <QTimer>
class Player : public QGraphicsPixmapItem
{
public:
	Player();
	enum dirF { UP, DOWN, LEFT, RIGHT };
	//Set the direction of the player
	void setDir(dirF dir);
protected:
	//Keypressevent handler
	void keyPressEvent(QKeyEvent* event);

private:
	dirF dir;
	QVector<QPixmap> anim[4];
	int anim_index;
};

#endif // PLAYER_H
