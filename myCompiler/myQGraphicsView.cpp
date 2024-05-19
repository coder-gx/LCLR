

//@func   : myQGraphicsViewִ�к���
#include "myQGraphicsView.h"
#include <cmath>
#include <QDebug>
#include <qmath.h>
#include <QWheelEvent>
#include <qscrollbar.h>

//@name  : ���캯��
//@para  : 
//@return: 
myQGraphicsView::myQGraphicsView(QWidget* parent) : QGraphicsView(parent)
{
}

//@name  : �������¼�
//@para  : 
//@return: 
void myQGraphicsView::wheelEvent(QWheelEvent* event)
{
	auto step = event->angleDelta().y();
	QPointF cursorPoint = event->pos();
	QPointF scenePos = this->mapToScene(cursorPoint.x(), cursorPoint.y());
	qreal factor = 1 + ((qreal)step / (qreal)viewport()->height()); scale(factor, factor);

	QPointF viewPoint = this->transform().map(scenePos);
	horizontalScrollBar()->setValue(viewPoint.x() - cursorPoint.x());
	verticalScrollBar()->setValue(viewPoint.y() - cursorPoint.y());

}

//@name  : ������С
//@para  : 
//@return: 
void myQGraphicsView::shrink()
{
	zoom(1.0 / 1.2);
}
//@name  : ��������
//@para  : 
//@return: 
void myQGraphicsView::expand()
{
	zoom(1.2);
}
//@name  : ����ı�
//@para  : 
//@return: 
void myQGraphicsView::zoom(double scaleFactor)
{
	//���ź���
	qreal factor = transform().scale(scaleFactor, scaleFactor).mapRect(QRectF(0, 0, 1, 1)).width();
	if (factor < 0.1 || factor > 50)
		return;
	scale(scaleFactor, scaleFactor);
}

//@name  : ˢ�½���
//@para  : 
//@return: 
void myQGraphicsView::refresh()
{
	this->viewport()->update();
}

