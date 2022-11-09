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
