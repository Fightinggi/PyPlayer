#pragma once

#include <QtWidgets/QWidget>
#include "ui_PyPlayer.h"

class PyPlayer : public QWidget
{
    Q_OBJECT

public:
    PyPlayer(QWidget *parent = nullptr);
    ~PyPlayer();


public slots: // 信号槽  点击信号button，调用Open()
    void Open();

private:
    Ui::PyPlayerClass ui;
};
