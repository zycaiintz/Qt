#ifndef MYSLIDER_H
#define MYSLIDER_H

#include <QSlider>

class MySlider : public QSlider
{
public:
    explicit MySlider(Qt::Orientation orientation, QWidget *parent = nullptr) : QSlider(orientation, parent) { } ;
    explicit MySlider(QWidget *parent = nullptr) : QSlider(parent) {
        this->setStyleSheet("                                                                           \
            QSlider::groove:horizontal {                                                                \
                height: 1px;                                                                            \
                border: 1px solid #999999;                                                              \
                margin: 0px;                                                                            \
            }                                                                                           \
            QSlider::handle:horizontal {                                                                \
                border: 0px;                                                                            \
                border-image: url(:/image/button.svg);                                                  \
                width: 15px;                                                                            \
                margin: -7px -7px -7px -7px;                                                            \
            }                                                                                           \
            QSlider::add-page:horizontal{ background: #d9d9d9; }                                        \
            QSlider::sub-page:horizontal{ background: #439cf3; }                                        \
        ");
    };

//    explicit MySlider(QWidget *parent = nullptr) : QSlider(parent) {
//        this->setStyleSheet("                                                                           \
//              QSlider::groove:horizontal {                                                              \
//                  height: 8px;                                                                          \
//                  background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #B1B1B1, stop:1 #c4c4c4);  \
//                  margin: 2px 0;                                                                        \
//              }                                                                                         \
//              QSlider::handle:horizontal {                                                              \
//                  background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #b4b4b4, stop:1 #8f8f8f);  \
//                  border: 1px solid #5c5c5c;                                                            \
//                  width: 18px;                                                                          \
//                  margin: -2px 0;                                                                       \
//                  border-radius: 3px;                                                                   \
//              }                                                                                         \
//        ");
//    };

protected:
//    void paintEvent(QPaintEvent* e);
};

#endif // MYSLIDER_H
