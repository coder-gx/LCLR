#pragma once
#ifndef MYQGRAPHICSVIEW_H
#define MYQGRAPHICSVIEW_H

#include <QGraphicsView>

//��ͼ��Ԫ�࣬����ͼ�������ز���
class myQGraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit myQGraphicsView(QWidget* parent = nullptr);
    void wheelEvent(QWheelEvent* event);                  // ��д�������
    void expand();                                        // �Ŵ�
    void shrink();                                        // ��С
    void refresh();                                       // ˢ��

private:
    void zoom(double);                                    // �ߴ�仯

signals:

};

#endif // MYQGRAPHICSVIEW_H