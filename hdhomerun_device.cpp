#include "headers/hdhomerun_device.h"

#ifndef HDHOMERUN_DEVICE_CPP
#define HDHOMERUN_DEVICE_CPP

#define HOW_MANY 1

HDHomeRun_Device::HDHomeRun_Device()
{
   
    struct hdhomerun_discover_device_t device[1] {};
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

std::string HDHomeRun_Device::getDeviceBaseURL()
{
    return deviceBaseUrl;
}

std::string HDHomeRun_Device::getDeviceIPAddress(){
   return deviceIPAddress;
}
std::string HDHomeRun_Device::getDeviceId()
{
    return deviceId;
}
std::string HDHomeRun_Device::getDeviceType()
{
    return deviceType;
}
bool HDHomeRun_Device::getIsLegacy()
{
    return isLegacy;
}
std::string HDHomeRun_Device::getDeviceAuth()
{
    return deviceAuth;
}
std::string HDHomeRun_Device::getBaseUrl()
{
    return baseUrl;
}

std::string HDHomeRun_Device::toString()
{
    return deviceBaseUrl + "|" + 
        deviceIPAddress + "|" +
        deviceId + "|" + 
        deviceType + "|" + 
        std::to_string(isLegacy) + "|" +
        deviceAuth + "|\n";
}

#endif
