#pragma once
#include <QOpenGLWidget>
//Q_OBJECT表示会自动生成信号槽代码
class VideoView:public QOpenGLWidget
{
	Q_OBJECT
public:
	VideoView(QWidget* parent = nullptr);
	~VideoView();
	
	//重载显示函数
	void paintEvent(QPaintEvent *e);

public slots:
	void SetImage(unsigned char* rgb); //槽函数
signals:
	void SetImageSig(unsigned char* rgb); //信号函数

protected:
	//存放显示的图像
	QImage* img = NULL;
};

