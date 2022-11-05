#ifndef SCENEVIEW_H
#define SCENEVIEW_H

#include <QGraphicsView>

class SceneView : public QGraphicsView
{
     Q_OBJECT
public:
    explicit SceneView(QWidget* parent = nullptr);
signals:
    void movePlayera();
protected:
    void resizeEvent(QResizeEvent* event);
};

#endif // SCENEVIEW_H
