#include "headers/hdhomerun_wrapper.h"

#ifndef HDHOMERUN_WRAPPER_CPP
#define HDHOMERUN_WRAPPER_CPP

#define HOW_MANY 1

HDHomeRun_Wrapper::HDHomeRun_Wrapper()
{
   
    struct hdhomerun_discover_device_t device[1];
    int nDevice = hdhomerun_discover_find_devices_custom_v2(0, 
            HDHOMERUN_DEVICE_TYPE_TUNER,
            HDHOMERUN_DEVICE_ID_WILDCARD, 
            device,
            HOW_MANY);
    try
    {
        if (nDevice < -1 )
            throw "Error Occurred";
        if (nDevice == 0)
            throw "Could Not Find A Device!";

    }
    catch (std::exception &e)
    {
        exit(1);
    }
    this->deviceBaseUrl = (std::string) device[0].base_url;
    this->deviceIPAddress =  std::to_string((unsigned int) (device[0].ip_addr >> 24) & 0x0FF) +"."
        + std::to_string(((unsigned int) (device[0].ip_addr >> 16) & 0x0FF))+ "." 
        + std::to_string(((unsigned int) (device[0].ip_addr >> 8) & 0x0FF))+ "." 
        + std::to_string(((unsigned int) (device[0].ip_addr >> 0) & 0x0FF)); 
    this->tunerCount = (int) device[0].tuner_count; 
    this->isLegacy = device[0].is_legacy; this->deviceAuth = device[0].device_auth; 
    this->deviceType = std::to_string(device[0].device_type); 
    this->deviceId = std::to_string((int) device[0].device_id);
} 

std::string HDHomeRun_Wrapper::getDeviceBaseURL()
{
    return deviceBaseUrl;
}

std::string HDHomeRun_Wrapper::getDeviceIPAddress(){
   return deviceIPAddress;
}
std::string HDHomeRun_Wrapper::getDeviceId()
{
    return deviceId;
}
std::string HDHomeRun_Wrapper::getDeviceType()
{
    return deviceType;
}
bool HDHomeRun_Wrapper::getIsLegacy()
{
    return isLegacy;
}
std::string HDHomeRun_Wrapper::getDeviceAuth()
{
    return deviceAuth;
}
std::string HDHomeRun_Wrapper::getBaseUrl()
{
    return baseUrl;
}

std::string HDHomeRun_Wrapper::toString()
{
    return deviceBaseUrl + "|" + 
        deviceIPAddress + "|" +
        deviceId + "|" + 
        deviceType + "|" + 
        std::to_string(isLegacy) + "|" +
        deviceAuth + "|\n";
}

#endif
