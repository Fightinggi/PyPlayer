#include "VideoView.h"
#include <QPainter>

//������Ҫ���Ÿ�python
//��XFFmpeg����õ�rgbͼ�� ���� QT ��VideoView���ڵ�ͼ��img
void VideoView::SetImage(unsigned char* rgb)
{
	if (!rgb) return;

	if (!img)
	{
		//width()  height() ��VideoView�Ĵ��ڿ��
		//���䴰�ڿռ�
		uchar* buf = new uchar[width() * height() * 4];
		img = new QImage(buf, width(), height(), QImage::Format_ARGB32);
	}

	uchar* bits = img->bits();//��ȡ�ڲ��洢�ռ�
	memcpy(bits, rgb, width() * height() * 4);

	update(); //ˢ��  //����update ���� repaint�ػ�������ǿ�ƴ�����

}

void VideoView::paintEvent(QPaintEvent* e)
{
	if (!img || img->isNull()) return;

	QPainter painter;
	painter.begin(this); //��ʼ����
	painter.drawImage(QPoint(0, 0), *img);
	painter.end();

}

VideoView::VideoView(QWidget* parent) :QOpenGLWidget(parent)
{
	//���źŲ�
	QObject::connect(
		this, SIGNAL(SetImageSig(unsigned char*)),
		this, SLOT(SetImage(unsigned char*))
	);
}

VideoView::~VideoView()
{
}
