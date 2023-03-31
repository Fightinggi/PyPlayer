#include "PyPlayer.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    PyPlayer w;
    w.show();
    return a.exec();
}
