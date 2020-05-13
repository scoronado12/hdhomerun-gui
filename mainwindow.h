#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <iostream>
#include <qdebug.h>
#include <curl/curl.h>
#include <string.h>
#include <QMainWindow>
#include <QTableWidget>
#include <QPushButton>
#include "qtextedit.h"

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

private slots:
    void on_connect_button_clicked();

private:
    Ui::MainWindow *ui;
    QTextEdit ip_addressbox;
    QTableWidget channelTable;
    QPushButton launchButton;

    enum channelColumns{ NUMBER, NAME, URL};


};
#endif // MAINWINDOW_H
