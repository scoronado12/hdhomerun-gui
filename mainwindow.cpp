#include <iostream>
#include <QDebug>
#include <curl/curl.h>
#include <string.h>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include "channel.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    ui->setupUi(this);
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
    QString ip_address = ui->ip_addressbox->document()->toPlainText();

    QString url = "http://" + ip_address + "/lineup.json";
    std::string cppUrl = url.toStdString();
    const char *cStyleURL = cppUrl.c_str();
    std::string jsonString;
    QString json;
    CURL *curl_handle = curl_easy_init();
    CURLcode response;
    if(curl_handle){
        /*TODO - replace with global IP address value*/
        curl_easy_setopt(curl_handle, CURLOPT_URL, cStyleURL);
        curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, &jsonString);
        response = curl_easy_perform(curl_handle);

    } else {
        qDebug() << "CurlFailed" << endl;
    }



    QJsonDocument lineup = QJsonDocument::fromJson(json.toUtf8());
    //QJsonObject lineupObj = lineup.object();

    //qDebug() << lineupObj[0].["GuideNumber"].toString();

    QJsonArray lineupArr = lineup.array();
    QVector <Channel> channels;
    for (int i = 0 ; i < lineupArr.count(); i++){
        /*
        qDebug() << lineupArr.at(i)["GuideNumber"].toString();
        qDebug() << lineupArr.at(i)["URL"].toString();*/


        channels.push_front(Channel(lineupArr.at(i)["GuideName"].toString().toStdString(),
                                    lineupArr.at(i)["GuideNumber"].toString().toStdString(),
                                    lineupArr.at(i)["URL"].toString().toStdString()));
    }


    for (Channel c : channels)
        qDebug() << QString::fromStdString(c.getChannelName()) << " " << QString::fromStdString(c.getNumber()) << endl;








    //TODO populate channel list


}
