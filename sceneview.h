#ifndef SCENEVIEW_H
#define SCENEVIEW_H

#include <QGraphicsView>
#include <QKeyEvent>

class SceneView : public QGraphicsView
{
	Q_OBJECT
public:
	explicit SceneView(QWidget* parent = nullptr);
    void keyPressEvent(QKeyEvent* event);
signals:

protected:
	void resizeEvent(QResizeEvent* event);
};

#endif // SCENEVIEW_H
