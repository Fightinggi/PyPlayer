#pragma once
#include <QOpenGLWidget>
//Q_OBJECT��ʾ���Զ������źŲ۴���
class VideoView:public QOpenGLWidget
{
	Q_OBJECT
public:
	VideoView(QWidget* parent = nullptr);
	~VideoView();
	
	//������ʾ����
	void paintEvent(QPaintEvent *e);

public slots:
	void SetImage(unsigned char* rgb); //�ۺ���
signals:
	void SetImageSig(unsigned char* rgb); //�źź���

protected:
	//�����ʾ��ͼ��
	QImage* img = NULL;
};

