
#include "headers/mainwindow.h"
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
 */

HDHomeRun_Wrapper MainWindow::auto_connect()
{

    HDHomeRun_Wrapper device = HDHomeRun_Wrapper();
    targetURL = "http://" + QString::fromStdString(device.getDeviceIPAddress()); 

    return device;
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
    std::vector <Channel>  channels = auto_connect().getChannels();
    for (int i = 0 ; i < channels.size(); i++){
        
         ui->channelTable->insertRow(ui->channelTable->rowCount());

        ui->channelTable->setItem(ui->channelTable->rowCount() -1, NUMBER,
                                  new QTableWidgetItem(QString::number(channels.at(i).getNumber())));

        ui->channelTable->setItem(ui->channelTable->rowCount()-1, NAME,
                                 new QTableWidgetItem(QString::fromStdString(channels.at(i).getChannelName())));

        ui->channelTable->setItem(ui->channelTable->rowCount()-1, URL,
                                 new QTableWidgetItem(QString::fromStdString(channels.at(i).getURL())));

    }

    qDebug() << "Found " << channels.size() << " channels." << "\n";



}

void MainWindow::on_launchButton_clicked()
{
    qDebug() << "Clicked Launch Button" << "\n";
    if (ui->channelTable->rowCount() == 0)
        return void();

    QList<QTableWidgetItem *> qi = ui->channelTable->selectedItems();
    std::string selectedUrl = qi.at(URL)->text().toStdString();


    int fd = fork();
    if (fd < 0){
        qDebug() << "Fork Failed!\n";
        return void();
    }

    if (fd == 0){
        char *mpvArgs[] = {(char *)"mpv", (char *)selectedUrl.c_str(), NULL};
        int rc = execvp(mpvArgs[0], mpvArgs);
        if (rc < 0){
            qDebug() << "Exec Failed! Is MPV not installed?" << "\n";
        }

    }

    selectedUrl.clear();

}
