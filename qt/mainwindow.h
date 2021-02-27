#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <iostream>
#include <stdlib.h>
#include <QDebug>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QTableWidget>
#include <QMainWindow>

#include "channel.h"
#include "hdhomerun_wrapper.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp);
    QString targetURL;

private slots:

    void on_connect_button_clicked();
    HDHomeRun_Wrapper auto_connect();
    void on_launchButton_clicked();


private:
    Ui::MainWindow *ui;
    QTableWidget channelTable;

    enum channelColumns{ NUMBER, NAME, URL};


};
#endif // MAINWINDOW_H
