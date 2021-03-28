#include <QApplication>
#include <QDebug>

#include "qt/mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setFixedSize(w.size());
    w.show();
    return a.exec();
}
