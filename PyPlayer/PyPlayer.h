#pragma once

#include <QtWidgets/QWidget>
#include "ui_PyPlayer.h"

class PyPlayer : public QWidget
{
    Q_OBJECT

public:
    PyPlayer(QWidget *parent = nullptr);
    ~PyPlayer();


public slots: // �źŲ�  ����ź�button������Open()
    void Open();

private:
    Ui::PyPlayerClass ui;
};
