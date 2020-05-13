#include <iostream>
#include <stdlib.h>
#include <QDebug>
#include <curl/curl.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QTableWidget>
#include "channel.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    ui->channelTable->verticalHeader()->hide();
    ui->channelTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->channelTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

MainWindow::~MainWindow()
{
    delete ui;
}

size_t MainWindow::WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}



/**
 * @brief MainWindow::on_connect_button_clicked
 * This function makes an HTTP GET request using curl and populates the table with JSON data
 */

void MainWindow::on_connect_button_clicked()
{
    qDebug() << "Clicked button\n";

    for (int i = ui->channelTable->rowCount(); i >= 0; i--){
        ui->channelTable->removeRow(i);
    }
    QString ip_address = ui->ip_addressbox->document()->toPlainText();

    QString url = "http://" + ip_address + "/lineup.json";
    qDebug() << "URL: " << url << endl;
    std::string cppUrl = url.toStdString();
    const char *cStyleURL = cppUrl.c_str();
    std::string jsonString;
    QString json;
    CURL *curl_handle = curl_easy_init();
    CURLcode response;

    if(curl_handle){
        /*TODO - replace with global IP address value*/
        qDebug() << cStyleURL << endl;
        curl_easy_setopt(curl_handle, CURLOPT_URL, cStyleURL);
        curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, &jsonString);
        response = curl_easy_perform(curl_handle);

    } else {
        qDebug() << "CurlFailed" << endl;
    }

    json = QString::fromStdString(jsonString);
    QJsonDocument lineup = QJsonDocument::fromJson(json.toUtf8());


    QJsonArray lineupArr = lineup.array();
    QVector <Channel> channels;
    for (int i = 0 ; i < lineupArr.count(); i++){
        /*
        qDebug() << lineupArr.at(i)["GuideName"].toString() << " " <<
                lineupArr.at(i)["GuideNumber"].toString()<< " " <<
                lineupArr.at(i)["URL"].toString() << endl;
        */
        channels.push_back(Channel(lineupArr.at(i)["GuideName"].toString().toStdString(),
                                    atof(lineupArr.at(i)["GuideNumber"].toString().toStdString().c_str()),
                                    lineupArr.at(i)["URL"].toString().toStdString()));
    }


    qDebug() << "Channel Name " << "Channel Number " << "URL " << endl;

    for (Channel c : channels){
        qDebug() << QString::fromStdString(c.getChannelName()) << " "
                 << c.getNumber() << " "
                 << QString::fromStdString(c.getURL()) << endl;
    }

    /* Insert Each Member of Channel into Table View */
    for (int i = 0 ; i < channels.size(); i++){
        ui->channelTable->insertRow(ui->channelTable->rowCount());

        ui->channelTable->setItem(ui->channelTable->rowCount() -1, NUMBER,
                                  new QTableWidgetItem(QString::number(channels.at(i).getNumber())));

        ui->channelTable->setItem(ui->channelTable->rowCount()-1, NAME,
                                 new QTableWidgetItem(QString::fromStdString(channels.at(i).getChannelName())));

        ui->channelTable->setItem(ui->channelTable->rowCount()-1, URL,
                                 new QTableWidgetItem(QString::fromStdString(channels.at(i).getURL())));
    }

    qDebug() << "Found " << channels.size() << " channels." << endl;



}

void MainWindow::on_launchButton_clicked()
{
    qDebug() << "Clicked Launch Button" << endl;
    if (ui->channelTable->rowCount() == 0)
        return void();

    QList<QTableWidgetItem *> qi = ui->channelTable->selectedItems();
    std::string selectedUrl = qi.at(URL)->text().toStdString();


    int fd = fork();
    qDebug() << fd << endl;
    if (fd < 0){
        qDebug() << "Fork Failed!\n";
        return void();
    }

    if (fd == 0){
        char *mpvArgs[] = {(char *)"mpv", (char *)selectedUrl.c_str(), NULL};
        int rc = execvp(mpvArgs[0], mpvArgs);
        if (rc < 0){
            qDebug() << "Exec Failed! Is MPV not installed?" << endl;
        }

    }

    selectedUrl.clear();

    wait(NULL);

}
