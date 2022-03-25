#include "dialog.h"

Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
{
//    sliderTest();
//    mydrawTest();
//    scrollTest();
//    dialTest();
    myslideTest();
}

Dialog::~Dialog()
{
    // Do noting ...
}

void Dialog::sliderTest() {
    mainLayout = new QVBoxLayout;

    slider1 = new MyCustomSlider();
    slider2 = new MyCustomSlider();
    slider3 = new MyCustomSlider();

    slider1->setValue(10);
    slider2->setValue(20);
    slider3->setValue(30);

    mainLayout->addWidget(slider1);
    mainLayout->addWidget(slider2);
    mainLayout->addWidget(slider3);

    setLayout(mainLayout);
}

void Dialog::myslideTest(){
    mainLayout = new QVBoxLayout;

    myslider1 = new MySlider();
    myslider2 = new MySlider();
    myslider3 = new MySlider();

    myslider1->setOrientation(Qt::Horizontal);
    myslider2->setOrientation(Qt::Horizontal);
    myslider3->setOrientation(Qt::Horizontal);

    myslider1->setValue(10);
    myslider2->setValue(20);
    myslider3->setValue(30);

    //connect(scroll1, &QScrollBar::valueChanged, [&](){this->update();});

    mainLayout->addWidget(myslider1);
    mainLayout->addWidget(myslider2);
    mainLayout->addWidget(myslider3);

    setLayout(mainLayout);
}

void Dialog::scrollTest() {
    mainLayout = new QVBoxLayout;

    scroll1 = new QScrollBar();
    scroll2 = new QScrollBar();
    scroll3 = new QScrollBar();

    scroll1->setValue(10);
    scroll2->setValue(20);
    scroll3->setValue(30);

    connect(scroll1, &QScrollBar::valueChanged, [&](){this->update();});

    mainLayout->addWidget(scroll1);
    mainLayout->addWidget(scroll2);
    mainLayout->addWidget(scroll3);

    setLayout(mainLayout);
}

void Dialog::dialTest() {
    mainLayout = new QVBoxLayout;

    dial1 = new QDial;
    dial2 = new QDial;
    dial3 = new QDial;

    dial1->setValue(10);
    dial1->setValue(20);
    dial3->setValue(30);

    connect(dial1, &QDial::valueChanged, [&](){this->update();});

    mainLayout->addWidget(dial1);
    mainLayout->addWidget(dial2);
    mainLayout->addWidget(dial3);

    setLayout(mainLayout);
}
