#ifndef MYCUSTOMSLIDER_H
#define MYCUSTOMSLIDER_H

#include <QSlider>
#include <QLabel>
#include <QMouseEvent>

class MyCustomSlider : public QSlider
{
public:
    MyCustomSlider();
    ~MyCustomSlider();

protected:
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void paintEvent(QPaintEvent *ev);

private:
    QLabel* m_displayLable;
};

#endif // MYCUSTOMSLIDER_H
