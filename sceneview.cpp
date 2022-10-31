#include "sceneview.h"

SceneView::SceneView(QWidget *parent)
    : QGraphicsView{parent}
{
    setMinimumSize(QSize(300, 200));
    setFrameShape(QFrame::NoFrame);

    QPalette p = palette();
    QColor c = p.color(QPalette::Base);
    c.setAlpha(0);
    p.setColor(QPalette::Base, c);
    setPalette(p);
    setBackgroundRole(QPalette::Base);

    setCacheMode(QGraphicsView::CacheBackground);
    setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);

    setOptimizationFlags( QGraphicsView::DontSavePainterState);


}
