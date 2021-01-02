#include <iostream>
#include <string>
#include <libhdhomerun/hdhomerun.h>
#ifndef HDHOMERUN_DEVICE_H
#define HDHOMERUN_DEVICE_H

class HDHomeRun_Device
{
public:
    HDHomeRun_Device();
    std::string getDeviceBaseURL();
    std::string getDeviceId();
    std::string getDeviceIPAddress();
    int getTunerCount();
    std::string getDeviceType();
    bool getIsLegacy();
    std::string getDeviceAuth();
    std::string getBaseUrl();
    std::string toString();
private:
        std::string deviceBaseUrl;
        std::string deviceIPAddress;
        std::string deviceId;
        int tunerCount;
        std::string deviceType;
        bool isLegacy;
        std::string deviceAuth;
        std::string baseUrl;
        //std::vector<Channel> channels; 
     

};




#endif // HDHOMERUN_DEVICE_H
