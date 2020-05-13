#include <iostream>
#include <QDebug>
#include <curl/curl.h>
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



void MainWindow::on_connect_button_clicked()
{
    //USE libcurl to download the file
    qDebug() << "Clicked button\n";
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

    //std::cout << jsonString << "\n";

    json = QString::fromStdString(jsonString);
    //todo looking here for conversion
    QJsonDocument lineup = QJsonDocument::fromJson(json.toUtf8());

    //qDebug() << lineup. << endl;

    QJsonArray lineupArr = lineup.array();
    qDebug() << lineupArr.at(0).toString() << endl;
    QVector <Channel> channels;
    for (int i = 0 ; i < lineupArr.count(); i++){

        qDebug() << lineupArr.at(i)["GuideName"].toString() << " " <<
                lineupArr.at(i)["GuideNumber"].toString()<< " " <<
                lineupArr.at(i)["URL"].toString() << endl;


        channels.push_front(Channel(lineupArr.at(i)["GuideName"].toString().toStdString(),
                                    lineupArr.at(i)["GuideNumber"].toString().toStdString(),
                                    lineupArr.at(i)["URL"].toString().toStdString()));
    }

    qDebug() << " ----" << endl;
    qDebug() << "Channel Name " << "Channel Number " << "URL " << endl;

    for (Channel c : channels){
        qDebug() << QString::fromStdString(c.getChannelName()) << " "
                 << QString::fromStdString(c.getNumber()) << " "
                 << QString::fromStdString(c.getURL()) << endl;
        //channelTable .insertRow()
    }

    for (int i = 0 ; i < channels.size(); i++){
        ui->channelTable->insertRow(ui->channelTable->rowCount());
        ui->channelTable->setItem(ui->channelTable->rowCount() -1, NUMBER,
                                  new QTableWidgetItem(QString::fromStdString(channels.at(i).getNumber())));
        ui->channelTable->setItem(ui->channelTable->rowCount()-1, NAME,
                                 new QTableWidgetItem(QString::fromStdString(channels.at(i).getChannelName())));
        ui->channelTable->setItem(ui->channelTable->rowCount()-1, URL,
                                 new QTableWidgetItem(QString::fromStdString(channels.at(i).getURL())));
    }







    //TODO populate channel list


}
