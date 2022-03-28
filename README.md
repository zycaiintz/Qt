## QSlider在macOS Monterey下的异常



### 0. 写在前面

学习C++三月有余，便接到项目的一个BUG改动需求，受宠若惊

这是一个用Qt设计的桌面平台软件，而我此前并没有接触过Qt。虽然说Qt依旧使用C++开发，但其庞大的体系着实有点令人望代码生畏

得益于面向对象思想和封装的特性，我在解决问题的途中可以忽略许多代码的实现细节，仅需了解其使用方式。不得不敬佩编程语言设计者前辈的智慧及Qt庞大而完善的体系

以下内容是我对解决问题过程的详细复盘，在问题解决后我才意识到这是一个小问题，但在解决问题的途中依旧耗费了不少精力，也学到了不少东西

若你同样是一名Qt新手，本内容对你定有启发



### 1. 异常解析

经反馈文件描述，客户在使用软件时发现，在苹果电脑升级最新系统（macOS Monterey）之后，软件的滑块经常出现问题

- 滑块条不显示或者满格
- 滑块不显示
- 滑块不跟鼠标

经过一番Google之后，发现还是有零星几个网友发现了类似的问题。也许是mac系统实在是小众，相关的资料确实是少

但这些许的资料确实带给我不少启发，其中就有一个描述QSlider在Monterey下的异常现象，让我看清了BUG的真面目

> [QSlider is broken in MacOS Monterey](https://bugreports.qt.io/browse/QTBUG-98093)
>
> [QSlider tickPosition not working on macOS](https://bugreports.qt.io/browse/QTBUG-98903)
> 
> [QSlider fail to repaint on macOS Monterey](https://bugreports.qt.io/browse/QTBUG-96522)

<video src=".\Vedio\slider_bug1.mov"></video>

<center>BUG1-滑块不跟手(.\Vedio\slider_bug1.mov)</center>

<video src=".\Vedio\slider_bug2.mov"></video>

<center>BUG2-滑块之间互相影响(.\Vedio\slider_bug2.mov)</center>

特别是BUG2，让我想到了客户描述的滑块消失或满格，也许正是这个原因造成的，滑块其实被移动到了两侧之外，在我们看来如同消失一般

更令人惊喜的是，Qt在6.2.3版本修复了这个BUG。我立刻汇报组长，表示用新版本的Qt重新编译一下就可以解决，事情还没开始就结束了，欣喜若狂

快乐来得还是太早了，为了适配旧版本的系统，我依旧需要在旧版的Qt上解决这个问题，无奈被迫但此大任



> #### 比较新旧版本的源码（失败方案）
>
> 我最先想到，既然是新系统诞生的问题，那么BUG一定产生在底层代码上，于是我下决定比较新旧版本QSlider的源码，找出其中的差异，问题就可以解决了
>
> 后来想想我这个想法过于天真，Qt源码不好查不说，源码的规模也是庞大到令人发指，浪费2天的精力



### 2. 自定义/重绘滑块

#### 2.1 了解QSlider类

当前首要任务是详细地了解QSlider类，最好的办法其实就是Qt官方的示例程序和API文档，在Qt示例界面搜索slider可以找到有关的示例程序（后来我的测试程序就基于此程序）

<img src=".\Image\help-slider.png" alt="help-slider" title="Slider示例程序" style="zoom:150%;" />

<center>Slider示例程序</center>

对着程序的帮助文档，花费一天的时间理解Qt的三类基础滑块（QSlider、QDial、QScrollBar）以及他们的使用方式

slider是一个完整的经功能封装的模块，其功能实现仅通过创建对象并调用即可，很显然这个BUG出现在某个封装的方法的内部

#### 2.2 值异常 or 显示异常

滑块最重要的功能就是操纵值的改变，滑块和值是对应的。但当前尚不能确定这个异常中滑块的跳动是否伴随着对应值的改变，因此需要对其进行测试

恰好在网上看到有博主分享自定义的滑块，能够在滑动的过程中实时显示滑块对应的值，直接CV大法开启

> [Qt自定义Slider](https://blog.csdn.net/weixin_35719214/article/details/115209151)

首先在项目文件中新建头和源文件，我起名为MyCustomSlider，这将是我用来测试的滑块，继承QSlider

```c++
// mycustomslider.h
#ifdef MYCUSTOMSLIDER_H
#define MYCUSTOMSLIDER_H

#include <QSlider>
#include <QLabel>
#include <QMouseEvent>

class MyCustomSlider : public QSlider {
public:
	MyCustomSlider();
	~MyCustomSlider();
	
protected:
	virtual void mousePressEvent(QMouseEvent *event);
	virtual void mouseReleoseEvent(QMouseEvent *event);
	virtual void mouseMoveEvent(QMouseEvent *event);
	
private:
	QLabel *m_displayLabel;
}
```

方法的实现见链接，将其写入源文件中，而QSS样式对测试并不影响，因此舍弃

模仿Slider示例程序对上方自定义滑块进行测试，在dialog中插入3个水平的自定义滑块

```c++
// dialog.h
#include <QDialog>
#include <QVBoxLayout>
#include "mycustomslider.h"

class Dialog : public QDialog {
public:
	explicit Dialog(QWidget *parent = nullptr);
protected:
	QVBoxLayout* mainLayout;
	MyCustomSlider* slider1;
	MyCustomSlider* slider2;
	MyCustomSlider* slider3;
	void slidrTest();
}
```

在sliderTest中对slider进行初始化，将其封装成函数方便对多个组件进行测试

```c++
// dialog.cpp
#include "dialog.h"

Dialog::Dialog(QWidget *parent) : QDialog(parent) {
	sliderTest();
}

void Dialog::sliderTest() {
	slider1 = new MyCustomSlider();
	slider2 = new MyCustomSlider();
	slider3 = new MyCustomSlider();
	
	slider1->setOrientation(Qt::Horizontal);
	slider2->setOrientation(Qt::Horizontal);
	slider3->setOrientation(Qt::Horizontal);
	slider1->setValue(10);
	slider2->setValue(20);
	slider3->setValue(30);
	
	mainLayout = new QVBoxLayout;
	mainLayout->addWidget(slider1);
	mainLayout->addWidget(slider2);
	mainLayout->addWidget(slider3);
	setLayout(mainLayout);
}
```

这样Dialog中就已经包含了需要测试的一切，只需在main函数中启动dialog即可

```c++
// main.cpp
#include <QApplication>
#include "dialog.h"

int main(int argc, char *argv[]) {
	QApplication a(argc, argv);
	Dialog w; w.show();
	return a.exec();
}
```

启动程序显示窗口，在窗口的滑块上随意移动对其进行测试，发现在滑块突然跳动时，滑块的值也随滑块的位置发生改变，而上面滑块带动下面滑块移动时，其值是不改变的

![sliderTest](.\Image\sliderTest.jpg)
<center>sliderTest显示</center>

到此我们可以大胆猜测一下，滑块跳动应该与mouseEvent读取错误有关，而上面滑块带动下面滑块应该与paintEvent绘制错误有关

#### 2.3 Painter Error ？

我们现在要重新实现QSlider的绘制方法，及paintEvent函数

但在修改之前，我们应该确保QSlider的父类传递过来的接口没有问题，继续查资料，了解QSlider相关的继承关系

资料显示，QSlider继承自QAbstractSlider类，该类是个抽象类，抽象出所有滑块所具有的共同特征。

另外有两个类同样继承了这个类，及QSlider的兄弟类：QDial类和QScrollBar类。而QAbstractSlider类继承自QWidget类

![Inherit](.\Image\Inherit.png)
<center>QSlider类的继承关系</center>

为了检测QAbstractSlider类是否存在问题，我们可以对QSlider的兄弟类，即QDial和QScrollBar进行检测，由这两个类的表现判断其父类是否存在问题

如果父类存在问题，那么我们需要对父类中的方法进行重写，或者直接从QWidget类重新制作slider。如果父类没有问题，那么只需对QSlider中的方法进行重写即可

由于在查早资料的过程中并未发现有关dial和scrollbar的异常帖子，大致可以估计这两个类并不存在问题，但为了保险起见，依旧对这两个类进行测试

测试方法同2.2小结，这里不再进行累述。经测试，这两类并未出现QSlider表现出来的问题，那么解决方案就很清晰了，只需解决paintEvent存在的异常即可

#### 2.4 Qt Style Sheets

解决问题最快的办法还是到网上找相似的代码，我在搜寻各类帖子，尝试寻找有无重写paintEvent来解决这个问题的代码.最终还是被我找到了

> [QSlider in QT misbehaves in new MacOS](https://stackoverflow.com/questions/69890284/qslider-in-qt-misbehaves-in-new-macos-monterey-v12-0-1-any-workaround)

我发现Answer中的代码正好重写了paintEvent函数，于是尝试将其加入mycustomsider中测试

```c++
// MyCustomSlider.cpp
void MyCustomSlider::paintEvent(QPaintEvent *ev) {
    QStylePainter p(this);
    QStyleOptionSlider opt;
    initStyleOption(&opt);

    QRect handle = style()->subControlRect(QStyle::CC_Slider, &opt, QStyle::SC_SliderHandle, this);

	int interval = tickInterval();
	if (interval == 0) interval = pageStep();
	if (tickPosition() != NoTicks) {
		for (int i = minimum(); i <= maximum(); i += interval) {
			int x = std::round(
				(double)(i - this->minimum())                / 
				(double)(this->maximum() - this->minimum())  *
				(double)(this->width() - handle.width())     +
				(double)(handle.width() / 2.0)
			) - 1;
			int h = 4;
			p.setPen(QColor("#a5a294"));
			if (tickPosition() == TicksBothSides || tickPosition() == TicksAbove) {
				int y = this->rect().top();
				p.drawLine(x, y, x, y + h);
			}
			if (tickPosition() == TicksBothSides || tickPosition() == TicksBelow) {
				int y = this->rect().bottom();
				p.drawLine(x, y, x, y - h);
			}
		}
	}
	QSlider::paintEvent(ev);
}
```

但结果发现添加上该paintEvent函数并没有什么作用，甚至没有任何改变

于是我尝试重新创建一个MySlider类，测试这段代码的效果，在复制代码的过程中，我意识到了作者在构造函数中使用了Qt Style Sheet。添加构造函数后滑块果然发生了改变

```c++
// myslider.cpp
MySlider::MySlider(QWidget *parent = nullptr) : QSlider(parent) {
	this->setStyleSheet("																	\
		QSlider::groove:horizontal{															\
			height:8px;																		\
			background:qlineargradient(x1:0,y1:0,x2:0,y2:1,stop:0 #B1B1B1,stop:1 #c4c4c4);	\
			margin:2px 0;																	\
		}																					\
		QSlider::handle:horizontal{															\
			background:qlineargradient(x1:0,y1:0,x2:1,y2:1,stop:0 #b4b4b4,stop:1 #8f8f8f);	\
			border:1px solid #5c5c5c;														\
			width:18px;																		\
			margin:-2px 0;																	\
			border-radius:3px;																\
		}																					\
	");
}
```

![mysliderTest](.\Image\mysliderTest.jpg)
<center>mySliderTest</center>

由图可见，滑块的样式已经发生了改变，与此同时原先QSlider存在的问题也消失了，不过就是滑块前面的条的颜色变灰了，也就是说滑块可以用，但使用体验降低了很多

事实上作者已经在回答中对此情况进行了阐述

> I was able to fix the issue applying a custom stylesheet to the slider.
> However, doing that also creates a problem with the ticks that are not displayed.

现在则需要考虑使用某些手段对滑块的UI进行重构，这样就可以得到既能够使用，又没有外观差别的滑块了

> #### paintEvent的作用
> 
> 我在随后的测试中，尝试去掉作者提供的paintEvent代码，运行发现滑块没有发生明显改变，依旧如上图所示
> 我在paintEvent函数中发现了drawLine的调用，我猜测这段代码的作用是绘制消失的蓝色条，但由于未知的原因这段代码并没有起作用

接下来只需要修改QSS还原macOS下的滑块风格，就大功告成了。同样先尝试在网上找答案，幸运的是果真找到一个类似的

> [Qt中QSlider的样式表设置](https://blog.csdn.net/tax10240809163com/article/details/50899023)

根据比较不难发现

```c++
//还没有滑上去的地方
QSlider::add-page:horizontal{
	background: qlineargradient(spread:pad, x1:0, y1:1, x2:0, y2:0, stop:0 #bcbcbc, stop:0.25 #bcbcbc, stop:0.5 #bcbcbc, stop:1 #bcbcbc); 
}
//已经划过的从地方
QSlider::sub-page:horizontal{
	background: qlineargradient(spread:pad, x1:0, y1:1, x2:0, y2:0, stop:0 #439cf3, stop:0.25 #439cf3, stop:0.5 #439cf3, stop:1 #439cf3);
}
```

这段代码正是我想要的为滑块条添加颜色，但我不清楚作者为何要使用渐变色，却在渐变点采用同样的颜色

打开macOS自带的数码测色计，提取我想要的实现的颜色，并修改上述部分代码

```c++
//还没有滑上去的地方
QSlider::add-page:horizontal{ background: #d9d9d9; }
//已经划过的从地方
QSlider::sub-page:horizontal{ background: #439cf3; }
```

而按钮依靠图片贴上去，因此打开Ai自绘了一个按钮，使用svg格式可以做到无损缩放

<img src="C:\Users\DELL\Desktop\QSlider在macOS下的bug\Image\button.svg" alt="button"/>

<center>button贴图</center>

并且在项目中插入资源，使用`border-image`指向按钮贴图，就实现了效果，不过还有一个小小的问题，就是按钮在滑到两端时，按钮会超出一半

![mySlider](C:\Users\DELL\Desktop\QSlider在macOS下的bug\Image\mySlider.jpg)

为此需要在`QSlider::groove:horizontal`中添加左右限制

```c++
border-right-width: 6px;
border-left-width: 6px;
```

成功解决上述问题，至此一个能在MontereyOS下正常使用并外观一致的滑块就制作完成了

当然以上仅实现了水平滑块的效果，要实现垂直滑块同样的外观，仅需将代码中的**horizontal**替换成**vertical**即可



### 3. 总结回顾

依稀记得早在义务教育阶段就从课本上看到过**黑盒实验**相关的内容，当然我作为物理科班出身，这种思维已经刻进骨子里了

> #### 黑盒实验
> 
> 在一个科学课堂上，老师拿着一个黑盒子，问同学们：“如何不打开黑盒子，知道里面藏的是什么？” 
> 同学们议论纷纷，给出了各种各样的答案：
> - 摇一摇盒子，听声音
> - 用x光扫描看看
> - 敲敲盒子，看看里面有多空
> - 用磁铁吸以下，看看有没有磁性金属

黑盒实验没有标准答案，但提供给我们一种探索未知问题的思维方式

在以上解决问题的过程中，多次用到这个思维，如“判断QSlider的父类是否正常”、“paintEvent函数提供了什么样的作用”、“QSS各键值对是否生效”等

另外在本次解决问题的过程中也学到了许多Qt相关的内容，按专题说明
- QWidget、QDialog和QWindows
- QDial、QScrollBar和QSlider
- paintEvent和QPainter
- QSS基础语法

之后我讲把以上内容整理为专题发布，由于我以一位新人视角看待问题，因此我的内容会更加符合新人看问题的方式
