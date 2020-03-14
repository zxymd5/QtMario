#include "MarioComponent.h"
#include<time.h>
#include <QApplication>

int main(int argc, char *argv[])
{
    srand((int)time(0));
    QApplication a(argc, argv);
    MarioComponent w;
    w.show();
    return a.exec();
}
