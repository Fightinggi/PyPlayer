#include "VideoView.h"
#include <QPainter>

//函数需要开放给python
//把XFFmpeg解码好的rgb图像 传给 QT 的VideoView窗口的图像img
void VideoView::SetImage(unsigned char* rgb)
{
	if (!rgb) return;

	if (!img)
	{
		//width()  height() 是VideoView的窗口宽度
		//分配窗口空间
		uchar* buf = new uchar[width() * height() * 4];
		img = new QImage(buf, width(), height(), QImage::Format_ARGB32);
	}

	uchar* bits = img->bits();//获取内部存储空间
	memcpy(bits, rgb, width() * height() * 4);

	update(); //刷新  //调用update 或者 repaint重画，可以强制触发；

}

void VideoView::paintEvent(QPaintEvent* e)
{
	if (!img || img->isNull()) return;

	QPainter painter;
	painter.begin(this); //开始绘制
	painter.drawImage(QPoint(0, 0), *img);
	painter.end();

}

VideoView::VideoView(QWidget* parent) :QOpenGLWidget(parent)
{
	//绑定信号槽
	QObject::connect(
		this, SIGNAL(SetImageSig(unsigned char*)),
		this, SLOT(SetImage(unsigned char*))
	);
}

VideoView::~VideoView()
{
}
