#include <QPalette>
#include <QStylePainter>
#include <QStyleOptionSlider>

#include "mycustomslider.h"

MyCustomSlider::MyCustomSlider()
{
    this->setOrientation(Qt::Horizontal);

    m_displayLable = new QLabel(this);
    m_displayLable->setFixedSize(QSize(20, 20));
    m_displayLable->setAlignment(Qt::AlignCenter);
    m_displayLable->setVisible(true);
    m_displayLable->move(0, 3);
}

MyCustomSlider::~MyCustomSlider()
{
    // Do nothing ...
}

void MyCustomSlider::mousePressEvent(QMouseEvent *event)
{
    QSlider::mousePressEvent(event);
}

void MyCustomSlider::mouseReleaseEvent(QMouseEvent *event)
{
    QSlider::mouseReleaseEvent(event);
}

void MyCustomSlider::mouseMoveEvent(QMouseEvent *event)
{
    m_displayLable->setText(QString::number(this->value()));
    m_displayLable->move( (this->width() - m_displayLable->width()) * this->value() / (this->maximum() - this->minimum()), 3);
    QSlider::mouseMoveEvent(event);
}

void MyCustomSlider::paintEvent(QPaintEvent *ev)
{
    QStylePainter stylePainter(this);

    QStyleOptionSlider optionSlider;
    initStyleOption(&optionSlider);

    QRect handle = style()->subControlRect(QStyle::CC_Slider, &optionSlider, QStyle::SC_SliderHandle, this);

    int interval = tickInterval();
    if(interval == 0)
    {
        interval = pageStep();
    }

    if(tickInterval() != NoTicks)
    {
        for(int i = minimum(); i <= maximum(); i += interval)
        {
            int x = std::round( (double)(i - this->minimum())                  /
                                (double)(this->maximum() - this->minimum())    *
                                (double)(this->width() - handle.width())       +
                                (double)(handle.width() / 2.0)
                              ) - 1;
            int h = 4;
            stylePainter.setPen(QColor("#a5a294"));

            if(tickPosition() == TicksBothSides || tickPosition() == TicksAbove)
            {
                int y = this->rect().top();
                stylePainter.drawLine(x, y, x, y + h);
            }

            if(tickPosition() == TicksBothSides || tickPosition() == TicksBelow)
            {
                int y = this->rect().bottom();
                stylePainter.drawLine(x, y, x, y - h);
            }
        }
    }

    QSlider::paintEvent(ev);
}
