#include <QStylePainter>
#include <QStyleOptionSlider>
#include <QStyleOptionComplex>
#include <QColor>

#include "myslider.h"

//void MySlider::paintEvent(QPaintEvent *ev)
//{
//    QStylePainter stylePainter(this);
//    QStyleOptionSlider optionSlider;
//    initStyleOption(&optionSlider);

//    QRect handle = style()->subControlRect(QStyle::CC_Slider, &optionSlider, QStyle::SC_SliderHandle, this);

//    int interval = tickInterval();
//    if(interval == 0) interval = pageStep();

//    if(tickInterval() != NoTicks)
//    {
//        for(int i = minimum(); i <= maximum(); i += interval)
//        {
//            int x = std::round( (double)(i - this->minimum())                  /
//                                (double)(this->maximum() - this->minimum())    *
//                                (double)(this->width() - handle.width())       +
//                                (double)(handle.width() / 2.0)
//                              ) - 1;
//            int h = 4;
//            stylePainter.setPen(QColor("#87cefa"));

//            if(tickPosition() == TicksBothSides || tickPosition() == TicksAbove)
//            {
//                int y = this->rect().top();
//                stylePainter.drawLine(x, y, x, y + h);
//            }

//            if(tickPosition() == TicksBothSides || tickPosition() == TicksBelow)
//            {
//                int y = this->rect().bottom();
//                stylePainter.drawLine(x, y, x, y - h);
//            }
//        }
//    }

//    QSlider::paintEvent(ev);
//}
