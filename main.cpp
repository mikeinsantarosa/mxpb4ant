#include "mxpb4ant.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    mxpb4ant w;
    w.show();
    return a.exec();
}
