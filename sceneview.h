#ifndef SCENEVIEW_H
#define SCENEVIEW_H

#include <QGraphicsView>

class SceneView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit SceneView(QWidget *parent = nullptr);

signals:

};

#endif // SCENEVIEW_H
