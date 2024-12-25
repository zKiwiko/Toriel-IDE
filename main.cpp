#include "torielwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    TorielWindow w;
    w.show();
    return a.exec();
}
