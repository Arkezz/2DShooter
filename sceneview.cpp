#include "sceneview.h"

SceneView::SceneView(QWidget* parent)
	: QGraphicsView{ parent }
{
	//Default settings
	setMinimumSize(QSize(300, 200));
	setStyleSheet("background-color: #000000;");

	QPalette p = palette();
	QColor c = p.color(QPalette::Base);
	c.setAlpha(0);
	p.setColor(QPalette::Base, c);
	setPalette(p);
	setBackgroundRole(QPalette::Base);

	//Optimizations to view
	setCacheMode(QGraphicsView::CacheBackground);
	setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
	setRenderHint(QPainter::SmoothPixmapTransform);

	setOptimizationFlags(QGraphicsView::DontSavePainterState);
}

void SceneView::resizeEvent(QResizeEvent* event)
{
	//Resize keeps scales everything in the scene as well
	fitInView(sceneRect(), Qt::KeepAspectRatio);
	QGraphicsView::resizeEvent(event);
}

void SceneView::keyPressEvent(QKeyEvent* event)
{
	switch (event->key())
	{
		//w a s d
	case Qt::Key_W:
		move(0, -1);
		break;
	case Qt::Key_A:
		move(-1, 0);
		break;
	case Qt::Key_S:
		move(0, 1);
		break;
	case Qt::Key_D:
		move(1, 0);
		break;
	}
}