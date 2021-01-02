#include <QApplication>
#include <QDebug>

#include "headers/mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setFixedSize(w.size());
    w.show();
    qDebug() << "Starting Application\n";
    return a.exec();
}
