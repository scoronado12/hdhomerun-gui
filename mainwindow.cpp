
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
 * This function makes an attempt to connect with one of the Tuners on the network.
 *
 * @returns 0 if success
 * @returns 1 if not found
 */

int MainWindow::auto_connect()
{


    struct hdhomerun_discover_device_t foundDevices[5] = {};

    //int numDevices = hdhomerun_discover_find_devices_custom_v2();
    int nDevices = hdhomerun_discover_find_devices_custom_v2(0, HDHOMERUN_DEVICE_TYPE_TUNER, HDHOMERUN_DEVICE_ID_WILDCARD, foundDevices, 10);

    if (nDevices <= 0){
        qDebug() << "Device Not Found" << Qt::endl;
        return 1;
    }

    hdhomerun_discover_device_t device = foundDevices[0];

    targetURL = (QString) device.base_url;
    qDebug() << "URL" << device.base_url << Qt::endl;




    return 0;
}



/**
 * @brief MainWindow::on_connect_button_clicked
 * This function makes an HTTP GET request using curl and populates the table with JSON data
 */

void MainWindow::on_connect_button_clicked()
{
    qDebug() << "Clicked button\n";
    auto_connect();


    for (int i = ui->channelTable->rowCount(); i >= 0; i--){
        ui->channelTable->removeRow(i);
    }
    QString url = targetURL+ "/lineup.json";
    qDebug() << "URL: " << url << Qt::endl;
    std::string cppUrl = url.toStdString();
    const char *cStyleURL = cppUrl.c_str();
    std::string jsonString;
    QString json;
    CURL *curl_handle = curl_easy_init();
    CURLcode response;

    if(curl_handle){
        qDebug() << cStyleURL << Qt::endl;
        curl_easy_setopt(curl_handle, CURLOPT_URL, cStyleURL);
        curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, &jsonString);
        response = curl_easy_perform(curl_handle);

    } else {
        qDebug() << "Curl Failed" << Qt::endl;
    }

    json = QString::fromStdString(jsonString);
    QJsonDocument lineup = QJsonDocument::fromJson(json.toUtf8());


    QJsonArray lineupArr = lineup.array();
    QVector <Channel> channels;
    for (int i = 0 ; i < lineupArr.count(); i++){

        qDebug() << lineupArr.at(i)["GuideName"].toString() << " " <<
                lineupArr.at(i)["GuideNumber"].toString()<< " " <<
                lineupArr.at(i)["URL"].toString() << Qt::endl;

        channels.push_back(Channel(lineupArr.at(i)["GuideName"].toString().toStdString(),
                                    atof(lineupArr.at(i)["GuideNumber"].toString().toStdString().c_str()),
                                    lineupArr.at(i)["URL"].toString().toStdString()));
    }


    qDebug() << "Channel Name " << "Channel Number " << "URL " << Qt::endl;

    for (int i = 0; i < channels.size(); i++){
        qDebug() << QString::fromStdString(channels.at(i).getChannelName()) << " "
                 << channels.at(i).getNumber() << " "
                 << QString::fromStdString(channels.at(i).getURL()) << Qt::endl;
    }

     //Insert Each Member of Channel into Table view
    for (int i = 0 ; i < channels.size(); i++){
        ui->channelTable->insertRow(ui->channelTable->rowCount());

        ui->channelTable->setItem(ui->channelTable->rowCount() -1, NUMBER,
                                  new QTableWidgetItem(QString::number(channels.at(i).getNumber())));

        ui->channelTable->setItem(ui->channelTable->rowCount()-1, NAME,
                                 new QTableWidgetItem(QString::fromStdString(channels.at(i).getChannelName())));

        ui->channelTable->setItem(ui->channelTable->rowCount()-1, URL,
                                 new QTableWidgetItem(QString::fromStdString(channels.at(i).getURL())));
    }

    qDebug() << "Found " << channels.size() << " channels." << Qt::endl;



}

void MainWindow::on_launchButton_clicked()
{
    qDebug() << "Clicked Launch Button" << Qt::endl;
    if (ui->channelTable->rowCount() == 0)
        return void();

    QList<QTableWidgetItem *> qi = ui->channelTable->selectedItems();
    std::string selectedUrl = qi.at(URL)->text().toStdString();


    int fd = fork();
    qDebug() << fd << Qt::endl;
    if (fd < 0){
        qDebug() << "Fork Failed!\n";
        return void();
    }

    if (fd == 0){
        char *mpvArgs[] = {(char *)"mpv", (char *)selectedUrl.c_str(), NULL};
        int rc = execvp(mpvArgs[0], mpvArgs);
        if (rc < 0){
            qDebug() << "Exec Failed! Is MPV not installed?" << Qt::endl;
        }

    }

    selectedUrl.clear();

}
