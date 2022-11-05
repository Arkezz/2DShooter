#ifndef PLAYER_H
#define PLAYER_H

#include <QGraphicsItem>
#include <QKeyEvent>
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
};

#endif // PLAYER_H
