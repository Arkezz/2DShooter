#ifndef ENEMY_H
#define ENEMY_H
#include <QGraphicsItem>

class Enemy : public QGraphicsPixmapItem
{
public:
    Enemy();
    void pathFinding();
private:
    int health;
};

#endif // ENEMY_H
