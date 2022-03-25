#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QScrollBar>
#include <QDial>
#include <QVBoxLayout>

#include "mycustomslider.h"
#include "myslider.h"

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = nullptr);
    ~Dialog();

protected:
    QVBoxLayout* mainLayout;

    MyCustomSlider* slider1;
    MyCustomSlider* slider2;
    MyCustomSlider* slider3;

    QScrollBar* scroll1;
    QScrollBar* scroll2;
    QScrollBar* scroll3;

    MySlider* myslider1;
    MySlider* myslider2;
    MySlider* myslider3;

    QDial* dial1;
    QDial* dial2;
    QDial* dial3;

    void sliderTest();
    void myslideTest();
    void scrollTest();
    void dialTest();
};
#endif // DIALOG_H
