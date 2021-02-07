#include <iostream>
#include <string>
#include <curl/curl.h>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <vector>
#include <libhdhomerun/hdhomerun.h>
#include "channel.h"


#ifndef HDHOMERUN_WRAPPER_H
#define HDHOMERUN_WRAPPER_H

class HDHomeRun_Wrapper
{
public:
    HDHomeRun_Wrapper();
    std::string getDeviceBaseURL();
    std::string getDeviceId();
    std::string getDeviceIPAddress();
    int getTunerCount();
    std::string getDeviceType();
    bool getIsLegacy();
    std::string getDeviceAuth();
    std::string getBaseUrl();
    std::string toString();
    std::vector<Channel> getChannels();
    static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp);

private:
        std::string deviceBaseUrl;
        std::string deviceIPAddress;
        std::string deviceId;
        int tunerCount;
        std::string deviceType;
        bool isLegacy;
        std::string deviceAuth;
        std::string baseUrl;
        std::vector<Channel> channels; 
     

};




#endif // HDHOMERUN_WRAPPER_H
